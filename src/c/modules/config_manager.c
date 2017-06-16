#include "config_manager.h"

/***********************************
* Checks whether user is logged in *
***********************************/
int is_configured() {
  APP_LOG(APP_LOG_LEVEL_INFO, "Checking whether app is configured..");
  if(persist_exists(SETTINGS_STORAGE_KEY)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App is configured!");
    return 1;
  }
  
  APP_LOG(APP_LOG_LEVEL_WARNING, "App is not configured..");
  return 0;
}

/***********************************
* Marks that the user is logged in *
***********************************/
void set_is_configured(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Marked app as configured %d.", persist_write_int(SETTINGS_STORAGE_KEY, 1));
}

/***********************************
* Marks the user as not logged in  *
***********************************/
void set_is_not_configured(void) {
  persist_delete(SETTINGS_STORAGE_KEY);
  
  // also clear all the old data..
  for(int i = 0; i < 100000000; i++) {
    persist_delete(i);
  }
}