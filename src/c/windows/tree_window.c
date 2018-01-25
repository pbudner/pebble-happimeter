#include "tree_window.h"

static Window *treeWindow;
static GBitmap *treeImage = NULL;
static BitmapLayer *treeImageLayer;
static TextLayer *tree_text_layer;
static uint32_t counter = 1;
static int tree_counter;
static bool hasBtC;

int       frame_no1, 
          frame_no2, 
          frame_no3,
          frame_no4;
const int NO_OF_FRAMES1 = 16, 
          NO_OF_FRAMES2 = 15, 
          NO_OF_FRAMES3 = 14, 
          NO_OF_FRAMES4 = 13; 
const int TREE_KEY = 0;

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

const int animation_images_3[] = {
      RESOURCE_ID_Tree3_1,
      RESOURCE_ID_Tree3_2,
      RESOURCE_ID_Tree3_3,
      RESOURCE_ID_Tree3_4,
      RESOURCE_ID_Tree3_5,
      RESOURCE_ID_Tree3_6,
      RESOURCE_ID_Tree3_7,
      RESOURCE_ID_Tree3_8,
      RESOURCE_ID_Tree3_9,
      RESOURCE_ID_Tree3_10,
      RESOURCE_ID_Tree3_11,
      RESOURCE_ID_Tree3_12,
      RESOURCE_ID_Tree3_13,
      RESOURCE_ID_Tree3_14
 };

const int animation_images_4[] = {
      RESOURCE_ID_Tree4_1,
      RESOURCE_ID_Tree4_2,
      RESOURCE_ID_Tree4_3,
      RESOURCE_ID_Tree4_4,
      RESOURCE_ID_Tree4_5,
      RESOURCE_ID_Tree4_6,
      RESOURCE_ID_Tree4_7,
      RESOURCE_ID_Tree4_8,
      RESOURCE_ID_Tree4_9,
      RESOURCE_ID_Tree4_10,
      RESOURCE_ID_Tree4_11,
      RESOURCE_ID_Tree4_12,
      RESOURCE_ID_Tree4_13
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
int frame_no =0;
  int NO_OF_FRAMES=0;
  
  if(treeImage !=NULL){
    gbitmap_destroy(treeImage);
    treeImage = NULL;
  }
  
  switch (counter){
    
    case 1:
    frame_no = frame_no1;
    treeImage = gbitmap_create_with_resource(animation_images_1[frame_no]);
    frame_no1++;
    NO_OF_FRAMES = NO_OF_FRAMES1;
    break;
    
        case 2:
    frame_no = frame_no2;
    treeImage = gbitmap_create_with_resource(animation_images_2[frame_no]);
    frame_no2++;
    NO_OF_FRAMES = NO_OF_FRAMES2;
    break;
    
        case 3:
    frame_no = frame_no3;
     treeImage = gbitmap_create_with_resource(animation_images_3[frame_no]);
    frame_no3++;
    NO_OF_FRAMES = NO_OF_FRAMES3;
    break;
    
        case 4:
    frame_no = frame_no4;
    treeImage = gbitmap_create_with_resource(animation_images_4[frame_no]);
    frame_no4++;
    NO_OF_FRAMES = NO_OF_FRAMES4;
    break;
  } 
  if(frame_no == NO_OF_FRAMES){
  frame_no = 0;
    return;
  }

  
  {

  
  bitmap_layer_set_bitmap(treeImageLayer, treeImage);
  layer_mark_dirty(bitmap_layer_get_layer(treeImageLayer));
 
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
  hasBtC = get_hasBtConnection();
  // deinit old windows, such that we have more free ram
  // to show the fancy animations! :-)
  deinit_creativity_input_windows();
  deinit_happiness_input_window();
  deinit_activation_input_window();
  
  int _activation, _pleasant, _creativity;
  uint8_t _genericValues[5];
  _activation = getActivation();
  _pleasant = getPleasant();
  _creativity = getCreativity();
  
  for (int i = 0; i < 5;i++) {
    if (i < getNumberOfGenericQuestions()) {
      _genericValues[i] = getGenericValue(i);  
    } else {
      _genericValues[i] = 99;
    }
  }
  
   if(hasBtC){
    upload_mood(_pleasant, _activation, _creativity,_genericValues);
  } else{
     save_storage_mood(_pleasant, _activation, _creativity,_genericValues);
  }
 
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
    if(counter == 0) {
      counter = 4;
    }
  }



  // Loads a png Image from ressources
  int topMargin = 0;
  if(counter == 1) {
    topMargin = -5;
  }
  
  GEdgeInsets label_insets = {.top = topMargin, .right = 0, .left = 0, .bottom = 15};
  treeImageLayer = bitmap_layer_create(grect_inset(bounds, label_insets));
  
  // add the tree counter
  GEdgeInsets tree_label_insets = {.top = 0, .right = 0, .left = 0, .bottom = 0};
  tree_text_layer = text_layer_create(grect_inset(bounds, tree_label_insets));
  text_layer_set_background_color(tree_text_layer, GColorClear);
  text_layer_set_text_alignment(tree_text_layer, GTextAlignmentCenter);
  text_layer_set_font(tree_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  static char buffer[90] = "";
  tree_counter = persist_exists(TREE_KEY) ? persist_read_int(TREE_KEY) / 4 : 0;
  snprintf(buffer,  sizeof(buffer), "Planted trees: %d", tree_counter);
  text_layer_set_text(tree_text_layer, buffer);
  GRect frame = layer_get_frame(text_layer_get_layer(tree_text_layer));
  GSize content = text_layer_get_content_size(tree_text_layer);
  layer_set_frame(text_layer_get_layer(tree_text_layer), GRect(frame.origin.x, frame.origin.y + (frame.size.h - content.h - 5), frame.size.w, content.h));
  layer_add_child(window_layer, text_layer_get_layer(tree_text_layer));
  
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
