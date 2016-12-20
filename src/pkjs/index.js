var Clay = require('pebble-clay'); // Import the Clay package
var clayConfig = require('./config'); // Load our Clay configuration file
var customClay = require('./custom-clay'); // Load our custom function Clay file
var messageKeys = require('message_keys'); // Load message keys

// Initialize Clay
var clay = new Clay(clayConfig, customClay, {
    'userData': {
        'serverInfo': localStorage.getItem("happimeter_server_info")
    }
});

var url = "";
var watchToken = "";
var accountToken = "";

// Listen for when the watchface is opened
Pebble.addEventListener('ready', function (e) {
    console.log('PebbleKit JS is ready!');
    watchToken = Pebble.getWatchToken();
    accountToken = Pebble.getAccountToken();
    url = "http://team9.coins.he-hosting.de/api/";
    console.log('(JS) Happimeter API url: ' + url);
    console.log('(JS) Watch Token: ' + watchToken);
    console.log('(JS) Account Token: ' + accountToken);
});

Pebble.addEventListener('webviewclosed', function (e) {
    if (e && !e.response) {
        return;
    }

    var dict = clay.getSettings(e.response);

    // sign up first and then update user profile data
    var mail = dict[messageKeys.userinfo_email];
    console.log("(JS) Connect to mail: " + mail);
    var request = new XMLHttpRequest();
    request.onload = function () {
        var response = JSON.parse(this.responseText);
        if (response.Status == 0) {
            console.log('(JS) Error during user signup: ' + response.Result);
            localStorage.setItem("happimeter_server_info", response.Result);
        } else {
            localStorage.setItem("happimeter_server_info", false);
            // setUserInfo(dict);
        }
    };
    request.open("POST", url + "user/");
    request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    request.send(JSON.stringify({
        'Mail': mail,
        'Device_ID': watchToken,
        'Account_ID': accountToken
    }));
});

var sendSensorData = function (dict, lat, lon, alt) {
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

    console.log('(JS) AVG LIGHT LEVEL is: ' + dict.avg_light_level);

    // Send the request
    request.open("POST", url + "sensor/");
    request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    request.send(JSON.stringify({
        'Account_ID': accountToken,
        'Device_ID': watchToken,
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
        'avglightlevel': dict.avg_light_level,
        'PositionLat': lat,
        'PositionLon': lon,
        'Altitude': alt
    }));
}

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

    if (dict.current_time >= 0) {
        console.log("(JS) Message contains sensor data..");

        // Request current position
        navigator.geolocation.getCurrentPosition(function (pos) {
            sendSensorData(dict, pos.coords.latitude, pos.coords.longitude, pos.coords.altitude);
        }, function (err) {
            console.log(err);
            sendSensorData(dict, null, null, null);
        }, options);
    } else if (dict.activation >= 0) {
        console.log('(JS) Message contains happiness data..');
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
            'Account_ID': accountToken,
            'Device_ID': watchToken,
            'Timestamp': Math.round(new Date() / 1000),
            'activation': dict.activation,
            'pleasant': dict.pleasant
        }));
    }
});

var sendFinishedWithUpload = function () {
    Pebble.sendAppMessage({
        'app_callback': 100 // this says I'm finished with upload
    }, function () {
        console.log('(JS) Message "finished with upload" sent successfully..');
    }, function (e) {
        console.log('(JS) Message "finished with upload" failed: ' + JSON.stringify(e));
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
            console.log("(JS) Succesfully updated the profile..");
        }
    };
    request.open("POST", url + "userinfo/");
    request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    request.send(JSON.stringify({
        'Account_ID': accountToken,
        'Name': dict[messageKeys.userinfo_name],
        'Age': dict[messageKeys.userinfo_age],
        'Weight': dict[messageKeys.userinfo_weight],
        'Height': dict[messageKeys.userinfo_height],
        'Sportiness': dict[messageKeys.userinfo_sportiness]
    }));
};