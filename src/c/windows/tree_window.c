#include "tree_window.h"

static Window *treeWindow;
static GBitmap *treeImage;
static BitmapLayer *treeImageLayer;
static uint32_t key;
static uint32_t counter;

// dedicated button manager from here ---
void back_single_click_handler_tree(ClickRecognizerRef recognizer, void *context) {
  Window *window = (Window *)context;
  APP_LOG(APP_LOG_LEVEL_INFO, "Back button is pressed on missingconfig_window. Exiting now!");
  window_stack_pop_all(true);
}

// dedicated button manager from here ---
void down_single_click_handler_tree(ClickRecognizerRef recognizer, void *context) {
  Window *window = (Window *)context;
  APP_LOG(APP_LOG_LEVEL_INFO, "Back button is pressed on missingconfig_window. Exiting now!");
  window_stack_pop_all(true);
}

// dedicated button manager from here ---
void select_single_click_handler_tree(ClickRecognizerRef recognizer, void *context) {
  Window *window = (Window *)context;
  APP_LOG(APP_LOG_LEVEL_INFO, "Back button is pressed on missingconfig_window. Exiting now!");
  window_stack_pop_all(true);
}

// dedicated button manager from here ---
void up_single_click_handler_tree(ClickRecognizerRef recognizer, void *context) {
  Window *window = (Window *)context;
  APP_LOG(APP_LOG_LEVEL_INFO, "Back button is pressed on missingconfig_window. Exiting now!");
  window_stack_pop_all(true);
}
void config_provider_tree(Window *window) {
  window_single_click_subscribe(BUTTON_ID_BACK, back_single_click_handler_tree);  
   window_single_click_subscribe(BUTTON_ID_SELECT, down_single_click_handler_tree);  
   window_single_click_subscribe(BUTTON_ID_DOWN, up_single_click_handler_tree);  
   window_single_click_subscribe(BUTTON_ID_UP, select_single_click_handler_tree);  
}

/***********************************
* Load event of the tree window    *
***********************************/
void tree_window_load(Window *window)
{
  int _activation, _pleasant;
  _activation = getActivation();
  _pleasant = getPleasant();
  
  APP_LOG(APP_LOG_LEVEL_INFO, "Pleasant value is %d", _pleasant);
  APP_LOG(APP_LOG_LEVEL_INFO, "Activation value is %d", _activation);
  upload_mood(_pleasant, _activation);
  Layer *window_layer = window_get_root_layer(window);
  APP_LOG(APP_LOG_LEVEL_INFO, "%ld", persist_read_int(counter));

  treeWindow = window_create();
#ifndef PBL_SDK_3
  window_set_fullscreen(treeWindow, true);
#endif

  if (!persist_exists(key))
  {
    key = 0;
    counter = 1;
    persist_write_int(key, counter);
    //treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE1);
  }
  else
  {
    counter = persist_read_int(key);
    counter = counter % 4;
  }

  switch (counter)

  {
  case 1:
#if defined(PBL_PLATFORM_BASALT)
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE1_TIME);
#else
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE1_Black_White);
#endif
    counter = counter + 1;
    persist_write_int(key, counter);
    break;

  case 2:
#if defined(PBL_PLATFORM_BASALT)
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE2_TIME);
#else
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE2_Black_White);
#endif
    counter = counter + 1;
    persist_write_int(key, counter);
    break;

  case 3:
#if defined(PBL_PLATFORM_BASALT)
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE3_TIME);
#else
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE3_Black_White);
#endif
    counter = counter + 1;
    persist_write_int(key, counter);
    break;

  case 0:
#if defined(PBL_PLATFORM_BASALT)
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE4_TIME);
#else
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE4_Black_White);
#endif
    counter = counter + 1;
    persist_write_int(key, counter);
    break;
  }

  // Loads a png Image from ressources
  #if defined(PBL_PLATFORM_EMERY)
  treeImageLayer = bitmap_layer_create(GRect(0, 0, 200, 228));
  #else
  treeImageLayer = bitmap_layer_create(GRect(0, 0, 144, 168));
  #endif
  bitmap_layer_set_bitmap(treeImageLayer, treeImage);
  bitmap_layer_set_compositing_mode(treeImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(treeImageLayer));
  
  // override back_button_manager
  window_set_click_config_provider(window, (ClickConfigProvider) config_provider_tree);
}

/***********************************
* Unload event of the tree window  *
***********************************/
void tree_window_unload(Window *window)
{
  gbitmap_destroy(treeImage);
  bitmap_layer_destroy(treeImageLayer);
}

/***********************************
* Initiates the tree window        *
***********************************/
void init_tree_window()
{
  if (!treeWindow){
    treeWindow = window_create();
    window_set_window_handlers(treeWindow, (WindowHandlers) { .load = tree_window_load, .unload = tree_window_unload });
  }
}

/***********************************
* Deinit the tree window           *
***********************************/
void deinit_tree_window()
{
  window_destroy(treeWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *tree_window_get_window()
{
  init_tree_window();
  return treeWindow;
}