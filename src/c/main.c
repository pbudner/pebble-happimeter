#include "main.h"

/***********************************
*       Initiates all windows      *
***********************************/
static void init_windows() {
  init_upload_window();
  init_exit_window();
  init_splash_window();
  init_wakeup_window();
  init_firstquestion_window();
  init_secondquestionA_window();
  init_secondquestionB_window();
  init_secondquestionC_window();
  init_secondquestionD_window();
  init_missingconfig_window();
  init_tree_window();
}

/***********************************
*     Deinitiates all windows      *
***********************************/
static void deinit_windows() {
  deinit_upload_window();
  deinit_exit_window();
  deinit_splash_window();
  deinit_wakeup_window();
  deinit_firstquestion_window();
  deinit_secondquestionA_window();
  deinit_secondquestionB_window();
  deinit_secondquestionC_window();
  deinit_secondquestionD_window();
  deinit_missingconfig_window();
  deinit_tree_window();
}

/***********************************
*         Initiates the app        *
***********************************/
static void init() {
  init_data_manager();
  init_inbox_message_manager();
  init_windows();

  // show enter config window if the app is not configured yet
  //if(is_configured() == 0) {
  //  window_stack_push(missingconfig_window_get_window(), true); // show the missing config window
  //  return;
  // }

  // do something based on the launch reason
  if(launch_reason() == APP_LAUNCH_WORKER) {
    window_stack_push(upload_window_get_window(), true);
  } else if(launch_reason() == APP_LAUNCH_WAKEUP) {
    vibes_double_pulse(); // vibrate..
    window_stack_push(wakeup_window_get_window(), true); // show the wakeup window
  } else {
    window_stack_push(firstquestion_window_get_window(), true); // show the main window
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
