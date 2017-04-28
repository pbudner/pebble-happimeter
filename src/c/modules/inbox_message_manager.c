#include "inbox_message_manager.h"

/***********************************
* Proccesses recevied app messages *
* from JavaScript. Either from up- *
* loading or from the config page. *
***********************************/
static void app_message_inbox_received_callback(DictionaryIterator *iter, void *context)
{
  // App Callback Message
  Tuple *app_callback_tuple = dict_find(iter, MESSAGE_KEY_app_callback);
  if (app_callback_tuple)
  {
    int32_t callback = app_callback_tuple->value->int32;
    APP_LOG(APP_LOG_LEVEL_INFO, "Received an app callback with code %d", (int)callback);
    if (callback == 100)
    {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Callback is finished with upload..");
      received_finished_upload();
    }
    else
    {
      APP_LOG(APP_LOG_LEVEL_INFO, "But we are not finished uploading..");
    }
  }
  
  // Mood Message
  Tuple *happiness_tuple = dict_find(iter, MESSAGE_KEY_pleasant);
  Tuple *activation_tuple = dict_find(iter, MESSAGE_KEY_activation);
  if (happiness_tuple && activation_tuple)
  {
    if(window_stack_get_top_window() == introduction_window_get_window()) {
      int32_t happiness = happiness_tuple->value->int32;
      int32_t activation = activation_tuple->value->int32;
      APP_LOG(APP_LOG_LEVEL_INFO, "Received mood Act.: %d Hap.: %d", (int)activation, (int)happiness);
      set_mood_window_text(happiness, activation);
    }
  }

  // Logged in Message
  Tuple *loggedin_t = dict_find(iter, MESSAGE_KEY_loggedin);
  if (loggedin_t)
  {
    set_is_configured(); // mark current device as logged in!
    APP_LOG(APP_LOG_LEVEL_DEBUG, "User is logged in now!");

    // if currently the app is blocked, go to main window
    if (window_stack_get_top_window() == missingconfig_window_get_window())
    {
      vibes_double_pulse();                                          // vibrate..
      window_stack_push(introduction_window_get_window(), true);    // show main window
      window_stack_remove(missingconfig_window_get_window(), false); // remove the missing config window from the stack
    }
  }

  // Logged out Message
  Tuple *loggedout_t = dict_find(iter, MESSAGE_KEY_loggedout);
  if (loggedout_t)
  {
    set_is_not_configured(); // mark current device as logged in!
    APP_LOG(APP_LOG_LEVEL_DEBUG, "User has been logged out!");
    vibes_double_pulse();                                        // vibrate..
    window_stack_pop_all(true);                                  // pop all other windows
    window_stack_push(missingconfig_window_get_window(), false); // remove the missing config window from the stack
  }
}

/***********************************
* Deinitializes the inbox messsage *
* controller.                      *
***********************************/
void deinit_inbox_message_manager()
{
  app_message_deregister_callbacks();
}

/***********************************
* Initializes the inbox messages   *
* controller.                      *
***********************************/
void init_inbox_message_manager()
{
  app_message_open(128, 128);
  app_message_register_inbox_received(app_message_inbox_received_callback);
}
