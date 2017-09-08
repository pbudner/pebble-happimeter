#include "inbox_message_manager.h"

static void set_upload_mode(bool live) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Set upload mode to %d...", live);
  if(live) {
    AppWorkerMessage message = {
      .data0 = 1888, // this is the code that says --> set to live mode
    };
    app_worker_send_message(SOURCE_FOREGROUND, &message);
  } else {
    AppWorkerMessage message = {
      .data0 = 1889, // this is the code that says --> set to hourly mode
    };
    app_worker_send_message(SOURCE_FOREGROUND, &message);
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "Sent mode change to background worker...");
}  

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
    return;
  }
  Tuple *generic_question_count_tuple = dict_find(iter, MESSAGE_KEY_generic_question_count);
  Tuple *happiness_tuple = dict_find(iter, MESSAGE_KEY_pleasant);
  Tuple *activation_tuple = dict_find(iter, MESSAGE_KEY_activation);
  Tuple *mail_tuple = dict_find(iter, MESSAGE_KEY_friend_mail);
  Tuple *name_tuple = dict_find(iter, MESSAGE_KEY_friend_name);
  if (mail_tuple)
  {
    int32_t happiness = happiness_tuple->value->int32;
    int32_t activation = activation_tuple->value->int32;
    char *name = name_tuple->value->cstring;
    char *mail = mail_tuple->value->cstring;
    APP_LOG(APP_LOG_LEVEL_INFO, "Received friend %s: Act.: %d Hap.: %d", mail, (int)activation, (int)happiness);
    add_friend(mail, name, happiness, activation);
    return;
  }
  
  // Mood Message
  if (happiness_tuple && activation_tuple)
  {
    if(window_stack_get_top_window() == introduction_window_get_window()) {
      int32_t happiness = happiness_tuple->value->int32;
      int32_t activation = activation_tuple->value->int32;
      APP_LOG(APP_LOG_LEVEL_INFO, "Received mood Act.: %d Hap.: %d", (int)activation, (int)happiness);
      set_mood_window_text(happiness, activation);
    }
    return;
  }
  if (generic_question_count_tuple) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Received questions %d", generic_question_count_tuple->value->int32);
    if(window_stack_get_top_window() == introduction_window_get_window()) {
      int32_t generic_question_count = generic_question_count_tuple->value->int32;
      setNumberOfGenericQuestions(generic_question_count);
      Tuple *generic_question_1 = dict_find(iter, MESSAGE_KEY_generic_question_desciption_1);
      Tuple *generic_question_2 = dict_find(iter, MESSAGE_KEY_generic_question_desciption_2);
      Tuple *generic_question_3 = dict_find(iter, MESSAGE_KEY_generic_question_desciption_3);
      Tuple *generic_question_4 = dict_find(iter, MESSAGE_KEY_generic_question_desciption_4);
      Tuple *generic_question_5 = dict_find(iter, MESSAGE_KEY_generic_question_desciption_5);
      
      if (generic_question_1) {
        APP_LOG(APP_LOG_LEVEL_INFO, "Received question1 %s", generic_question_1->value->cstring);
        setGenericDescription(0, generic_question_1->value->cstring);
      }
      if (generic_question_2) {
        setGenericDescription(1, generic_question_2->value->cstring);
      }
      if (generic_question_3) {
        setGenericDescription(2, generic_question_3->value->cstring);
      }
      if (generic_question_4) {
        setGenericDescription(3, generic_question_4->value->cstring);
      }
      if (generic_question_5) {
        setGenericDescription(4, generic_question_5->value->cstring);
      }
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
      vibes_double_pulse(); // vibrate..
      window_stack_pop_all(true);
    }
    return;
  }

  // Logged out Message
  Tuple *loggedout_t = dict_find(iter, MESSAGE_KEY_loggedout);
  if (loggedout_t)
  {
    set_is_not_configured(); // mark current device as logged in!
    APP_LOG(APP_LOG_LEVEL_DEBUG, "User has been logged out!");
    vibes_double_pulse();                                        // vibrate..
    window_stack_pop_all(true);                                  // pop all other windows
    window_stack_push(missingconfig_window_get_window(), false); // push the missing config window from the stack
  }
  
  // Live Mode vs. Hourly Mode
  Tuple *live_mode_t = dict_find(iter, MESSAGE_KEY_live_mode);
  if (live_mode_t)
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "(Pebble) Enable live mode..");
    set_upload_mode(true);
    vibes_double_pulse(); // vibrate..
  }
  
  Tuple *hourly_mode_t = dict_find(iter, MESSAGE_KEY_hourly_mode);
  if (hourly_mode_t)
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "(Pebble) Disable live mode..");
    set_upload_mode(false);
    vibes_short_pulse(); // vibrate..
  }
  
  // Generic Questions Settings
  Tuple *show_generic_questions_mode_t = dict_find(iter, MESSAGE_KEY_show_general_questions);
  if (show_generic_questions_mode_t)
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "(Pebble) Show generic questions mode..");
    persist_write_int(SHOW_GENERIC_QUESTIONS_MODE_STORAGE_KEY, 1);
    vibes_double_pulse(); // vibrate..
  }
  
  Tuple *hide_generic_questions_mode_t = dict_find(iter, MESSAGE_KEY_hide_general_questions);
  if (hide_generic_questions_mode_t)
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "(Pebble) Hide generic questions mode..");
    persist_delete(SHOW_GENERIC_QUESTIONS_MODE_STORAGE_KEY);
    vibes_short_pulse(); // vibrate..
  }
  
  // JS is ready
  Tuple *js_ready_t = dict_find(iter, MESSAGE_KEY_js_ready);
  if(js_ready_t) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "(Pebble) Received a JS is ready message..");
    mark_js_ready();
    if(is_open_upload_task()) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "(Pebble) An pending upload task was open. Sending sensor data..");
      upload_iteration();
    } else {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "(Pebble) Requesting the mood..");
      request_mood();
      //request_generic_questions();
    }
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
  app_message_open(256, 128);
  app_message_register_inbox_received(app_message_inbox_received_callback);
}
