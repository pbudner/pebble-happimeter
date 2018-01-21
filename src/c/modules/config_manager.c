#include "config_manager.h"

static bool js_ready = false;

/***********************************
* Mark that JS is ready            *
***********************************/
void mark_js_ready() {
  js_ready = true;
}

bool is_js_ready() {
  return js_ready;
}

/***********************************
* Checks whether user is logged in *
***********************************/
int is_configured() {
  APP_LOG(APP_LOG_LEVEL_INFO, "Checking whether app is configured..");
 /* if(persist_exists(SETTINGS_STORAGE_KEY)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App is configured!");
    return 1;
  }
  
  APP_LOG(APP_LOG_LEVEL_WARNING, "App is not configured..");
  return 0;*/
  
   APP_LOG(APP_LOG_LEVEL_DEBUG, "App is configured!");
  return 1;
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
  for(int i = 0; i < 100; i++) {
    persist_delete(i);
  }
  
  // also clear all the old data..
  for(int i = 1; i <= 10; i++) {
    for(int u = 0; u < 20; u++) {
      persist_delete(i * 100 + u);
    }
  }
  
}