#include "inbox_message_manager.h"

static void worker_message_handler(uint16_t type, AppWorkerMessage *message)
{
  if(type == SOURCE_FOREGROUND) {
    int action = message->data0;
    
    if(action == 1888) {
      APP_LOG(APP_LOG_LEVEL_INFO, "(Pebble BW) Enable live mode..");
      persist_write_int(LIVE_MODE_STORAGE_KEY, 1);
      UPLOAD_INTERVAL_IN_MINUTES = 2;
      UPDATE_INTERVAL_IN_MINUTES = 2;
      return;
    }
    
    if(action == 1889) {
      APP_LOG(APP_LOG_LEVEL_INFO, "(Pebble BW) Enable hourly mode..");
      persist_delete(LIVE_MODE_STORAGE_KEY);
      UPLOAD_INTERVAL_IN_MINUTES = 60;
      UPDATE_INTERVAL_IN_MINUTES = 15;
      return;
    }
  }
}

/***********************************
* Initializes the inbox messages   *
* controller.                      *
***********************************/
void init_inbox_message_manager()
{
  app_worker_message_subscribe(worker_message_handler);
}