var messageKeys = require('message_keys'); // Load message keys

var url = "https://api.happimeter.org/v1/";
var watchToken = "";
var accountToken = "";

// listen until javascript is ready
Pebble.addEventListener('ready', function (e) {
    console.log('PebbleKit JS is ready!');
    watchToken = Pebble.getWatchToken();
    accountToken = Pebble.getAccountToken();
    console.log('(JS) Happimeter API url: ' + url);
    Pebble.sendAppMessage({
      'js_ready': 1
    }, function () {
      console.log('(JS) Sent that JS is ready..');
    }, function (e) {
      console.log('(JS) Failed to send that JS is ready: ' + JSON.stringify(e));
    });
  
    if (localStorage.getItem("happimeter_token") === null) {
      console.log("(JS) User is not logged in.");
    }
});

// listen for when configuration page should be opened
Pebble.addEventListener('showConfiguration', function() {
  var url = 'http://config.happimeter.org/#';
  var config = {};
  
  var mail = localStorage.getItem("happimeter_mail");
  if(mail !== null) {
    config.mail = mail;
  }
  
  var hue = localStorage.getItem("happimeter_hue_username");
  if(hue !== null) {
    config.hue = hue;
  }
  
  var live = localStorage.getItem("happimeter_live");
  if(live !== null) {
    config.live = live;
  }
  
  url += encodeURIComponent(JSON.stringify(config));
  Pebble.openURL(url);
});

// listen for when configuration page is closed
Pebble.addEventListener('webviewclosed', function (e) {
    
    if (e && !e.response) {
        return;
    }
  
    var response = JSON.parse(e.response);
    if ("happimeter_hue_username" in response) {
      if(response.happimeter_hue_username !== false && response.happimeter_hue_username != "false") { 
        localStorage.setItem("happimeter_hue_username", response.happimeter_hue_username);
        localStorage.setItem("happimeter_hue_group", response.happimeter_hue_group);
        console.log("(JS Hue) Added connection to bridge (username: "+response.happimeter_hue_username+", lightgroup: "+response.happimeter_hue_group+").");
      } else {
        console.log("(JS Hue) Removed connection to bridge.");
        localStorage.removeItem("happimeter_hue_username");
        localStorage.removeItem("happimeter_hue_group");
      }
    } else if ("happimeter_token" in response) {
      if(response.happimeter_token !== false && response.happimeter_token != "false") {
        console.log("(JS) Set login token to " + response.happimeter_token);
        Pebble.sendAppMessage({
          'loggedin': 100 // this says we are logged in
        }, function () {
          console.log('(JS) Message "user logged in" sent successfully to watch..');
          localStorage.setItem("happimeter_token", response.happimeter_token);
          localStorage.setItem("happimeter_mail", response.happimeter_mail);
          retrieve_current_mood();
        }, function (e) {
          console.log('(JS) Message "user logged in" failed: ' + JSON.stringify(e));
        });
      } else {
        console.log("(JS) Logged out!");
        Pebble.sendAppMessage({
          'loggedout': 100 // this says we are logged out
        }, function () {
          console.log('(JS) Message "user logged out" sent successfully to watch..');
          localStorage.removeItem("sensorItems");
          localStorage.removeItem("moodItems");
          localStorage.removeItem("happimeter_token");
          localStorage.removeItem("happimeter_mail");
          localStorage.removeItem("happimeter_live");
        }, function (e) {
          console.log('(JS) Message "user logged out" failed: ' + JSON.stringify(e));
        });
      }
    } else if("happimeter_live" in response) {
      if (response.happimeter_live != "false" && response.happimeter_live !== false) {
        console.log("(JS) Set mode to live mode..");
        Pebble.sendAppMessage({
          'live_mode': 100
        }, function () {
          console.log('(JS) Message "live mode enabled" sent successfully..');
          localStorage.setItem("happimeter_live", true);
        }, function (e) {
          console.log('(JS) Message "live mode enabled" failed: ' + JSON.stringify(e));
        });
      } else {
        console.log("(JS) Set mode to hourly mode..");
        Pebble.sendAppMessage({
          'hourly_mode': 100
        }, function () {
          console.log('(JS) Message "live mode disabled" sent successfully..');
          localStorage.removeItem("happimeter_live");
        }, function (e) {
          console.log('(JS) Message "live mode disabled" failed: ' + JSON.stringify(e));
        });
      }
    }
});

// set the philips hue lights
var SetPhilipsHue = function(happiness, activation) {
  var username = localStorage.getItem("happimeter_hue_username");
  var lightgroup = localStorage.getItem("happimeter_hue_group");
  if(username) {
    var request = new XMLHttpRequest();
    request.onreadystatechange = function () {
      if(request.status == 200) {
        var response = JSON.parse(request.responseText);
        if(response.length > 0) {
          var ip = response[0].internalipaddress;
          var request_2 = new XMLHttpRequest();
          console.log("(JS Hue) API URL is " + "http://" + ip + "/api/" + username + "/groups/"+lightgroup+"/action");
          request_2.open("PUT", "http://" + ip + "/api/" + username + "/groups/"+lightgroup+"/action", false);
          request_2.send(JSON.stringify({ "ct": 160 + parseInt(340 * happiness / 3), "bri": parseInt(250 * activation / 3) }));
        } else {
          console.log("(JS Hue) Could not find a bridge in the network.");
        }
      }
    };
    request.open("GET", "https://www.meethue.com/api/nupnp", false);
    request.send(null);
  }
};

// retrieve the current mood and send it to the watch
var retrieve_current_mood = function() {
  console.log("Sending request...");
    var request = new XMLHttpRequest();
    request.onreadystatechange = function () {
      if(request.status == 200) {
        var response = JSON.parse(request.responseText);
        if(response.status == 200) {
          console.log("Happiness: " + response.happiness);
          console.log("Activation: " + response.activation);
          Pebble.sendAppMessage({
            'pleasant': response.happiness,
            'activation': response.activation
          }, function () {
            console.log('(JS) Message successfully sent the mood to the watch..');
          }, function (e) {
            console.log('(JS) Message failed to send the mood to the watch: ' + JSON.stringify(e));
          });
          
          SetPhilipsHue(response.happiness, response.activation);
        } else if(response.status == 400) {
          console.log("(JS) The model is not trained yet.");
          Pebble.sendAppMessage({ // (-2,-2) means not trained yet
            'pleasant': -2, 
            'activation': -2
          });
        } else {
          console.log("(JS) Did not receive a mood value: " + request.responseText);
        }
      }
    };
    request.onerror = function (e) {
      console.log("ERROR:",e);
    };
    request.open("GET", url + "classifier/prediction", false);
    request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    request.setRequestHeader("Authorization", "Bearer " + localStorage.getItem("happimeter_token"));
    request.send(null);
    console.log("Sent request..");
};

// send a message to the watch that the upload is finished
var sendFinishedWithUpload = function () {
    Pebble.sendAppMessage({
        'app_callback': 100 // this says finished with upload
    }, function () {
        console.log('(JS) Message "finished with upload" sent successfully..');
    }, function (e) {
        console.log('(JS) Message "finished with upload" failed: ' + JSON.stringify(e));
    });
};

// save the sensor data on the phone
var saveSensorData = function (dict) {
    var items = localStorage.getItem("sensorItems");
    if (!items) {
        items = [];
    } else {
        items = JSON.parse(items);
    }
  
    var currentTime = dict.current_time;
    var d = new Date();
    var hour_difference = d.getTimezoneOffset(); // UTC time offset
    var localTime = currentTime - hour_difference * 60;

    items.push({
        'device_id': watchToken,
        'timestamp': currentTime,
        'local_timestamp': localTime,
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

// send saved sensor data from the phone to the API
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

// save mood data on the phone
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
        'pleasance': dict.pleasant,
        'position': {
          'lat': dict.lat,
          'lon': dict.lon
        },
    });

    localStorage.setItem("moodItems", JSON.stringify(items));
};

// send the saved mood data from the phone to the API
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

// retrieve and send the friends of the current user
var retrieve_and_send_friends = function() {
  var request = new XMLHttpRequest();
  request.onload = function () {
    console.log('Retrieved friends response: ' + this.responseText);
    var response = JSON.parse(this.responseText);
    if(response.status == 200) {
      var friends = response.friends.slice(0,9);
      var send_friend = function(friends) {
        if(friends.length >= 1) {
          var friend = friends.shift();
          var happiness = friend.user.mood.pleasance;
          var activation = friend.user.mood.activation;
          var name = friend.user.name;
          name = name === null ? "" : name;
          happiness = happiness === null ? -1 : happiness;
          activation = activation === null ? -1 : activation;
          Pebble.sendAppMessage({
              'friend_name': name,
              'friend_mail': friend.user.mail,
              'pleasant': happiness,
              'activation': activation,
          }, function () {
              console.log('(JS) Message containing friends data successfully sent..');
              if(friends.length >= 1) {
                send_friend(friends);
              }
          }, function (e) {
              console.log('(JS) Message containing friends data failed: ' + JSON.stringify(e));
          });
        } else {
          Pebble.sendAppMessage({
              'friend_name': "na",
              'friend_mail': "na",
              'pleasant': -10,
              'activation': -10,
          }, function () {
              console.log('(JS) Message containing friends data successfully sent..');
          }, function (e) {
              console.log('(JS) Message containing friends data failed: ' + JSON.stringify(e));
          });
        }
      };
      send_friend(friends);
    }
  };

  request.onerror = function (e) {
    console.log('Error during retrieving friends data: ' + e);
  };

  // Send the request
  request.open("GET", url + "friends");
  request.setRequestHeader("Authorization", "Bearer " + localStorage.getItem("happimeter_token"));
  request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
  request.send(null);
};

// listen for AppMessages from the watch
Pebble.addEventListener('appmessage', function (e) {
    var dict = e.payload; // Get the dictionary from the message
    console.log('(JS) Got message: ' + JSON.stringify(dict));
    if ("vmc" in dict) {
        console.log("(JS) Message contains sensor data..");
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
    } else if ("activation" in dict) {
        console.log("(JS) Message contains mood data..");
        navigator.geolocation.getCurrentPosition(function (pos) {
            dict.lat = pos.coords.latitude;
            dict.lon = pos.coords.longitude;
            dict.alt = pos.coords.altitude;
            saveMoodData(dict);
            sendMoodData();
        }, function (err) {
            console.log(err);
            dict.lat = null;
            dict.lon = null;
            dict.alt = null;
            saveMoodData(dict);
            sendMoodData();
        }, { enableHighAccuracy: true, maximumAge: 10000, timeout: 5000 });
    } else if (dict.friend_mail == "request") {
        console.log("(JS) Pebble requested friend list..");
        retrieve_and_send_friends();
    } else if ("retrieve_mood" in dict) {
      console.log("(JS) Message contains request to retrieve the current mood..");
      retrieve_current_mood();
    }
});
