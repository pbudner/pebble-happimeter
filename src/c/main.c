#include "main.h"

/***********************************
*       Initiates all windows      *
***********************************/
static void init_windows() {
  init_upload_window();
  init_missingconfig_window();
  init_introduction_window();
  init_happiness_input_window();
  init_activation_input_window();
  init_tree_window(); //since we have memory issues, tree window only gets initiated on call.
  init_friends_window();
}

/***********************************
*     Deinitiates all windows      *
***********************************/
static void deinit_windows() {
  deinit_upload_window();
  deinit_missingconfig_window();
  deinit_tree_window();
  deinit_introduction_window();
  deinit_happiness_input_window();
  deinit_activation_input_window();
  deinit_friends_window();
}

/***********************************
*         Initiates the app        *
***********************************/
static void init() {
  init_data_manager();
  init_inbox_message_manager();
  init_windows();
  init_wakeup();

  // show login window if the app is not configured yet
  if(is_configured() == 0) {
   window_stack_push(missingconfig_window_get_window(), true); // show the missing config window
   return;
  }

  // do something based on the launch reason
  if(launch_reason() == APP_LAUNCH_WORKER) {
    window_stack_push(upload_window_get_window(), true);
  } else if(launch_reason() == APP_LAUNCH_WAKEUP) {
    vibes_double_pulse(); // vibrate..
    window_stack_push(introduction_window_get_window(), true); // show the wakeup window
  } else {
    window_stack_push(introduction_window_get_window(), true);
  }
}

/***********************************
*        Deinitiates the app       *
***********************************/
static void deinit() {
  deinit_windows();
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
