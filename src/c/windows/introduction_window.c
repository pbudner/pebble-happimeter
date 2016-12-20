#include "introduction_window.h"

static Window *introWindow;
static uint32_t counter;
static TextLayer *treenum_label_layer;
// static GBitmap *introImage;
// static BitmapLayer *introImageLayer;

void intro_single_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_push(firstquestion_window_get_window(), true); // show the main window
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
* Load event of the window         *
***********************************/
void introduction_window_load(Window *window){

  Layer *window_layer = window_get_root_layer(window);
  static char buf_tree[99] = "Hello, You have planted 999999 tree(s).\nLet's finish some questions to plant more. :)";

  #ifndef PBL_SDK_3
    window_set_fullscreen(window, true);
  #endif
  /**
  * It may should be changed to get the tree number from website.
  */
  counter = persist_exists(TREE_KEY) ? persist_read_int(TREE_KEY) / 4 : 0;
  if (counter < 1){
    snprintf(buf_tree,  sizeof(buf_tree), "Hello, You haven't planted any tree.\nYou can get one after finishing four question-sets. :)");
  } else{
    snprintf(buf_tree,  sizeof(buf_tree), "Hello, You have planted %ld tree(s).\nLet's go for a new one. :)", counter);
  }

  APP_LOG(APP_LOG_LEVEL_INFO, buf_tree);

  GRect bounds = layer_get_bounds(window_layer);
  const GEdgeInsets label_insets = {.top = 10, .right = 0, .left = 5};
  treenum_label_layer = text_layer_create(grect_inset(bounds, label_insets));
  text_layer_set_text(treenum_label_layer, buf_tree);
  text_layer_set_background_color(treenum_label_layer, GColorClear);
  text_layer_set_text_alignment(treenum_label_layer, GTextAlignmentLeft);
  #if defined(PBL_PLATFORM_EMERY)
    text_layer_set_font(treenum_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  #else
    text_layer_set_font(treenum_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  #endif
  layer_add_child(window_layer, text_layer_get_layer(treenum_label_layer));


  //set config providers
  window_set_click_config_provider(window, intro_click_config_provider);
  // overwrite default setting for back button
  // force_back_button(window, firstquestionMenuLayer);
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
  text_layer_destroy(treenum_label_layer);
  window_destroy(introWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *introduction_window_get_window(void) {
  return introWindow;
}
