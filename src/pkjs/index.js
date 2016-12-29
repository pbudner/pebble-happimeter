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

var url = "http://team9.coins.he-hosting.de/api/";
var watchToken = "";
var accountToken = "";

// Listen for when the watchface is opened
Pebble.addEventListener('ready', function (e) {
    console.log('PebbleKit JS is ready!');
    watchToken = Pebble.getWatchToken();
    accountToken = Pebble.getAccountToken();
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

var saveSensorData = function (dict) {
    var items = localStorage.getItem("sensorItems");
    if (!items) {
        items = [];
    } else {
        items = JSON.parse(items);
    }

    items.push({
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
        'PositionLat': dict.lat,
        'PositionLon': dict.lon,
        'Altitude': dict.alt
    });

    localStorage.setItem("sensorItems", JSON.stringify(items));
};

var sendSensorData = function () {
    var items = localStorage.getItem("sensorItems");
    if (!items) {
        return;
    }

    var sendData = function (items) {
        var sensorObj = items.shift();
        if (!sensorObj)
            return;

        // Create the request
        var request = new XMLHttpRequest();

        // Specify the callback for when the request is completed
        request.onload = function () {
            console.log('Got save sensor data response: ' + this.responseText);
            localStorage.setItem("sensorItems", JSON.stringify(items));
            sendFinishedWithUpload();
            sendData(items);
        };

        request.onerror = function (e) {
            console.log('Error during saving sensor data: ' + e);
            sendFinishedWithUpload();
        };

        // Send the request
        request.open("POST", url + "sensor/");
        request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
        request.send(JSON.stringify(sensorObj));
    };
    
    items = JSON.parse(items);
    if (!(!items) && items.length > 0) {
        sendData(items);
    } else {
        console.log("(JS) There are no sensor items to be send.");
    }
};

var saveMoodData = function (dict) {
    var items = localStorage.getItem("moodItems");
    console.log("(JS) Got mood items:", items);
    if (!items) {
        items = [];
    } else {
        items = JSON.parse(items);
    }

    items.push({
        'Account_ID': accountToken,
        'Device_ID': watchToken,
        'Timestamp': dict.current_time,
        'Activation': dict.activation,
        'Pleasant': dict.pleasant
    });

    localStorage.setItem("moodItems", JSON.stringify(items));
};

var sendMoodData = function () {
    var items = localStorage.getItem("moodItems");
    if (!items) {
        return;
    }

    var sendData = function (items) {
        var moodObj = items.shift();
        if (!moodObj)
            return;

        // Create the request
        var request = new XMLHttpRequest();

        // Specify the callback for when the request is completed
        request.onload = function () {
            console.log('Got save mood data response: ' + this.responseText);
            localStorage.setItem("moodItems", JSON.stringify(items));
            // sendFinishedWithUpload();
            sendData(items);
        };

        request.onerror = function (e) {
            console.log('Error during saving mood data: ' + e);
            // sendFinishedWithUpload();
        };

        // Send the request
        request.open("POST", url + "happiness/");
        request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
        request.send(JSON.stringify(moodObj));
    };
    
    items = JSON.parse(items);
    if (!(!items) && items.length > 0) {
        sendData(items);
    } else {
        console.log("(JS) There are no mood items to be send.");
    }
};

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage', function (e) {
    var dict = e.payload; // Get the dictionary from the message
    console.log('(JS) Got message: ' + JSON.stringify(dict));
    if (dict.vmc >= 0) {
        console.log("(JS) Message contains sensor data..");

        // Request current position
        navigator.geolocation.getCurrentPosition(function (pos) {
            dict.lat = pos.coords.latitude;
            dict.lon = pos.coords.longitude;
            dict.alt = pos.coords.altitude;
            saveSensorData(dict);
            sendSensorData();
        }, function (err) {
            console.log(err);
            saveSensorData(dict);
            sendSensorData();
        }, { enableHighAccuracy: true, maximumAge: 10000, timeout: 5000 });
    } else if (dict.activation >= 0) {
        console.log("(JS) Message contains mood data..");
        saveMoodData(dict);
        sendMoodData();
    }
});

var sendFinishedWithUpload = function () {
    Pebble.sendAppMessage({
        'app_callback': 100 // this says finished with upload
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