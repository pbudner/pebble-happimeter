#include "introduction_window.h"

static Window *introWindow;
static int counter;
static TextLayer *tree_text_layer;
static TextLayer *heading_text_layer;
static GBitmap *smileyImage;
static BitmapLayer *smileyImageLayer;

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
  if(happiness == -1 && activation == -1) {
    // show loading image
  } else if(happiness == -2 && activation == -2) {
    // show that there is no model
  } else {
    // show matching image
  }
  
  smileyImage = gbitmap_create_with_resource(RESOURCE_ID_HappySmiley144x100);
  bitmap_layer_set_bitmap(smileyImageLayer, smileyImage);
  layer_mark_dirty(bitmap_layer_get_layer(smileyImageLayer));
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
  
  // add the smiley image
  smileyImageLayer = bitmap_layer_create(GRect(0, 25, 144, 120));
  bitmap_layer_set_compositing_mode(smileyImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(smileyImageLayer));
  
  // add the heading
  const GEdgeInsets label_insets = {.top = 5, .right = 5, .left = 5, .bottom = 5};
  heading_text_layer = text_layer_create(grect_inset(bounds, label_insets));
  text_layer_set_background_color(heading_text_layer, GColorClear);
  text_layer_set_text_alignment(heading_text_layer, GTextAlignmentCenter);
  text_layer_set_font(heading_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text(heading_text_layer, "Mood Prediction");
  layer_add_child(window_layer, text_layer_get_layer(heading_text_layer));
  
  // add the tree counter
  tree_text_layer = text_layer_create(grect_inset(bounds, label_insets));
  text_layer_set_background_color(tree_text_layer, GColorClear);
  text_layer_set_text_alignment(tree_text_layer, GTextAlignmentCenter);
  text_layer_set_font(tree_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  static char buffer[90] = "";
  counter = persist_exists(TREE_KEY) ? persist_read_int(TREE_KEY) / 4 : 0;
  snprintf(buffer,  sizeof(buffer), "Planted trees: %d", counter);
  text_layer_set_text(tree_text_layer, buffer);
  GRect frame = layer_get_frame(text_layer_get_layer(tree_text_layer));
  GSize content = text_layer_get_content_size(tree_text_layer);
  layer_set_frame(text_layer_get_layer(tree_text_layer), GRect(frame.origin.x, frame.origin.y + (frame.size.h - content.h - 5), frame.size.w, content.h));
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
