var Clay = require('pebble-clay'); // Import the Clay package
var clayConfig = require('./config'); // Load our Clay configuration file
var customClay = require('./custom-clay'); // Load our custom function Clay file
var messageKeys = require('message_keys'); // Load message keys

// Initialize Clay
var clay = new Clay(clayConfig, customClay, {
    'userData': {
        'apiTokenIsSet': localStorage.getItem("happimeter_api_token") != null,
        'serverInformation': localStorage.getItem("happimeter_server_info")
    }
});

var url = "";
var watchToken = "";
var apiToken = "";

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
    function (e) {
        console.log('PebbleKit JS ready!');
        watchToken = Pebble.getWatchToken();
        url = "http://team9.coins.he-hosting.de/api/";
        console.log('(JS) Happimeter API url: ' + url);
        apiToken = localStorage.getItem("happimeter_api_token");
        console.log('(JS) API token: ' + apiToken);
    }
);

Pebble.addEventListener('webviewclosed', function (e) {
    if (e && !e.response) {
        return;
    }

    var dict = clay.getSettings(e.response);

    if (!apiToken) {
        // sign up first and then update user profile data
        var mail = dict[messageKeys.userinfo_email];
        console.log("(JS) Got mail: " + mail);
        var request = new XMLHttpRequest();
        request.onload = function () {
            var response = JSON.parse(this.responseText);
            if (response.Status == 0) {
                console.log('(JS) Error during user signup: ' + response.Result);
                localStorage.setItem("happimeter_server_info", response.Result);
            } else {
                localStorage.setItem("happimeter_server_info", false);
                localStorage.setItem("happimeter_api_token", response.Token);
                apiToken = response.Token;
                console.log("Succesfully signed up password is " + response.Password);
                setUserInfo(dict);
            }
        };
        request.open("POST", url + "user/");
        request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
        request.send(JSON.stringify({
            'Mail': mail,
            'DeviceID': watchToken
        }));
    } else {
        // just update user profile data
        setUserInfo(dict);
    }
});

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage', function (e) {
    // Get the dictionary from the message
    var dict = e.payload;

    console.log('(JS) Got message: ' + JSON.stringify(dict));

    var options = {
        enableHighAccuracy: true,
        maximumAge: 10000,
        timeout: 10000
    };

    if (dict.steps >= 0) {
        console.log("Message contains sensor data..");

        if (!apiToken) {
            console.warn("(JS) Cannot process sensor data, since now API token is specified..");
            return;
        }

        // Request current position
        navigator.geolocation.getCurrentPosition(function (pos) {
            console.log("Received average accelerometer z: " + dict.avg_acc_z);
            // Create the request
            var request = new XMLHttpRequest();

            // Specify the callback for when the request is completed
            request.onload = function () {
                console.log('Got save sensor data response: ' + this.responseText);
                sendFinishedWithUpload();
            };

            request.onerror = function (e) {
                console.log('Error during saving sensor data: ' + e);
                sendFinishedWithUpload();
            };

            // Send the request
            request.open("POST", url + "sensor/");
            request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
            request.send(JSON.stringify({
                'Token': apiToken,
                'Timestamp': dict.current_time,
                'Steps': dict.steps,
                'Activity': dict.activity,
                'AvgBPM': dict.avg_heart_rate,
                'acc_avg_x': dict.avg_acc_x,
                'acc_var_x': dict.var_acc_x,
                'acc_avg_y': dict.avg_acc_y,
                'acc_var_y': dict.var_acc_y,
                'acc_avg_z': dict.avg_acc_z,
                'acc_var_z': dict.var_acc_z,
                'VMC': dict.vmc,
                'AvgLightLevel': dict.avg_light_level,
                'PositionLat': pos.coords.latitude,
                'PositionLon': pos.coords.longitude,
                'Altitude': pos.coords.altitude
            }));
        }, function (err) {
            console.log(err);
            sendFinishedWithUpload();
        }, options);
    } else if (dict.happiness >= 0) {
        console.log('Message contains happiness data..');
        
        if (!apiToken) {
            console.warn("(JS) Cannot process sensor data, since now API token is specified..");
            return;
        }

        var request = new XMLHttpRequest();

        // Specify the callback for when the request is completed
        request.onload = function () {
            console.log('Got happiness response: ' + this.responseText);
        };

        request.onerror = function (e) {
            console.log("Got error response:", e.target.status);
        };

        // Send the request
        request.open("POST", url + "happiness/");
        request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
        request.send(JSON.stringify({
            'Token': apiToken,
            'Timestamp': Math.round(new Date() / 1000),
            'happiness': dict.happiness,
            'whohaveyoubeenwith': dict.who_have_you_been_with,
            'didyoudosports': dict.did_any_sports
        }));
    }
});

var sendFinishedWithUpload = function () {
    Pebble.sendAppMessage({
        'app_callback': 100 // this says finished with upload
    }, function () {
        console.log('Message "finished with upload" sent successfully..');
    }, function (e) {
        console.log('Message "finished with upload" failed: ' + JSON.stringify(e));
    });
};

var settingsChanged = function (mail) {
    console.log("(JS) Mail is " + mail);
};

var setUserInfo = function (dict) {
    var request = new XMLHttpRequest();
    request.onload = function () {
        var response = JSON.parse(this.responseText);
        if (response.Status == 0) {
            console.log('(JS) Error during updating user profile: ' + response.Result);
            localStorage.setItem("happimeter_server_info", response.Result);
        } else {
            console.log("Succesfully updated the profile..");
        }
    };
    request.open("POST", url + "userinfo/");
    request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    request.send(JSON.stringify({
        'Token': apiToken,
        'Name': dict[messageKeys.userinfo_name],
        'Age': dict[messageKeys.userinfo_age],
        'Weight': dict[messageKeys.userinfo_weight],
        'Sportiness': dict[messageKeys.userinfo_sportiness]
    }));
};