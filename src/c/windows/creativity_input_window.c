#include "creativity_input_window.h"

static Window *creativityWindow;
static Window *creativityWindows[6];
static bool initializedWindows[6];
static GBitmap *optionImage[6];
static BitmapLayer *optionImageLayer[6];
static TextLayer *heading_text_layer[6];
static ActionBarLayer *s_action_bar_layer[6];
static GBitmap *s_more_bitmap[6], *s_less_bitmap[6], *s_go_bitmap[6];
static int value[6] = {1,1,1,1,1,1};
static int currentIndex = -1;

void reset_generic_question_counter() {
  currentIndex = -1;
}

void reset_value(){
  for(int i = 0; i < 6; i++){
    value[i] = 1; 
  }
}

void refresh_creativity_image() {
  if(optionImage != NULL) {
    gbitmap_destroy(optionImage[currentIndex]);
    optionImage[currentIndex] = NULL;
  }

  switch (value[currentIndex])
  {
    case 0:
      optionImage[currentIndex] = gbitmap_create_with_resource(RESOURCE_ID_generic_0);
      break;
    case 1:
      optionImage[currentIndex] = gbitmap_create_with_resource(RESOURCE_ID_generic_1);
      break;
    case 2:
      optionImage[currentIndex] = gbitmap_create_with_resource(RESOURCE_ID_generic_2);
      break;
    default:
      optionImage[currentIndex] = NULL;
  }

  bitmap_layer_set_bitmap(optionImageLayer[currentIndex], optionImage[currentIndex]);
  layer_mark_dirty(bitmap_layer_get_layer(optionImageLayer[currentIndex]));
}

void creativity_single_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  value[currentIndex]++;

  if(value[currentIndex] > 2) {
    value[currentIndex] = 2;
  }

  refresh_creativity_image();
}

void creativity_single_down_click_handler(ClickRecognizerRef recognizer, void *context) {
  value[currentIndex]--;

  if(value[currentIndex] < 0) {
    value[currentIndex] = 0;
  }

  refresh_creativity_image();
}

void creativity_single_click_handler(ClickRecognizerRef recognizer, void *context){
  setGenericValue(currentIndex, value[currentIndex]);
  window_stack_push(creativity_input_window_get_next_window(), true);
}

void creativity_back_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_pop(true);
  currentIndex = currentIndex - 1;
}

/***********************************
* Right buttons click config       *
***********************************/
void creativity_click_config_provider(void *context){
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)creativity_single_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)creativity_single_down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)creativity_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)creativity_back_click_handler);
}

/***************************
* Refeshing the action bar *
***************************/
void refresh_menu(Window *window){
  if(s_action_bar_layer != NULL){
    action_bar_layer_destroy(s_action_bar_layer[currentIndex]);
    s_action_bar_layer[currentIndex] = NULL;
  }
  s_more_bitmap[currentIndex] = gbitmap_create_with_resource(RESOURCE_ID_more);
  s_less_bitmap[currentIndex] = gbitmap_create_with_resource(RESOURCE_ID_less);
  s_go_bitmap[currentIndex] = gbitmap_create_with_resource(RESOURCE_ID_NEXT);
  s_action_bar_layer[currentIndex] = action_bar_layer_create();
  action_bar_layer_add_to_window(s_action_bar_layer[currentIndex], window);
  action_bar_layer_set_icon(s_action_bar_layer[currentIndex], BUTTON_ID_UP, s_more_bitmap[currentIndex]);
  action_bar_layer_set_icon(s_action_bar_layer[currentIndex], BUTTON_ID_DOWN, s_less_bitmap[currentIndex]);
  action_bar_layer_set_icon(s_action_bar_layer[currentIndex], BUTTON_ID_SELECT, s_go_bitmap[currentIndex]);
  
  window_set_click_config_provider(window, creativity_click_config_provider);
}


/***********************************
* Load event of the window         *
***********************************/
void creativity_window_load(Window *window){

  // init window
  Layer *window_layer = window_get_root_layer(window);
  optionImageLayer[currentIndex] = bitmap_layer_create(GRect(0, 168 - 75, 144 - ACTION_BAR_WIDTH, 70));
  bitmap_layer_set_compositing_mode(optionImageLayer[currentIndex], GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(optionImageLayer[currentIndex]));

  // add heading
  GRect bounds = layer_get_bounds(window_layer);
  const GEdgeInsets label_insets = {.top = 5, .right = ACTION_BAR_WIDTH + 5, .left = 5, .bottom = 5};
  heading_text_layer[currentIndex] = text_layer_create(grect_inset(bounds, label_insets));
  text_layer_set_background_color(heading_text_layer[currentIndex], GColorClear);
  text_layer_set_text_alignment(heading_text_layer[currentIndex], GTextAlignmentCenter);
  text_layer_set_font(heading_text_layer[currentIndex], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  char* tmp = getGenericDescription(currentIndex);
  text_layer_set_text(heading_text_layer[currentIndex], tmp);
  value[currentIndex] = getGenericValue(currentIndex);
  layer_add_child(window_layer, text_layer_get_layer(heading_text_layer[currentIndex]));

  //refresh the menu
  refresh_menu(window);

  // refresh the shown image
  refresh_creativity_image();

}

/***********************************
* Unload event of the window       *
***********************************/
void creativity_window_unload(){
  bitmap_layer_destroy(optionImageLayer[currentIndex]);
  gbitmap_destroy(optionImage[currentIndex]);
  optionImage[currentIndex] = NULL;

}

/***********************************
* Init the window          *
***********************************/
void init_creativity_input_window(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "INIT CREATIVITY WINDOW");
  creativityWindow = window_create();
  window_set_background_color(creativityWindow, GColorWhite);
  window_set_window_handlers(creativityWindow, (WindowHandlers){.load = creativity_window_load, .unload = creativity_window_unload});
}

/***********************************
* Deinit the window         *
***********************************/
void deinit_creativity_input_window(void) {
  window_destroy(creativityWindow);
}

void deinit_creativity_input_windows(void) {
  for (int i = 0; i < getNumberOfGenericQuestions()-1; i++) {
    window_destroy(creativityWindows[i]);
  }
}

/***********************************
* Returns the window handle        *
***********************************/
Window *creativity_input_window_get_window(void) {
  return creativityWindow;
}

Window *creativity_input_window_get_next_window(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "creativity_input_window_get_next_window");
  
    currentIndex = currentIndex + 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "currentIndex is %d", currentIndex);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "getNumberOfGenericQuestions()  is %d", getNumberOfGenericQuestions() );
  APP_LOG(APP_LOG_LEVEL_DEBUG, "!persist_exists(SHOW_GENERIC_QUESTIONS_MODE_STORAGE_KEY)  is %d", !persist_exists(SHOW_GENERIC_QUESTIONS_MODE_STORAGE_KEY));

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Show generic questions is %d", persist_exists(SHOW_GENERIC_QUESTIONS_MODE_STORAGE_KEY));
  if (getNumberOfGenericQuestions() <= currentIndex) {
    return tree_window_get_window();
  }

  if (!initializedWindows[currentIndex]) {
    creativityWindows[currentIndex] =  window_create();
    window_set_background_color(creativityWindows[currentIndex], GColorWhite);
    window_set_window_handlers(creativityWindows[currentIndex], (WindowHandlers){.load = creativity_window_load, .unload = creativity_window_unload});
    initializedWindows[currentIndex] = true;
  }
    return creativityWindows[currentIndex];
}

