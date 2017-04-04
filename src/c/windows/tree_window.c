#include "tree_window.h"

static Window *treeWindow;
static GBitmap *treeImage = NULL;
static BitmapLayer *treeImageLayer;
static uint32_t counter = 1;

int       frame_no1, 
          frame_no2, 
          frame_no3,
          frame_no4;
const int NO_OF_FRAMES1 = 16, 
          NO_OF_FRAMES2 = 15, 
          NO_OF_FRAMES3 = 49, 
          NO_OF_FRAMES4 = 49; 

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
 const int animation_images_1[] = {
      RESOURCE_ID_Tree1_1,
      RESOURCE_ID_Tree1_2,
      RESOURCE_ID_Tree1_3,
      RESOURCE_ID_Tree1_4,
      RESOURCE_ID_Tree1_5,
      RESOURCE_ID_Tree1_6,
      RESOURCE_ID_Tree1_7,
      RESOURCE_ID_Tree1_8,
      RESOURCE_ID_Tree1_9,
      RESOURCE_ID_Tree1_10,
      RESOURCE_ID_Tree1_11,
      RESOURCE_ID_Tree1_12,
      RESOURCE_ID_Tree1_13,
      RESOURCE_ID_Tree1_14,
      RESOURCE_ID_Tree1_15,
      RESOURCE_ID_Tree1_16
 };

const int animation_images_2[] = {
      RESOURCE_ID_Tree2_1,
      RESOURCE_ID_Tree2_2,
      RESOURCE_ID_Tree2_3,
      RESOURCE_ID_Tree2_4,
      RESOURCE_ID_Tree2_5,
      RESOURCE_ID_Tree2_6,
      RESOURCE_ID_Tree2_7,
      RESOURCE_ID_Tree2_8,
      RESOURCE_ID_Tree2_9,
      RESOURCE_ID_Tree2_10,
      RESOURCE_ID_Tree2_11,
      RESOURCE_ID_Tree2_12,
      RESOURCE_ID_Tree2_13,
      RESOURCE_ID_Tree2_14,
      RESOURCE_ID_Tree2_15
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


static void load_sequence();

static void timer_handler(void *context){
  if(frame_no1 == NO_OF_FRAMES1){
  frame_no1= 0;
  }{

    
  if(treeImage !=NULL){
    gbitmap_destroy(treeImage);
    treeImage = NULL;
  }
  
    switch (counter)
  {
  case 1:
    treeImage = gbitmap_create_with_resource(animation_images_1[frame_no1]);
    break;

  case 2:
    treeImage = gbitmap_create_with_resource(animation_images_2[frame_no1]);
    break;

  case 3:
    treeImage = gbitmap_create_with_resource(animation_images_1[frame_no1]);
    break;

  case 0:
    treeImage = gbitmap_create_with_resource(animation_images_1[frame_no1]);
    break;
  }
  
  bitmap_layer_set_bitmap(treeImageLayer, treeImage);
  layer_mark_dirty(bitmap_layer_get_layer(treeImageLayer));
  frame_no1++;
  app_timer_register(80, timer_handler, NULL);
  
  } 
  
}

static void load_sequence() {
  frame_no1 = 0;
  app_timer_register(1, timer_handler, NULL);
}

/***********************************
* Load event of the tree window    *
***********************************/
void tree_window_load(Window *window)
{
  
  
  int _activation, _pleasant;
  _activation = getActivation();
  _pleasant = getPleasant();

  
  upload_mood(_pleasant, _activation);
  Layer *window_layer = window_get_root_layer(window);
     GRect bounds = layer_get_bounds(window_layer);

#ifndef PBL_SDK_3
  window_set_fullscreen(treeWindow, true);
#endif

  if (!persist_exists(TREE_KEY))
  {
    counter = 1;
    persist_write_int(TREE_KEY, counter);
  }
  else
  {
    counter = persist_read_int(TREE_KEY);
    counter += 1;
    persist_write_int(TREE_KEY, counter);
    counter = counter % 4;
  }



  // Loads a png Image from ressources
  treeImageLayer = bitmap_layer_create(bounds);

  //bitmap_layer_set_bitmap(treeImageLayer, treeImage);
  //bitmap_layer_set_compositing_mode(treeImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(treeImageLayer));
  load_sequence();

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
