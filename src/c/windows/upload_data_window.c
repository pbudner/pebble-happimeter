#include "upload_data_window.h"

static Window *upload_window;
static TextLayer *upload_window_text_layer;
static char *info_message = "Please keep your Watch connected and the Pebble App opened on your Phone! Otherwise we cannot receive your research data..";


/***********************************
* Overwrite default back button    *
***********************************/
void upload_back_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
    Window *window = (Window *)context;
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
  GRect bounds = layer_get_bounds(window_layer);

  // Create a text layer and set the text
  upload_window_text_layer = text_layer_create(bounds);
  text_layer_set_text(upload_window_text_layer, "Uploading Happimeter data..");

  // Set the font and text alignment
  text_layer_set_font(upload_window_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(upload_window_text_layer, GTextAlignmentCenter);
  
    //set config providers
  window_set_click_config_provider(upload_window, upload_click_config_provider);
  
  // Add the text layer to the window
  layer_add_child(window_get_root_layer(upload_window), text_layer_get_layer(upload_window_text_layer));

  // Enable text flow and paging on the text layer, with a slight inset of 10, for round screens
  text_layer_enable_screen_text_flow_and_paging(upload_window_text_layer, 10);

  // set a timer to show a warning message if the upload takes to long
  // normally the app gets closed as soon as the upload is finished!
  app_timer_register(10000, upload_timeout_callback, NULL);
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
