#include "data_manager.h"

static int current_measurement_id = 0;
static int happinessAnswers[3]; // array for saving the happiness answers

/***********************************
* Sets the happiness answer for a  *
* given question.                  *
***********************************/
void setHappinessAnswer(int key, int value) {
  happinessAnswers[key] = value;
}

/***********************************
* Returns the happiness answer for *
* a given key.                     *
***********************************/
int getHappinessAnswer(int key) {
  return happinessAnswers[key];
}

/***********************************
* Uploads the happiness dataset for*
* a given set of answers.          *
***********************************/
void upload_happiness(int answers[]) {
  DictionaryIterator *out_iter;
  app_message_open(64, 256); // open the app message
  AppMessageResult result = app_message_outbox_begin(&out_iter); // prepare the outbox buffer for this message
  if(result == APP_MSG_OK) {
    dict_write_int(out_iter, MESSAGE_KEY_happiness, &answers[0], sizeof(int), true);
    dict_write_int(out_iter, MESSAGE_KEY_did_any_sports, &answers[1], sizeof(int), true);
    dict_write_int(out_iter, MESSAGE_KEY_who_have_you_been_with, &answers[2], sizeof(int), true);
    result = app_message_outbox_send(); // send this message
    if(result != APP_MSG_OK) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the happiness outbox: %d", (int)result);
    } else {
      APP_LOG(APP_LOG_LEVEL_INFO, "Succesfully sent the happiness outbox: %d", (int)result);
    }
  } else {
    // the outbox cannot be used right now
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the happiness outbox: %d", (int)result);
  }
}

/***********************************
* Checks whether a further dataset *
* exist and sends it to the app js *
***********************************/
void upload_measure() {
  if(get_measure(current_measurement_id, 10, false) != -1) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Found a dataset with the id %d.", current_measurement_id);

    // receive all measurement data and delete them
    int current_time = get_measure(current_measurement_id, 10, true);
    int steps = get_measure(current_measurement_id, 11, true);
    int activity = get_measure(current_measurement_id, 12, true);
    int avg_heart_rate = get_measure(current_measurement_id, 13, true);
    int avg_acc_x = get_measure(current_measurement_id, 14, true);
    int var_acc_x = get_measure(current_measurement_id, 15, true);
    int avg_acc_y = get_measure(current_measurement_id, 16, true);
    int var_acc_y = get_measure(current_measurement_id, 17, true);
    int avg_acc_z = get_measure(current_measurement_id, 18, true);
    int var_acc_z = get_measure(current_measurement_id, 19, true);
    int vector_magnitude_counts = get_measure(current_measurement_id, 20, true);
    int avg_light_level = get_measure(current_measurement_id, 21, true);
    
    APP_LOG(APP_LOG_LEVEL_INFO, "Found number of steps %d.", steps);
    APP_LOG(APP_LOG_LEVEL_INFO, "Found light level %d.", avg_light_level);  
    APP_LOG(APP_LOG_LEVEL_INFO, "Found vmc %d.", vector_magnitude_counts);    
    DictionaryIterator *out_iter; // Declare the dictionary's iterator
    app_message_open(64, 256); // open the app message
    AppMessageResult result = app_message_outbox_begin(&out_iter); // prepare the outbox buffer for this message
    if(result == APP_MSG_OK) {
      dict_write_int(out_iter, MESSAGE_KEY_current_time, &current_time, sizeof(int), true);
      dict_write_int(out_iter, MESSAGE_KEY_steps, &steps, sizeof(int), true);
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
      result = app_message_outbox_send(); // send this message
      if(result != APP_MSG_OK) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
      }
    } else {
      // the outbox cannot be used right now
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result);
    }

    // go to next dataset
    current_measurement_id++;
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Did not find any further dataset.. Thus, finished processing data (last id was %d)", (current_measurement_id - 1));
  }
}

/***********************************
* When finished with sending data- *
* set try to send another one.     *
***********************************/
void outbox_sent_handler(DictionaryIterator *iter, void *context) {
  upload_measure();
}

/***********************************
* Executes the upload iteration    *
***********************************/
bool upload_iteration() {
  int lastId = get_last_measure_id();
  if(lastId == 0) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "There is no data in the storage to upload!!");
    return false;
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Last inserted id is %d.", lastId);
    current_measurement_id = 1;
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
      app_message_register_outbox_sent(outbox_sent_handler);
      upload_iteration();
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
