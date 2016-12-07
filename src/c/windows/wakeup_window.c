#include "wakeup_window.h"

static Window *wakeupWindow;
static ActionBarLayer *s_action_layer;
static TextLayer *s_label_layer;
static ActionBarLayer *s_action_bar_layer;
static GBitmap *s_tick_bitmap, *s_cross_bitmap, *s_sleep_bitmap;

/***********************************
* Delays the timer to let the user *
* answer later.                    *
***********************************/
static void delay_timer()
{
  // Let the timestamp be 1 minutes from now
  const time_t future_timestamp = time(NULL) + (10 * SECONDS_PER_MINUTE);

  // Choose a 'cookie' value representing the reason for the wakeup
  const int cookie = 0;

  // If true, the user will be notified if they missed the wakeup
  // (i.e. their watch was off)
  const bool notify_if_missed = true;

  //cancel all wakeups
  wakeup_cancel_all();

  // Schedule wakeup event
  WakeupId id = wakeup_schedule(future_timestamp, cookie, notify_if_missed);

  // Check the scheduling was successful
  if (id >= 0)
  {
    // Persist the ID so that a future launch can query it
    const int wakeup_id_key = 43;
    persist_write_int(wakeup_id_key, id);
  }

  // This will be set by wakeup_query()
  time_t wakeup_timestamp = 0;

  // Is the wakeup still scheduled?
  if (wakeup_query(id, &wakeup_timestamp))
  {
    // Get the time remaining
    int seconds_remaining = wakeup_timestamp - time(NULL);
    APP_LOG(APP_LOG_LEVEL_INFO, "%d seconds until wakeup", seconds_remaining);
  }
}

//void launch_main_window2 (){
//window_stack_push(main_window_get_window(), true);
//}

// A wakeup event has occurred while the app was already open

/***********************************
* Wakeup event occurred while the  *
* was already open                 *
***********************************/
static void wakeup_handler(WakeupId id, int32_t reason)
{
  window_stack_push(wakeupWindow, true);
}

/***********************************
* Up click handler (start)         *
***********************************/
void wakeup_up_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
  window_stack_push(firstquestion_window_get_window(), true);
}

/***********************************
* Down click handler (exit)        *
***********************************/
void wakeup_down_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
  window_stack_pop_all(true);
}

/***********************************
* Back button click handler (exit) *
***********************************/
void wakeup_back_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
  window_stack_pop_all(true);
}

/***********************************
* Select click handler (delay)     *
***********************************/
void wakeup_select_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
  delay_timer();
  window_stack_pop_all(true);
}

/***********************************
* Click config provider            *
***********************************/
void wakeup_click_config_provider(void *context)
{
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)wakeup_up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)wakeup_down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)wakeup_back_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, wakeup_select_single_click_handler);
}

/***********************************
* Wakeup                           *
***********************************/
static void wakeup()
{

  // Next occuring (day/hour/minutes)
  printf("wir sind in der wakeupfunktion");

  time_t timestamp_1 = clock_to_timestamp(TODAY, 9, 00);
  time_t timestamp_2 = clock_to_timestamp(TODAY, 13, 00);
  time_t timestamp_3 = clock_to_timestamp(TODAY, 17, 00);
  time_t timestamp_4 = clock_to_timestamp(TODAY, 21, 00);

  // Choose a 'cookie' value representing the reason for the wakeup
  //const int cookie = 0;

  // If true, the user will be notified if they missed the wakeup
  // (i.e. their watch was off)
  const bool notify_if_missed = true;

  //SUPERWICHTIG, // Cancel all wakeups
  wakeup_cancel_all();

  // Schedule wakeup event
  WakeupId id_1 = wakeup_schedule(timestamp_1, 1, notify_if_missed);
  WakeupId id_2 = wakeup_schedule(timestamp_2, 2, notify_if_missed);
  WakeupId id_3 = wakeup_schedule(timestamp_3, 3, notify_if_missed);
  WakeupId id_4 = wakeup_schedule(timestamp_4, 4, notify_if_missed);

  printf("scheduled!");

  // Check the scheduling was successful
  if (id_1 >= 0 && id_2 >= 0 && id_3 >= 0 && id_4 >= 0)
  {
    // Persist the ID so that a future launch can query it
    printf("id is scheduled!");
    const int wakeup_id_key = 43;
    persist_write_int(wakeup_id_key, id_1);
  }

  // Is the wakeup still scheduled?
  if (wakeup_query(id_1, &timestamp_1))
  {
    // Get the time remaining
    int seconds_remaining = timestamp_1 - time(NULL);
    APP_LOG(APP_LOG_LEVEL_INFO, "%d seconds until wakeup", seconds_remaining);
  }
}

/***********************************
* Load event of the wakeup window  *
***********************************/
static void wakeup_window_load(Window *window)
{

  Layer *window_layer = window_get_root_layer(window);
  window_set_background_color(window, GColorGreen);
  GRect bounds = layer_get_bounds(window_layer);

  const GEdgeInsets label_insets = {.top = 10, .right = ACTION_BAR_WIDTH + ACTION_BAR_WIDTH / 2, .left = ACTION_BAR_WIDTH / 2};
  s_label_layer = text_layer_create(grect_inset(bounds, label_insets));
  text_layer_set_text(s_label_layer, "Time for three little questions?");
  text_layer_set_background_color(s_label_layer, GColorClear);
  text_layer_set_text_alignment(s_label_layer, GTextAlignmentCenter);
  text_layer_set_font(s_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_label_layer));

  s_tick_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TICK);
  s_cross_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CROSS);
  s_sleep_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SLEEP);

  s_action_bar_layer = action_bar_layer_create();
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_tick_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_cross_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_sleep_bitmap);

  action_bar_layer_add_to_window(s_action_bar_layer, window);

  // Set the click config provider:
  action_bar_layer_set_click_config_provider(s_action_bar_layer, wakeup_click_config_provider);
}

/***********************************
* Unload event of the wakeup window*
***********************************/
static void wakeup_window_unload(Window *window)
{
}

/***********************************
* Initiates the wakeup window      *
***********************************/
void init_wakeup_window()
{

  //If the app is launched due to a previously scheduled wakeup event
  if (launch_reason() == APP_LAUNCH_WAKEUP)
  {
    // The app was started by a wakeup event.
    WakeupId id = 0;
    int32_t reason = 0;

    // Get details and handle the event appropriately
    wakeup_get_launch_event(&id, &reason);
  }

  // Subscribe to wakeup service

  wakeup_service_subscribe(wakeup_handler);

  wakeupWindow = window_create();
  window_set_window_handlers(wakeupWindow, (WindowHandlers){ .load = wakeup_window_load, .unload = wakeup_window_unload });

  wakeup();
}

/***********************************
* Deinitiates the wakeup window    *
***********************************/
void deinit_wakeup_window()
{
  window_destroy(wakeupWindow);
  action_bar_layer_destroy(s_action_layer);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *wakeup_window_get_window()
{
  return wakeupWindow;
}