#include "main.h"

/***********************************
*       Initiates all windows      *
***********************************/
static void init_windows() {
  init_upload_window();
}

/***********************************
*         Initiates the app        *
***********************************/
static void init() {
  init_data_manager();
  init_inbox_message_manager();
  init_windows();

  // show enter config window if the app is not configured yet
  if(!is_configured()) {
    // TODO: show the window
    return;
  }

  // do something based on the launch reason
  if(launch_reason() == APP_LAUNCH_WORKER) {
    window_stack_push(upload_window_get_window(), true);
  } else if(launch_reason() == APP_LAUNCH_WAKEUP) {
    vibes_double_pulse(); // vibrate..
    //window_stack_push(wakeup_window_get_window(), true); // show the wakeup window
  } else {
    //window_stack_push(default_window_get_window(), true); // show the default window
  }
}

/***********************************
*        Deinitiates the app       *
***********************************/
static void deinit() {
  deinit_upload_window();
  deinit_inbox_message_manager();
}

/***********************************
*          The main method         *
***********************************/
int main(void) {
  init();
  app_event_loop();
  deinit();
}
