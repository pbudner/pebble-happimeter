#include "data_collection_worker.h"


/***********************************
* Called every minute to collect   *
* measurement data and to start    *
* the upload routine               *
***********************************/
static void updates_tick_handler (struct tm *tick_time, TimeUnits changed) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Starting entire worker update tick..");
  measurement_iteration(); // start measurement iteration
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Finished measurement iteration.. Starting check for upload task...");
  check_for_upload_task(); // check whether measured has to be uploaded
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Finished entire worker update tick..");
}

/***********************************
* Init the background worker       *
***********************************/
static void init() {
  if(persist_exists(LIVE_MODE_STORAGE_KEY)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "(Pebble BW) Live mode is enabled..");
    UPLOAD_INTERVAL_IN_MINUTES = 2;
    UPDATE_INTERVAL_IN_MINUTES = 2;
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "(Pebble BW) Live mode is disabled..");
    UPLOAD_INTERVAL_IN_MINUTES = 60;
    UPDATE_INTERVAL_IN_MINUTES = 15;
  }
  
  init_measurement();
  init_inbox_message_manager();
  tick_timer_service_subscribe(MINUTE_UNIT, updates_tick_handler);
}

/***********************************
* Deinit the background worker     *
***********************************/
static void deinit() {
  tick_timer_service_unsubscribe();
  deinit_measurement();
}

/***********************************
* Main of the background worker    *
***********************************/
int main(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Started worker..");
  init();
  worker_event_loop();
  deinit();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Exit worker..");
}
