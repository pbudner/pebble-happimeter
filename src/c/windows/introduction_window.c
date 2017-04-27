#include "introduction_window.h"

static Window *introWindow;
static int counter;
static TextLayer *tree_text_layer;
// static GBitmap *introImage;
// static BitmapLayer *introImageLayer;

void intro_single_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_push(smileymatrix_window_get_window(), true); // show the main window
}
void intro_back_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_pop_all(true);
}
/***********************************
* Right buttons click config       *
***********************************/
void intro_click_config_provider(void *context){
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)intro_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)intro_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)intro_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)intro_back_click_handler);
}

/***********************************
* Sets the text of the intro window*
***********************************/
void set_mood_window_text(int happiness, int activation) {
  static char buffer[999] = "";
  counter = persist_exists(TREE_KEY) ? persist_read_int(TREE_KEY) / 4 : 0;
  snprintf(buffer,  sizeof(buffer), "Prediction of your Activation: %d \nPrediction of your Happiness: %d \nNumber of planted trees: %d", activation, happiness, counter);
  APP_LOG(APP_LOG_LEVEL_INFO, buffer);
  text_layer_set_text(tree_text_layer, buffer);
}

/***********************************
* Load event of the window         *
***********************************/
void introduction_window_load(Window *window){

  Layer *window_layer = window_get_root_layer(window);

  #ifndef PBL_SDK_3
    window_set_fullscreen(window, true);
  #endif

  GRect bounds = layer_get_bounds(window_layer);
  const GEdgeInsets label_insets = {.top = 10, .right = 0, .left = 5};
  tree_text_layer = text_layer_create(grect_inset(bounds, label_insets));
  text_layer_set_background_color(tree_text_layer, GColorClear);
  text_layer_set_text_alignment(tree_text_layer, GTextAlignmentLeft);
  #if defined(PBL_PLATFORM_EMERY)
    text_layer_set_font(tree_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  #else
    text_layer_set_font(tree_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  #endif
  layer_add_child(window_layer, text_layer_get_layer(tree_text_layer));

  //set config providers
  window_set_click_config_provider(window, intro_click_config_provider);
  
  set_mood_window_text(-1, -1); // -1 should mean still loading..
}

/***********************************
* Unload event of the window       *
***********************************/
void introduction_window_unload(){
  // bitmap_layer_destroy(introImageLayer);
  // gbitmap_destroy(introImage);
}

/***********************************
* Init the window          *
***********************************/
void init_introduction_window(void) {
  introWindow = window_create();
  window_set_background_color(introWindow, GColorWhite);
  window_set_window_handlers(introWindow, (WindowHandlers){.load = introduction_window_load, .unload = introduction_window_unload});
}

/***********************************
* Deinit the window         *
***********************************/
void deinit_introduction_window(void) {
  text_layer_destroy(tree_text_layer);
  window_destroy(introWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *introduction_window_get_window(void) {
  return introWindow;
}
