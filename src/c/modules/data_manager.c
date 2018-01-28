#include "data_manager.h"

static int current_measurement_id = 0;
static int pleasant; // saving pleasing answer
static int activation; // saving activation answer
static int creativity; // saving creativity answer

static int numberOfGenericQuestions = 0;
static int mood_id;
static int genericValues[10]; //values of the generic sampling windows
static char genericDescriptions[10][255]; //the descriptinos for the generic sampling windows

static bool wait_for_upload_finish = true;
static bool handled_all_data_items = false;
static bool open_upload_data_task = false;

/***********************************
* Returns weather an open upload   *
* data task is pending.            *
***********************************/
bool is_open_upload_task() {
  return open_upload_data_task;
}

/***********************************
* Sets the users happiness answer  *
***********************************/
void setHappiness(int _pleasant) {
  pleasant = _pleasant;
}

/***********************************
* Sets the users answer to the     *
* generic question with id = index *
***********************************/
void setGenericValue(int index, int value) {
  genericValues[index] = value;
}

void setNumberOfGenericQuestions(int number) {
  numberOfGenericQuestions = number;
}

/***********************************
* Sets the users answer to the     *
* generic question with id = index *
***********************************/
void setGenericDescription(int index, char value[255]) {
  strcpy(genericDescriptions[index], value);
}

/***********************************
* Sets the users creativity answer  *
***********************************/
void setCreativity(int _creativity) {
  creativity = _creativity;
}

/***********************************
* Sets the users activation answer *
***********************************/
void setActivation(int _activation) {
  activation = _activation;
}

/***********************************
* Returns the pleasnt answer.      *
***********************************/
int getPleasant() {
  return pleasant;
}

/***********************************
* Returns the activation answer.   *
***********************************/
int getActivation() {
  return activation;
}

/***********************************
* Returns the creativity answer.   *
***********************************/
int getCreativity() {
  return creativity;
}

/***********************************
* Gets the users answer to the     *
* generic question with id = index *
***********************************/
int getGenericValue(int index) {
  return genericValues[index];
}

int getNumberOfGenericQuestions() {
  return numberOfGenericQuestions;
}

/***********************************
* Sets the users answer to the     *
* generic question with id = index *
***********************************/
char* getGenericDescription(int index) {
  return genericDescriptions[index];
}

/***********************************
* Marks an data upload as finished *
***********************************/
void received_finished_upload() {
  wait_for_upload_finish = false;
  remove_all_measures(current_measurement_id - 1); // remove the measures for the last uploaded id
  upload_measure();
}

/**********************************
* Requests the friends list       *
**********************************/
void request_friends() {  
  DictionaryIterator *out_iter;
  app_message_open(64, 256); // open the app message
  AppMessageResult result = app_message_outbox_begin(&out_iter); // prepare the outbox buffer for this message
  if(result == APP_MSG_OK) {
    dict_write_cstring(out_iter, MESSAGE_KEY_friend_mail, "request");
    result = app_message_outbox_send(); // send this message
    if(result != APP_MSG_OK) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the request friend outbox: %d", (int)result);
    } else {
      APP_LOG(APP_LOG_LEVEL_INFO, "Succesfully sent the request friend outbox: %d", (int)result);
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the request friends outbox: %d", (int)result);
  }
}

/**********************************
* Requests the mood               *
**********************************/
void request_mood() { 
  DictionaryIterator *out_iter;
  app_message_open(64, 256); // open the app message
  AppMessageResult result = app_message_outbox_begin(&out_iter); // prepare the outbox buffer for this message
  if(result == APP_MSG_OK) {
	dict_write_cstring(out_iter, MESSAGE_KEY_retrieve_mood, "1");
    dict_write_cstring(out_iter, MESSAGE_KEY_retrieve_mood, "1");
    dict_write_cstring(out_iter, MESSAGE_KEY_retrieve_generic_question, "1");
    result = app_message_outbox_send();
    if(result != APP_MSG_OK) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the mood request outbox: %d", (int)result);
    } else {
      APP_LOG(APP_LOG_LEVEL_INFO, "Succesfully sent the mood request outbox: %d", (int)result);
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the mood request outbox: %d", (int)result);
  }
}


/**********************************
* Pings the javascript app        *
**********************************/
void ping_js() {
  DictionaryIterator *out_iter;
  app_message_open(64, 256); // open the app message
  AppMessageResult result = app_message_outbox_begin(&out_iter); // prepare the outbox buffer for this message
  if(result == APP_MSG_OK) {
    dict_write_cstring(out_iter, MESSAGE_KEY_ping, "1");
    result = app_message_outbox_send();
    if(result != APP_MSG_OK) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error pinging the javascript app: %d", (int)result);
    } else {
      APP_LOG(APP_LOG_LEVEL_INFO, "Succesfully pinged the javacsript app: %d", (int)result);
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox to ping the javascript app: %d", (int)result);
  }
}

/***********************************
* Checks whether the entire data   *
* uploading process is finished.   * 
***********************************/
void check_whether_upload_process_is_finished() {
  APP_LOG(APP_LOG_LEVEL_INFO, "Wait for upload to be finished: %d", wait_for_upload_finish);
  APP_LOG(APP_LOG_LEVEL_INFO, "Handled all data items: %d", handled_all_data_items);

  if(!wait_for_upload_finish && handled_all_data_items) {
    if(window_stack_get_top_window() == upload_window_get_window()) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Exit app, since upload process finished.");
      window_stack_pop_all(false);
    }
  }
}

/***********************************
* Uploads the mood dataset.        *
***********************************/
void upload_mood(int pleasant, int activation, int creativity, uint8_t genericValues[5]) {
  DictionaryIterator *out_iter;
  int _numberOfgenericValues = getNumberOfGenericQuestions();
  app_message_open(64, 256); // open the app message
  AppMessageResult result = app_message_outbox_begin(&out_iter); // prepare the outbox buffer for this message
  if(result == APP_MSG_OK) {
    
    time_t now = time(NULL);
    struct tm * utc_time = gmtime(&now);
    int utc_unix_time = mktime(utc_time);
    struct tm * local_time = localtime(&now); 
    static char time_buffer[] = "-0400";
    strftime(time_buffer, sizeof(time_buffer), "%z", local_time);
    double difference_to_utc_in_hours = atoi(time_buffer) / 100.0 + local_time->tm_isdst;
    int local_unix_time = utc_unix_time + difference_to_utc_in_hours * 60 * 60;
    APP_LOG(APP_LOG_LEVEL_INFO, "Current time is %d", local_unix_time);
    APP_LOG(APP_LOG_LEVEL_INFO, "UTC time is %d", utc_unix_time);
    dict_write_int(out_iter, MESSAGE_KEY_current_time, &utc_unix_time, sizeof(int), true);
    dict_write_int(out_iter, MESSAGE_KEY_local_time, &local_unix_time, sizeof(int), true);
    dict_write_int(out_iter, MESSAGE_KEY_pleasant, &pleasant, sizeof(int), true);
    dict_write_int(out_iter, MESSAGE_KEY_activation, &activation, sizeof(int), true);
    dict_write_int(out_iter, MESSAGE_KEY_generic_question_count, &_numberOfgenericValues, sizeof(int), true);
    dict_write_data(out_iter, MESSAGE_KEY_generic_values, genericValues, sizeof(uint8_t) * 5);
    result = app_message_outbox_send(); // send this message
   if(result != APP_MSG_OK) { 
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the mood outbox: %d", (int)result);
    } else {
      APP_LOG(APP_LOG_LEVEL_INFO, "Succesfully sent the mood outbox: %d", (int)result);
    }
  } else {
    // the outbox cannot be used right now

    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the mood outbox: %d", (int)result);
  }
  
  // reset the counter for the generic questions
  reset_generic_question_counter();
}

/***********************************
* Saves a mood on the storage      *
***********************************/
void save_storage_mood(int pleasant, int activation, int creativity, uint8_t genericValues[5]) {
        uint32_t k =7;
    if(persist_exists(k)){
      mood_id = persist_read_int(k)+1; 
      persist_write_int(k, mood_id);
    }else{
      persist_write_int(k, 1);
      mood_id = persist_read_int(k);
    }
  
  //type: pleasant = 1, activation = 2, creativity = 3, genericValues = 4;     
      save_mood(1, pleasant, mood_id);
      save_mood(2, activation, mood_id);
      save_mood(3, creativity, mood_id);
  
   int values[5];
      for(int i = 0; i<5; i++){
        values[i] = genericValues[i];
      }
      save_generic_values(4, values, mood_id);
}


/***********************************
* Saves a mood for the given id *
* and type.                        *
***********************************/
void save_mood(int type, int mood, int mood_id){
  uint32_t key = mood_id * 1000 + type;
    APP_LOG(APP_LOG_LEVEL_INFO, "Current key: %d.", key);
  persist_write_int(key, mood);
 if(persist_exists(key)){
   APP_LOG(APP_LOG_LEVEL_INFO, "There is a key. Current key: %d.", key);
 }
}

/***********************************
* Saves the generic Values for a   * 
* given id and type.               *
**********************************/
void save_generic_values(int type, int mood[], int mood_id){
  for(int i =0; i<5;i++){
    uint32_t key = mood_id * 1000 + type + i;
    int value = mood[i]; 
    persist_write_int(key, value);
  }
}

/***********************************
* gets the cached answers and calls *
* upload_mood to start the upload   *
**********************************/
void upload_mood_from_storage(int mood_id){
  
   APP_LOG(APP_LOG_LEVEL_INFO, "Opened upload mood from storage %d.", current_measurement_id);
    int p =  get_mood(mood_id, 1, true);
  APP_LOG(APP_LOG_LEVEL_INFO, "Pleasure: %d.", p);
    int a =  get_mood(mood_id, 2, true);
  APP_LOG(APP_LOG_LEVEL_INFO, "Activity: %d.", a);
    int c =  get_mood(mood_id, 3, true);
  APP_LOG(APP_LOG_LEVEL_INFO, "Creativity %d.", c);
  
  uint8_t values[5];
   for(int i =0; i<5;i++){
      uint32_t key = mood_id * 1000 + 4 + i;
      if(persist_exists(key)) {
          values[i] = persist_read_int(key);
          persist_delete (key);
      } else{
          values[i] = 99;
      }
  }    
  
  if(p!=-1 || a!=-1 || c!=-1 ){
    upload_mood(p,a,c, values);
  }
}

/*************************************
* reads the value in the storage for *
* a given id and type. delets the    *
* value from the storage and returns *
* it                                 *
*************************************/
int get_mood(int id, int type, int removeAfterRead) {
  int key = id * 1000 + type;
  APP_LOG(APP_LOG_LEVEL_INFO, "Current key: %d.", key);
  if(persist_exists(key)) {
    int value = persist_read_int(key);
    if(removeAfterRead) {
      persist_delete (key);
    }
    return value;
  }
  return -1;
}

/***********************************
* starts the upload of the saved   *
* mood data                        *
***********************************/
void start_upload(){
   uint32_t k =7;
    if(persist_exists(k)){
      mood_id = persist_read_int(k); 
      if(mood_id>0){
        upload_mood_from_storage(mood_id);
        mood_id = mood_id-1;
        persist_write_int(k, mood_id);  
        psleep(5000);
      } 
   }
}

/***********************************
* Checks whether a further dataset *
* exist and sends it to the app js *
***********************************/
void upload_measure() {
   
  APP_LOG(APP_LOG_LEVEL_INFO, "Opened upload measure %d.", current_measurement_id);
  if(get_measure(current_measurement_id, 10, false) != -1) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Found a dataset with the id %d.", current_measurement_id);

    // receive all measurement data and delete them
    int current_time = get_measure(current_measurement_id, 10, false);
    int activity = get_measure(current_measurement_id, 11, false);
    int avg_heart_rate = get_measure(current_measurement_id, 12, false);
    int avg_acc_x = get_measure(current_measurement_id, 13, false);
    int var_acc_x = get_measure(current_measurement_id, 14, false);
    int avg_acc_y = get_measure(current_measurement_id, 15, false);
    int var_acc_y = get_measure(current_measurement_id, 16, false);
    int avg_acc_z = get_measure(current_measurement_id, 17, false);
    int var_acc_z = get_measure(current_measurement_id, 18, false);
    int vector_magnitude_counts = get_measure(current_measurement_id, 19, false);
    int avg_light_level = get_measure(current_measurement_id, 20, false);
    
    APP_LOG(APP_LOG_LEVEL_INFO, "Found light level %d.", avg_light_level);  
    APP_LOG(APP_LOG_LEVEL_INFO, "Found vmc %d.", vector_magnitude_counts);    
    DictionaryIterator *out_iter; // Declare the dictionary's iterator
    app_message_open(64, 512); // open the app message
    AppMessageResult result = app_message_outbox_begin(&out_iter); // prepare the outbox buffer for this message
    if(result == APP_MSG_OK) {
      // go to next dataset
      current_measurement_id++;
      dict_write_int(out_iter, MESSAGE_KEY_current_time, &current_time, sizeof(int), true);
      dict_write_int(out_iter, MESSAGE_KEY_activity, &activity, sizeof(int), true);
      dict_write_int(out_iter, MESSAGE_KEY_avg_heart_rate, &avg_heart_rate, sizeof(int), true);
      dict_write_int(out_iter, MESSAGE_KEY_avg_acc_x, &avg_acc_x, sizeof(int), true);
      dict_write_int(out_iter, MESSAGE_KEY_var_acc_x, &var_acc_x, sizeof(int), true);
      dict_write_int(out_iter, MESSAGE_KEY_avg_acc_y, &avg_acc_y, sizeof(int), true);
      dict_write_int(out_iter, MESSAGE_KEY_var_acc_y, &var_acc_y, sizeof(int), true);
      dict_write_int(out_iter, MESSAGE_KEY_avg_acc_z, &avg_acc_z, sizeof(int), true);
      dict_write_int(out_iter, MESSAGE_KEY_var_acc_z, &var_acc_z, sizeof(int), true);
      dict_write_int(out_iter, MESSAGE_KEY_vmc, &vector_magnitude_counts, sizeof(int), true);
      dict_write_int(out_iter, MESSAGE_KEY_avg_light_level, &avg_light_level, sizeof(int), true);
      wait_for_upload_finish = true;
      result = app_message_outbox_send(); // send this message
      if(result != APP_MSG_OK) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
      }
    } else {
      // the outbox cannot be used right now
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result);
      if(result == APP_MSG_BUSY) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Retry in 2 seconds");
        app_timer_register(2000, upload_measure, NULL);
      }
    }
  } else {
    handled_all_data_items = true;
    APP_LOG(APP_LOG_LEVEL_INFO, "Did not find any further dataset.. Thus, finished processing data (last id was %d)", (current_measurement_id - 1));
    check_whether_upload_process_is_finished();
  }
}

/***********************************
* Executes the upload iteration    *
***********************************/
bool upload_iteration() {
  open_upload_data_task = false;
  int lastId = get_last_measure_id();
  if(lastId == 0) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "There is no data in the storage to upload. Thus, exiting app.");
    handled_all_data_items = true;
    if(window_stack_get_top_window() == upload_window_get_window()) {
      window_stack_pop_all(false);
    }
    
    return false;
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Last inserted id is %d.", lastId);
    current_measurement_id = 1;
    handled_all_data_items = false;
    upload_measure();
    return true;
  }
}

/***********************************
* Message handler for messages sent*
* from the background worker.      *
***********************************/
void worker_message_handler(uint16_t type, AppWorkerMessage *message) {
  if(type == SOURCE_BACKGROUND) {
    int action = message->data0;
    if(action == 4711) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Received the uploading task. Thus, uploading the data saved on storage..");
      if(!is_js_ready()) {
        ping_js();
        open_upload_data_task = true;
      } else {
        upload_iteration();
      }
    }
  }
}

/***********************************
* Initialates the data manager     *
***********************************/
void init_data_manager() {
  // start the background worker
  AppWorkerResult app_worker_result = app_worker_launch();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App worker launched with result %d", app_worker_result);
 
  // subscribe to background worker messages
  app_worker_message_subscribe(worker_message_handler);
}