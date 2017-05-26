#include "upload_data_window.h"

static Window *upload_window;
static TextLayer *upload_window_text_layer;
static char *info_message = "Please keep your watch connected via Bluetooth and the official Pebble App opened on your phone. Otherwise we cannot receive your research data.";
static TextLayer *timeLayer;
static GRect textFrame = {.origin = {.x = 0, .y = 10}, .size = {.w = 144, .h = 168}};
static GRect clockFrame = {.origin = {.x = 5, .y = 95}, .size = {.w = 144-10, .h = 168-95}};


/***********************************
* Overwrite default back button    *
***********************************/
void upload_back_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
  window_stack_pop_all(true);
}



/***********************************
* CCP of the tree window           *
***********************************/
void upload_click_config_provider(void *context)
{
  window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)upload_back_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)upload_back_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)upload_back_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)upload_back_single_click_handler);
}
 
/***********************************
* Refresh the clock                *
***********************************/
void handle_minute_tick(struct tm *currentTime, TimeUnits units_changed) {
  static char timeText[] = "00:00";
  strftime(timeText, sizeof(timeText), "%H:%M", currentTime);
  text_layer_set_text(timeLayer, timeText);
}


/***********************************
* Show warning message if the up-  *
* load timed out..                 *
* Reasons: phone not connected,    *
* no internet connection           *
***********************************/
void upload_timeout_callback(void *data) {
  text_layer_set_text(upload_window_text_layer, info_message);
}

/***********************************
* Initiates the upload info window *
***********************************/
void init_upload_window(void) {
  // Create a window and get information about the window
  upload_window = window_create();
  Layer *window_layer = window_get_root_layer(upload_window);
  window_set_background_color(upload_window, GColorBlack);

  // Create a text layer and set the text
  upload_window_text_layer = text_layer_create(textFrame);
  text_layer_set_text(upload_window_text_layer, "Sending Happimeter data..");
  text_layer_set_font(upload_window_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(upload_window_text_layer, GTextAlignmentCenter);
  text_layer_set_text_color(upload_window_text_layer, GColorWhite);
  text_layer_set_background_color(upload_window_text_layer, GColorClear);
  
  // Set the current time layer
  timeLayer = text_layer_create(clockFrame);
  text_layer_set_text_color(timeLayer, GColorWhite);
  text_layer_set_background_color(timeLayer, GColorClear);
  text_layer_set_font(timeLayer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(timeLayer, GTextAlignmentCenter);
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  handle_minute_tick(tick_time, MINUTE_UNIT);
  layer_add_child(window_layer, text_layer_get_layer(timeLayer));

  
  //set config providers
  window_set_click_config_provider(upload_window, upload_click_config_provider);
  
  // Add the text layer to the window
  layer_add_child(window_get_root_layer(upload_window), text_layer_get_layer(upload_window_text_layer));
  layer_add_child(window_get_root_layer(upload_window), text_layer_get_layer(timeLayer));

  // set a timer to show a warning message if the upload takes to long
  // normally the app gets closed as soon as the upload is finished!
  app_timer_register(10000, upload_timeout_callback, NULL);
  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

/***********************************
* Deinit the upload info window    *
***********************************/
void deinit_upload_window(void) {
  text_layer_destroy(upload_window_text_layer); // Destroy the text layer
  window_destroy(upload_window); // Destroy the window
}

/***********************************
* Returns the window handle        *
***********************************/
Window *upload_window_get_window(void) {
  return upload_window;
}
