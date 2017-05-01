var messageKeys = require('message_keys'); // Load message keys

var url = "http://pascalbudner.de:8080/v1/";
var watchToken = "";
var accountToken = "";

var retrieve_current_mood = function() {
  console.log("Sending request...");
    var request = new XMLHttpRequest();
    request.onload = function () {
      var response = JSON.parse(request.responseText);
      if(response.status == 200) {
        console.log("Happiness: " + response.happiness);
        console.log("Activation: " + response.activation);
        Pebble.sendAppMessage({
          'pleasant': response.activation,
          'activation': response.happiness
        }, function () {
          console.log('(JS) Message Successfully sent the mood to the watch..');
        }, function (e) {
          console.log('(JS) Message Failed to send the mood to the watch: ' + JSON.stringify(e));
        });
      } else if(response.status == 400) {
        console.log("(JS) The model is not trained yet.");
        Pebble.sendAppMessage({ // (-2,-2) means not trained yet
          'pleasant': -2, 
          'activation': -2
        });
      } else {
        console.log("(JS) Did not receive a mood value: " + request.responseText);
      }
    };
    request.onerror = function (e) {
      console.log("ERROR:",e);
    };
    request.open("GET", url + "classifier/prediction");
    request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    request.setRequestHeader("Authorization", "Bearer " + localStorage.getItem("happimeter_token"));
    request.send(null);
    console.log("Sent request..");
};

// Listen for when the watchface is opened
Pebble.addEventListener('ready', function (e) {
    console.log('PebbleKit JS is ready!');
    watchToken = Pebble.getWatchToken();
    accountToken = Pebble.getAccountToken();
    console.log('(JS) Happimeter API url: ' + url);
    if (localStorage.getItem("happimeter_token") !== null) {
      console.log("(JS) Retrieve mood.");
      retrieve_current_mood();
    } else {
      console.log("(JS) User is not logged in.");
    }
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

    if (response.happimeter_token != false && response.happimeter_token != "false") {
        console.log("Set token to " + response.happimeter_token);
        localStorage.setItem("happimeter_token", response.happimeter_token);
        Pebble.sendAppMessage({
            'loggedin': 100 // this says we are logged in
        }, function () {
            console.log('(JS) Message "user logged in" sent successfully..');
        }, function (e) {
            console.log('(JS) Message "user logged in" failed: ' + JSON.stringify(e));
        });
    } else {
        console.log("Logged out!");
        localStorage.removeItem("sensorItems");
        localStorage.removeItem("moodItems");
        localStorage.removeItem("happimeter_token");
        Pebble.sendAppMessage({
            'loggedout': 100 // this says we are logged out
        }, function () {
            console.log('(JS) Message "user logged out" sent successfully..');
        }, function (e) {
            console.log('(JS) Message "user logged out" failed: ' + JSON.stringify(e));
        });
    }
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
          'alt': dict.alt,
        },
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
      
        console.log("Sending sensor obj: ", sensorObj);

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
            items.push(sensorObj);
            localStorage.setItem("sensorItems", JSON.stringify(items));
            sendFinishedWithUpload();
            sendData(items);
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
            sendData(items);
        };

        request.onerror = function (e) {
            console.log('Error during saving mood data: ' + e);
            items.push(moodObj);
            localStorage.setItem("moodItems", JSON.stringify(items));
            sendData(items);
        };

        // Send the request
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
            dict.lat = null;
            dict.lon = null;
            dict.alt = null;
            saveSensorData(dict);
            sendSensorData();
        }, { enableHighAccuracy: true, maximumAge: 10000, timeout: 5000 });
    } else if (dict.activation >= 0) 
        console.log("(JS) Message contains mood data..");{
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