#include "tree_window.h"

          

// // dedicated button manager from here ---
// void back_single_click_handler_tree(ClickRecognizerRef recognizer, void *context) {
//   Window *window = (Window *)context;
//   APP_LOG(APP_LOG_LEVEL_INFO, "Back button is pressed on missingconfig_window. Exiting now!");
//   window_stack_pop_all(true);
// }
//
// // dedicated button manager from here ---
// void down_single_click_handler_tree(ClickRecognizerRef recognizer, void *context) {
//   Window *window = (Window *)context;
//   APP_LOG(APP_LOG_LEVEL_INFO, "Back button is pressed on missingconfig_window. Exiting now!");
//   window_stack_pop_all(true);
// }
//
// // dedicated button manager from here ---
// void select_single_click_handler_tree(ClickRecognizerRef recognizer, void *context) {
//   Window *window = (Window *)context;
//   APP_LOG(APP_LOG_LEVEL_INFO, "Back button is pressed on missingconfig_window. Exiting now!");
//   window_stack_pop_all(true);
// }
//
// // dedicated button manager from here ---
// void up_single_click_handler_tree(ClickRecognizerRef recognizer, void *context) {
//   Window *window = (Window *)context;
//   APP_LOG(APP_LOG_LEVEL_INFO, "Back button is pressed on missingconfig_window. Exiting now!");
//   window_stack_pop_all(true);
// }
/***********************************
* Any button single click handler  *
***********************************/

 const int animation_images[] = {
        RESOURCE_ID_IMAGE_TREE_1,
     RESOURCE_ID_IMAGE_TREE_2,
     RESOURCE_ID_IMAGE_TREE_3,
     RESOURCE_ID_IMAGE_TREE_4,
     RESOURCE_ID_IMAGE_TREE_5,
     RESOURCE_ID_IMAGE_TREE_6,
     RESOURCE_ID_IMAGE_TREE_7,
     RESOURCE_ID_IMAGE_TREE_8,
     RESOURCE_ID_IMAGE_TREE_9,
        RESOURCE_ID_IMAGE_TREE_10,
     RESOURCE_ID_IMAGE_TREE_11,
     RESOURCE_ID_IMAGE_TREE_12,
     RESOURCE_ID_IMAGE_TREE_13,
     RESOURCE_ID_IMAGE_TREE_14,
     RESOURCE_ID_IMAGE_TREE_15,
     RESOURCE_ID_IMAGE_TREE_16,
     RESOURCE_ID_IMAGE_TREE_17,
     RESOURCE_ID_IMAGE_TREE_18,
     RESOURCE_ID_IMAGE_TREE_19,
        RESOURCE_ID_IMAGE_TREE_20,
     RESOURCE_ID_IMAGE_TREE_21,
     RESOURCE_ID_IMAGE_TREE_22,
     RESOURCE_ID_IMAGE_TREE_23,
     RESOURCE_ID_IMAGE_TREE_24,
     RESOURCE_ID_IMAGE_TREE_25,
     RESOURCE_ID_IMAGE_TREE_26,
     RESOURCE_ID_IMAGE_TREE_27,
     RESOURCE_ID_IMAGE_TREE_28,
     RESOURCE_ID_IMAGE_TREE_29,
        RESOURCE_ID_IMAGE_TREE_30,
     RESOURCE_ID_IMAGE_TREE_31,
     RESOURCE_ID_IMAGE_TREE_32,
     RESOURCE_ID_IMAGE_TREE_33,
     RESOURCE_ID_IMAGE_TREE_34,
     RESOURCE_ID_IMAGE_TREE_35,
     RESOURCE_ID_IMAGE_TREE_36,
     RESOURCE_ID_IMAGE_TREE_37,
     RESOURCE_ID_IMAGE_TREE_38,
     RESOURCE_ID_IMAGE_TREE_39,
        RESOURCE_ID_IMAGE_TREE_40,
     RESOURCE_ID_IMAGE_TREE_41,
     RESOURCE_ID_IMAGE_TREE_42,
     RESOURCE_ID_IMAGE_TREE_43,
     RESOURCE_ID_IMAGE_TREE_44,
     RESOURCE_ID_IMAGE_TREE_45,
     RESOURCE_ID_IMAGE_TREE_46,
     RESOURCE_ID_IMAGE_TREE_47

 };


void tree_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
  APP_LOG(APP_LOG_LEVEL_INFO, "One button is pressed on tree_window. Exiting now!");
  window_stack_pop_all(true);
}

/***********************************
* CCP of the tree window           *
***********************************/
void tree_click_config_provider(void *context){
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)tree_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)tree_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)tree_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)tree_single_click_handler);
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

  // treeWindow = window_create(); //apper second times
#ifndef PBL_SDK_3
  window_set_fullscreen(treeWindow, true);
#endif

  if (!persist_exists(TREE_KEY))
  {
    counter = 1;
    persist_write_int(TREE_KEY, counter);
    //treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE1);
  }
  else
  {
    counter = persist_read_int(TREE_KEY);
    counter += 1;
    persist_write_int(TREE_KEY, counter);
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
    break;

  case 2:
#if defined(PBL_PLATFORM_BASALT)
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE2_TIME);
#else
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE2_Black_White);
#endif
    break;

  case 3:
#if defined(PBL_PLATFORM_BASALT)
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE3_TIME);
#else
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE3_Black_White);
#endif
    break;

  case 0:
#if defined(PBL_PLATFORM_BASALT)
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE4_TIME);
#else
    treeImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREE4_Black_White);
#endif
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
  window_set_click_config_provider(window, (ClickConfigProvider) tree_click_config_provider);
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
