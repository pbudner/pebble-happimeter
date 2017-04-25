var messageKeys = require('message_keys'); // Load message keys

var url = "http://pascalbudner.de:8080/v1/";
var watchToken = "";
var accountToken = "";

// Listen for when the watchface is opened
Pebble.addEventListener('ready', function (e) {
    console.log('PebbleKit JS is ready!');
    watchToken = Pebble.getWatchToken();
    accountToken = Pebble.getAccountToken();
    console.log('(JS) Happimeter API url: ' + url);
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'http://team9.coins.he-hosting.de/config/config.html';
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function (e) {
    if (e && !e.response) {
        return;
    }
  
    response = JSON.parse(e.response);
    console.log("Set token to " + response.happimeter_token);
    localStorage.setItem("happimeter_token", response.happimeter_token);
    Pebble.sendAppMessage({
        'loggedin': 100 // this says we are logged in
    }, function () {
        console.log('(JS) Message "user logged in" sent successfully..');
    }, function (e) {
        console.log('(JS) Message "user logged in" failed: ' + JSON.stringify(e));
    });
});

var saveSensorData = function (dict) {
    var items = localStorage.getItem("sensorItems");
    if (!items) {
        items = [];
    } else {
        items = JSON.parse(items);
    }

    items.push({
        'account_id': accountToken,
        'device_id': watchToken,
        'timestamp': dict.current_time,
        'activity': dict.activity,
        'avg_bpm': dict.avg_heart_rate,
        'accelerometer': {
          'avg_x': dict.avg_acc_x,
          'var_x': dict.var_acc_x,
          'avg_y': dict.avg_acc_y,
          'var_y': dict.var_acc_y,
          'avg_z': dict.avg_acc_z,
          'var_z': dict.var_acc_z,
        },
        'vmc': dict.vmc,
        'avg_light_level': dict.avg_light_level,
        'position': {
          'lat': dict.lat,
          'lon': dict.lon,
          'alt': dict.alt
        }
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
        request.open("POST", url + "sensors");
        request.setRequestHeader("Authorization", "Bearer " + localStorage.getItem("happimeter_token"));
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
  
    console.log("Dict is ", dict);
    
    items.push({
        'account_id': accountToken,
        'device_id': watchToken,
        'timestamp': dict.current_time,
        'local_timestamp': dict.local_time,
        'activation': dict.activation,
        'pleasance': dict.pleasant
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
        console.log("Auth Header is: Bearer " + localStorage.getItem("happimeter_token"));
        request.open("POST", url + "moods");
        request.setRequestHeader("Authorization", "Bearer " + localStorage.getItem("happimeter_token"));
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