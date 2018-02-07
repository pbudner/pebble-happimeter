/******************************************
 * Global Variables
 ******************************************
 */

var messageKeys = require('message_keys'); // Load message keys
var url = "http://192.168.179.23:4711/v1/";
var baseUrl = url;
// var url = "https://api.happimeter.org/v1/";
var watchToken = "";
var accountToken = "";

// create modules
var myServerCommunicationModule;
var withoutServer = true;

/******************************************
 * Event Listners
 ******************************************
 */

var addEventListerners = function () {

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
    Pebble.addEventListener('showConfiguration', function () {
        var url = 'http://config.happimeter.org/#';
        var config = {};

        var mail = localStorage.getItem("happimeter_mail");
        if (mail !== null) {
            config.mail = mail;
        }

        var hue = localStorage.getItem("happimeter_hue_username");
        if (hue !== null) {
            config.hue = hue;
        }

        var live = localStorage.getItem("happimeter_live");
        if (live !== null) {
            config.live = live;
        }

        var show_general_questions = localStorage.getItem("happimeter_show_general_questions");
        if (show_general_questions !== null) {
            config.show_general_questions = show_general_questions;
        }

        var show_general_questions2 = localStorage.getItem("happimeter_show_general_questions2");
        if (show_general_questions2 !== null) {
            config.show_general_questions2 = show_general_questions2;
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
            if (response.happimeter_hue_username !== false && response.happimeter_hue_username != "false") {
                localStorage.setItem("happimeter_hue_username", response.happimeter_hue_username);
                localStorage.setItem("happimeter_hue_group", response.happimeter_hue_group);
                console.log("(JS Hue) Added connection to bridge (username: " + response.happimeter_hue_username + ", lightgroup: " + response.happimeter_hue_group + ").");
            } else {
                console.log("(JS Hue) Removed connection to bridge.");
                localStorage.removeItem("happimeter_hue_username");
                localStorage.removeItem("happimeter_hue_group");
            }
        } else if ("happimeter_token" in response) {
            if (response.happimeter_token !== false && response.happimeter_token != "false") {
                console.log("(JS) Set login token to " + response.happimeter_token);
                Pebble.sendAppMessage({
                    'loggedin': 100 // this says we are logged in
                }, function () {
                    console.log('(JS) Message "user logged in" sent successfully to watch..');
                    localStorage.setItem("happimeter_token", response.happimeter_token);
                    localStorage.setItem("happimeter_mail", response.happimeter_mail);
                    myServerCommunicationModule.getPredictions();
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
                    localStorage.removeItem("happimeter_show_general_questions");
                }, function (e) {
                    console.log('(JS) Message "user logged out" failed: ' + JSON.stringify(e));
                });
            }
        } else if ("happimeter_live" in response) {
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
        } else if ("happimeter_show_general_questions" in response) {
            if (response.happimeter_show_general_questions != "false" && response.happimeter_show_general_questions !== false) {
                console.log("(JS) Show general questions mode..");
                localStorage.setItem("happimeter_show_general_questions", true);
                localStorage.removeItem("happimeter_show_general_questions2");
                Pebble.sendAppMessage({
                    'show_general_questions': 100
                }, function () {
                    console.log('(JS) Message "general questions mode enabled" sent successfully..');
                }, function (e) {
                    console.log('(JS) Message "Show general questions mode enabled" failed: ' + JSON.stringify(e));
                });
            } else {
                console.log("(JS) Set mode to not show general questions mode..");
                Pebble.sendAppMessage({
                    'hide_general_questions': 100
                }, function () {
                    console.log('(JS) Message "show general questions mode disabled" sent successfully..');
                    localStorage.removeItem("happimeter_show_general_questions");
                    localStorage.removeItem("happimeter_show_general_questions2");
                }, function (e) {
                    console.log('(JS) Message "show general questions mode disabled" failed: ' + JSON.stringify(e));
                });
            }
        } else if ("happimeter_show_general_questions2" in response) {
            if (response.happimeter_show_general_questions2 != "false" && response.happimeter_show_general_questions2 !== false) {
                console.log("(JS) Show general questions 2 mode..");
                localStorage.removeItem("happimeter_show_general_questions");
                localStorage.setItem("happimeter_show_general_questions2", true);
                Pebble.sendAppMessage({
                    'show_general_questions': 100
                }, function () {
                    console.log('(JS) Message "general questions mode enabled" sent successfully..');
                }, function (e) {
                    console.log('(JS) Message "Show general questions mode enabled" failed: ' + JSON.stringify(e));
                });
            } else {
                console.log("(JS) Set mode to not show general questions mode..");
                Pebble.sendAppMessage({
                    'hide_general_questions': 100
                }, function () {
                    console.log('(JS) Message "show general questions mode disabled" sent successfully..');
                    localStorage.removeItem("happimeter_show_general_questions");
                    localStorage.removeItem("happimeter_show_general_questions2");
                }, function (e) {
                    console.log('(JS) Message "show general questions mode disabled" failed: ' + JSON.stringify(e));
                });
            }
        }
    });

    // listen for AppMessages from the watch
    Pebble.addEventListener('appmessage', function (e) {

        var interpretAppMessageAndTriggerActions = function(dict) {
            var containsSensorData = "vmc" in dict;
            var containsQuestionAnswerData = "activation" in dict;
            var containsRequestForFriends = dict.friend_mail == "request";
            var containsRequestForPredictions = "retrieve_mood" in dict;
            var containsRequestForGenericQuestions = "retrieve_generic_question" in dict;

            if (containsSensorData) {
                console.log("(JS) Message contains sensor data.");
                saveSensorData(dict);
                myServerCommunicationModule.sendSensorData();
            }
            if (containsQuestionAnswerData) {
                console.log("(JS) Message contains mood data.");
                saveMoodData(dict);
                myServerCommunicationModule.sendMoodData();
            }
            if (containsRequestForFriends) {
                console.log("(JS) Pebble requested friend list.");
                retrieve_and_send_friends();
            }
            if (containsRequestForPredictions) {
                console.log("(JS) Message contains request to retrieve the predictions.");
                myServerCommunicationModule.getPredictions();
            }
            if (containsRequestForGenericQuestions) {
                console.log("(JS) Message contains request to retrieve the retrieve_generic_questions.");
                myServerCommunicationModule.retrieveGenericQuestionsFromServerAndSendToPebbleWatch();
            }
        };

        var dict = e.payload; // Get the dictionary from the message
        console.log('(JS) Got message from Pebble Watch: ' + JSON.stringify(dict));
        // try to get geolocation, but continue anyways
        navigator.geolocation.getCurrentPosition(function (pos) {
            dict.lat = pos.coords.latitude;
            dict.lon = pos.coords.longitude;
            dict.alt = pos.coords.altitude;
            interpretAppMessageAndTriggerActions(dict);
        }, function (err) {
            console.log(err);
            dict.lat = null;
            dict.lon = null;
            dict.alt = null;
            interpretAppMessageAndTriggerActions(dict);
        }, {enableHighAccuracy: true, maximumAge: 10000, timeout: 5000});


    });

};

/******************************************
 * save data
 ******************************************
 */

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

    SetPhilipsHue(dict.pleasant, dict.activation);

    items.push({
        'account_id': accountToken,
        'device_id': watchToken,
        'timestamp': dict.current_time,
        'local_timestamp': dict.local_time,
        'activation': dict.activation,
        'pleasance': dict.pleasant,
        'creativity': dict.creativity,
        'generic_values': dict.generic_values,
        'generic_question_count': dict.generic_question_count,
        'generic_question_group': localStorage.getItem("happimeter_show_general_questions2") == "true" ? 1 : 0,
        'position': {
            'lat': dict.lat,
            'lon': dict.lon
        },
    });
    console.log(JSON.stringify(items));
    localStorage.setItem("moodItems", JSON.stringify(items));
};

/******************************************
 * Helpers
 ******************************************
 */


var transformOldMoodDataIntoGenericStyle = function(moodData) {
    console.log('moodData: ');
    console.log(JSON.stringify(moodData));
    var genericMoodData = {};
    genericMoodData.timestamp = moodData.timestamp;
    genericMoodData.local_timestamp = moodData.local_timestamp;
    genericMoodData.position = moodData.position;
    genericMoodData['account_id'] = moodData['account_id'];
    genericMoodData['device_id'] = moodData['device_id'];
    genericMoodData.mood_answers = {};
    genericMoodData.mood_answers['1'] = moodData['activation'];
    genericMoodData.mood_answers['2'] = moodData['pleasance'];

    if (moodData.hasOwnProperty('generic_values')) {
        var generic_values =  moodData['generic_values'];
        var cachedGenericQuestions = JSON.parse(localStorage.getItem('cachedGenericQuestions'));

        var index = 0;
        while (index < generic_values.length) {
            if (generic_values[index] !== 99) {
                genericMoodData.mood_answers[cachedGenericQuestions[index]['id']] = generic_values[index];
            }
            index ++;
        }
    }
    console.log('transformed genericMoodData: ');
    console.log(JSON.stringify(genericMoodData));
};

// set the philips hue lights
var SetPhilipsHue = function (happiness, activation) {
    var username = localStorage.getItem("happimeter_hue_username");
    var lightgroup = localStorage.getItem("happimeter_hue_group");
    if (username) {
        var request = new XMLHttpRequest();
        request.onreadystatechange = function () {
            if (request.status == 200) {
                var response = JSON.parse(request.responseText);
                if (response.length > 0) {
                    var ip = response[0].internalipaddress;
                    var request_2 = new XMLHttpRequest();
                    console.log("(JS Hue) API URL is " + "http://" + ip + "/api/" + username + "/groups/" + lightgroup + "/action");
                    request_2.open("PUT", "http://" + ip + "/api/" + username + "/groups/" + lightgroup + "/action", false);
                    var brigthness = 250;
                    var x = 0;
                    var y = 0;
                    if (happiness == 2 && activation == 2) {
                        x = 0.3664211182481677;
                        y = 0.43373521075020305;
                    } else if (happiness == 2 && activation == 1) {
                        x = 0.6412284187421379;
                        y = 0.3353234676945936;
                    } else if (happiness == 2 && activation == 0) {
                        x = 0.5809241995073342;
                        y = 0.2697768194115517;
                    } else if (happiness == 1 && activation == 2) {
                        x = 0.33649454170095267;
                        y = 0.37464845490729193;
                    } else if (happiness == 1 && activation == 1) {
                        x = 0.32272672086556803;
                        y = 0.3290229095590793;
                    } else if (happiness == 1 && activation == 0) {
                        x = 0.172616103214012;
                        y = 0.05108838222345197;
                    } else if (happiness == 0 && activation == 2) {
                        x = 0.2693852552307843;
                        y = 0.22766417142964726;
                    } else if (happiness == 0 && activation == 1) {
                        x = 0.4362344376516375;
                        y = 0.18945691223968855;
                    } else if (happiness == 0 && activation == 0) {
                        x = 0.2834359366816283;
                        y = 0.10972421908809096;
                    }

                    request_2.send(JSON.stringify({"xy": [x, y], "bri": brigthness}));
                } else {
                    console.log("(JS Hue) Could not find a bridge in the network.");
                }
            }
        };
        request.open("GET", "https://www.meethue.com/api/nupnp", false);
        request.send(null);
    }
};

// retrieve and send the friends of the current user
var retrieve_and_send_friends = function () {
    var request = new XMLHttpRequest();
    request.onload = function () {
        console.log('Retrieved friends response: ' + this.responseText);
        var response = JSON.parse(this.responseText);
        if (response.status == 200) {
            var friends = response.friends.slice(0, 9);
            var send_friend = function (friends) {
                if (friends.length >= 1) {
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
                        if (friends.length >= 1) {
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

var transformQuestionsForPebble = function (questions) {
    var transformedQuestions = {
        'generic_question_count': questions.length
    }
    var i = 0;
    while (i < questions.length) {
        if (questions[i].hasOwnProperty('question')) {
            transformedQuestions['generic_question_desciption_' + (i + 1)] = questions[i]['question'];
        } else {
            transformedQuestions['generic_question_desciption_' + (i + 1)] = null
        }
        i++;
    }
    while (i < 5) {
        transformedQuestions['generic_question_desciption_' + (i + 1)] = null;
        i++;
    }
    console.log('generic questions will be send to pebble.')
    console.log(JSON.stringify(transformedQuestions));
    console.log('-------------')
    return transformedQuestions
};

var pushItemToLocalStorage = function (key, item) {
    var items;
    try {
        items = JSON.parse(localStorage.getItem(key));
    } catch (e) { // if key doesnt exists
        items = [];
    }
    items.push(item);
    localStorage.setItem(key, JSON.stringify(items));
};

/******************************************
 * Module for Communication with Pebble Server
 ******************************************
 */

var serverCommunicationModule = function serverCommunicationModule() {
    var genericQuestionsUrl = baseUrl + 'moods/genericquestions';
    var moodsUrl = baseUrl + 'moods-generic';
    var predictionUrl = baseUrl + 'classifier/predict-all';
    var sensorDataUrl = baseUrl + 'sensors';
    var token = localStorage.getItem("happimeter_token");

    // generalized functions for all post requests
    function doPostRequest(url, data, resolve, reject) {
        console.log('doPostRequest() called');
        var JSONString = typeof data == 'string' ? data : JSON.stringify(data);
        var xmlHttp = new XMLHttpRequest();
        xmlHttp.open('POST', url);
        xmlHttp.onreadystatechange = function () {
            if (xmlHttp.readyState === 4 && xmlHttp.status === 200) {
                resolve(JSON.parse(xmlHttp.responseText));
            } else if (xmlHttp.readyState === 4 && xmlHttp.status !== 200) {
                reject(xmlHttp.responseText, data);
            }
        };
        /* xmlHttp.onerror = function (e) {
            reject(JSON.stringify(e));
        }; */
        xmlHttp.setRequestHeader('Content-Type', 'application/json; charset=UTF-8');
        xmlHttp.setRequestHeader('Authorization', 'Bearer ' + token);
        xmlHttp.send(JSONString);
    }

    // generalized functions for all get requests
    function doGetRequest(theUrl, resolve, reject) {
        console.log('doGetRequest() called');
        console.log('url: ' + theUrl);
        var xmlHttp = new XMLHttpRequest();
        xmlHttp.onreadystatechange = function () {
            if (xmlHttp.readyState === 4) { // api call status is DONE
                if (xmlHttp.status < 400) {
                    resolve(JSON.parse(xmlHttp.responseText));
                } else {
                    reject(xmlHttp.responseText);
                }
            }
        };
        /* xmlHttp.onerror = function (e) {
            reject(JSON.stringify(e));
        };               */
        xmlHttp.open("GET", theUrl, true); // true for asynchronous
        xmlHttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
        xmlHttp.setRequestHeader("Authorization", "Bearer " + token);
        xmlHttp.send(null);
    }


    function retrieveGenericQuestionsFromServerAndSendToPebbleWatch() {
        console.log('retrieveGenericQuestionsFromServerAndSendToPebbleWatch() called');
        var sendGenericQuestionsToPebble = function (genericQuestions) {
            if (genericQuestions !== null) {    // only send if questions is set
                Pebble.sendAppMessage(transformQuestionsForPebble(genericQuestions), function () {
                    console.log('(JS) Message successfully sent the generic questions to the watch..');
                }, function (e) {
                    console.log('(JS) Message failed to send the generic questions to the watch: ' + JSON.stringify(e));
                });
            } else {
                console.log('genericQuestions are null');
            }
        };
        doGetRequest(genericQuestionsUrl, function resolve(response) {
            console.log('received generic questions from server:');
            const genericQuestions = response['questions'];
            console.log(JSON.stringify(genericQuestions));
            localStorage.setItem("cachedGenericQuestions", JSON.stringify(genericQuestions));
            sendGenericQuestionsToPebble(genericQuestions);
        }, function reject(error) {
            console.log('Failed to receive generic questions from server!');
            console.error(error);
            console.log('cached generic questions will be used if they are stored');
            try { // will fail if server request fails on first usage of app
                const cachedGenericQuestions = JSON.parse(localStorage.getItem('cachedGenericQuestions'));
                sendGenericQuestionsToPebble(cachedGenericQuestions);
            } catch (e) {
                console.log('No generic questions stored yet.');
            }
        });
    };

    // send saved sensor data from the phone to the server API
    function sendSensorData() {
        console.log('sendSensorData() called');
        var items = JSON.parse(localStorage.getItem("sensorItems"));
        if (!items || items.length === 0) {
            console.log("No sensor data stored. Exit.");
            return;
        }

        while (items.length > 0) {
            console.log(items.length + ' sensor data items remain for sending to server.');
            var sensorObj = items.shift();   // to be discussed: send only first item?
            doPostRequest(sensorDataUrl, sensorObj, function resolve(response) {
                console.log('Successfully sent sensor data to server.');
                console.log("response from sending sensor data: ");
                console.log(JSON.stringify(response));
            }, function reject(error, sensorObj) {
                console.error("Failed to send sensor data to server!");
                console.error(JSON.stringify(error));
                pushItemToLocalStorage('sensorItems', sensorObj); // push failed item back to storage
            });
        }
        localStorage.setItem('sensorItems', JSON.stringify([])); // clear storage
        Pebble.sendAppMessage({
            'app_callback': 100 // this says finished with upload
        }, function () {
            console.log('(JS) Message "finished with upload" sent successfully..');
        }, function (e) {
            console.log('(JS) Message "finished with upload" failed: ' + JSON.stringify(e));
        });
    };

    // retrieve the current mood and send it to the watch
    function getPredictions() {
        console.log("getPredictions() called");

        doGetRequest(predictionUrl, function resolve(response) {
            console.log('Successfully retrieved predictions');
            console.log(JSON.stringify(response));
            Pebble.sendAppMessage(response, function () {
                console.log('(JS) Message successfully sent predictions to the watch..');
            }, function (e) {
                console.log('(JS) Message failed to send predictions to the watch: ' + JSON.stringify(e));
            });
        }, function reject(error) {
            console.error('Failed to retrieve predictions!');
            console.error(JSON.stringify(error));
            Pebble.sendAppMessage({ // (-2,-2) means not trained yet
                'pleasant': -2,
                'activation': -2
            }, function () {
                console.log('(JS) Message successfully sent models have not been trained to the watch..');
            }, function (e) {
                console.log('(JS) Message failed to send models have not been trained to the watch: ' + JSON.stringify(e));
            });
        });
        /*
        var request = new XMLHttpRequest();
        request.onreadystatechange = function () {
            if (request.status == 200) {
                var response = JSON.parse(request.responseText);
                if (response.status == 200) {
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

                    // SetPhilipsHue(response.happiness, response.activation);
                } else if (response.status == 400) {
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
            console.log("ERROR:", e);
        };
        request.open("GET", url + "classifier/prediction", false);
        request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
        request.setRequestHeader("Authorization", "Bearer " + localStorage.getItem("happimeter_token"));
        request.send(null);
        console.log("Sent request..");    */
    };

    // send the saved mood data from the phone to the API
    function sendMoodData() {
        console.log('sendMoodData() called');
        var items = JSON.parse(localStorage.getItem("moodItems"));
        if (!items || items.length === 0) {
            console.log("There are no mood items to be send to server.");
            return;
        }


        while (items.length > 0) {
            console.log(items.length + ' mood data items remain for sending to server.');
            var moodObj = items.shift();
            if (moodObj == null) {
                console.log('Null Object in Mood data!');
                continue;
            }
            moodObj = transformOldMoodDataIntoGenericStyle(moodObj);
            doPostRequest(moodsUrl, moodObj, function resolve(response) {
                console.log('Successfully send mood data item to server.');
            }, function reject(error, moodObj) {
                console.error('Failed to send mood data item to server.');
                console.error(JSON.stringify(error));
                // todo: push again
                // pushItemToLocalStorage('moodItems', moodObj); // push failed item back to storage
            });
        }
        localStorage.setItem("moodItems", JSON.stringify([])); // clear storage
    };


    /**
     * MOCKUP methods for prediction and generic questions
     */
    // just mockup
    function mockupGetPredictions() {
        console.log('mockupGetPredictions() called');

        var mockupPredictions = {
            "predictions":
                { // "question_id": predicted_value
                    "1": Math.floor((Math.random() * 3)),
                    "2": Math.floor((Math.random() * 3)),
                    "5": Math.floor((Math.random() * 3)),
                    "16": Math.floor((Math.random() * 3))
                }
        }
        console.log(JSON.stringify(mockupPredictions));
        Pebble.sendAppMessage(mockupPredictions, function () {
            console.log('(JS) Message successfully sent predictions to the watch. (MOCKUP)');
        }, function (e) {
            console.log('(JS) Message failed to send predictions to the watch (MOCKUP): ' + JSON.stringify(e));
        });
    };

    // just mockup
    function mockupGetGenericQuestions() {
        console.log('mockupGetGenericQuestions() called');
        var mockupGenericQuestions = {
            "questions": [
                {
                    "question": "How active do you feel?",
                    "id": 1,
                    "watchface": "runner"
                },
                {
                    "question": "How pleasent do you feel?",
                    "id": 2,
                    "watchface": "smiley"
                },
                {
                    "question": "I am stressed",
                    "id": 5,
                    "watchface": "stressed"
                },
                {
                    "question": "I want more alcohol.",
                    "id": 16,
                    "watchface": "default"
                }
            ]
        };
        console.log(JSON.stringify(mockupGenericQuestions));
        Pebble.sendAppMessage(mockupGenericQuestions, function () {
            console.log('(JS) Message successfully sent the generic questions to the watch. (MOCKUP)');
        }, function (e) {
            console.log('(JS) Message failed to send the generic questions to the watch (MOCKUP): ' + JSON.stringify(e));
        });
    };
    /**
     * DELME MOCKUP methods for prediction and generic questions
     */
    // just mockup
    function delMemockupGetPredictionsForCompatibility() {
        console.log('delMemockupGetPredictionsForCompatibility() called');
        Pebble.sendAppMessage({
            'pleasant': 0,
            'activation': 0
        }, function () {
            console.log('(JS) Message successfully sent models have not been trained to the watch..');
        }, function (e) {
            console.log('(JS) Message failed to send models have not been trained to the watch: ' + JSON.stringify(e));
        })
    };

    // just mockup
    function delMemockupGetGenericQuestionsForCompatibiltity() {
        console.log('delMemockupGetGenericQuestionsForCompatibiltity() called');
        var mockupGenericQuestions = {
            "questions": [
                {
                    "question": "I am stressed",
                    "id": 5,
                    "watchface": "stressed"
                },
                {
                    "question": "I want more alcohol.",
                    "id": 16,
                    "watchface": "default"
                }
            ]
        };
        console.log(JSON.stringify(mockupGenericQuestions));
        Pebble.sendAppMessage(transformQuestionsForPebble(mockupGenericQuestions.questions), function () {
            console.log('(JS) Message successfully sent the generic questions to the watch. (MOCKUP)');
        }, function (e) {
            console.log('(JS) Message failed to send the generic questions to the watch (MOCKUP): ' + JSON.stringify(e));
        });
    };

    if (withoutServer) {
        return {
            "retrieveGenericQuestionsFromServerAndSendToPebbleWatch": mockupGetGenericQuestions,
            "sendSensorData": sendSensorData,
            "sendMoodData": sendMoodData,
            "getPredictions": mockupGetPredictions
        }
    } else {
        return {
            "retrieveGenericQuestionsFromServerAndSendToPebbleWatch": delMemockupGetGenericQuestionsForCompatibiltity,
            "sendSensorData": sendSensorData,
            "sendMoodData": sendMoodData,
            "getPredictions": delMemockupGetPredictionsForCompatibility
        }
    }
};

/******************************************
 * main()
 ******************************************
 */
(function mainIIFE() {
    console.log("JS start: main() called");
    myServerCommunicationModule = serverCommunicationModule();
    addEventListerners();
})();
