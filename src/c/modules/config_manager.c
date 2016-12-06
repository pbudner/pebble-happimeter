#include "config_manager.h"

/***********************************
* Checks whether user is logged in *
***********************************/
int is_configured() {
  if(persist_exists(SETTINGS_STORAGE_KEY)) {
    return 1;
  }
  
  return 0;
}

/***********************************
* Marks that the user is logged in *
***********************************/
void set_is_configured(void) {
  persist_write_int(SETTINGS_STORAGE_KEY, 1);
}