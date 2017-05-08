#include "activation_input_window.h"

static Window *activationWindow;
static GBitmap *optionImage;
static BitmapLayer *optionImageLayer;
static TextLayer *heading_text_layer;
static ActionBarLayer *s_action_bar_layer;
static GBitmap *s_more_bitmap, *s_less_bitmap, *s_go_bitmap;
static int activation = 1;

void refresh_activation_image() {
  if(optionImage != NULL) {
    gbitmap_destroy(optionImage);
    optionImage = NULL;
  }

  switch (activation)
  {
    case 0:
      optionImage = gbitmap_create_with_resource(RESOURCE_ID_activation_0);
      break;
    case 1:
      optionImage = gbitmap_create_with_resource(RESOURCE_ID_activation_1);
      break;
    case 2:
      optionImage = gbitmap_create_with_resource(RESOURCE_ID_activation_2);
      break;
    default:
      optionImage = NULL;
  }

  bitmap_layer_set_bitmap(optionImageLayer, optionImage);
  layer_mark_dirty(bitmap_layer_get_layer(optionImageLayer));

}

void activation_single_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  activation++;

  if(activation > 2) {
    activation = 2;
  }

  refresh_activation_image();
}

void activation_single_down_click_handler(ClickRecognizerRef recognizer, void *context) {
  setActivation(activation);
  window_stack_push(tree_window_get_window(), true);
}

void activation_single_click_handler(ClickRecognizerRef recognizer, void *context){
  activation--;

  if(activation < 0) {
    activation = 0;
  }
  
  refresh_activation_image();
}

void activation_back_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_pop(true);
}

/***********************************
* Right buttons click config       *
***********************************/
void activation_click_config_provider(void *context){
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)activation_single_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)activation_single_down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)activation_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)activation_back_click_handler);
}


/***********************************
* Load event of the window         *
***********************************/
void activation_window_load(Window *window){
  
  // init window
  Layer *window_layer = window_get_root_layer(window);
  optionImageLayer = bitmap_layer_create(GRect(0, 0, 144 - ACTION_BAR_WIDTH, 168));
  bitmap_layer_set_compositing_mode(optionImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(optionImageLayer));
  
  // add heading
  GRect bounds = layer_get_bounds(window_layer);
  const GEdgeInsets label_insets = {.top = 5, .right = ACTION_BAR_WIDTH + 5, .left = 5, .bottom = 5};
  heading_text_layer = text_layer_create(grect_inset(bounds, label_insets));
  text_layer_set_background_color(heading_text_layer, GColorClear);
  text_layer_set_text_alignment(heading_text_layer, GTextAlignmentCenter);
  text_layer_set_font(heading_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text(heading_text_layer, "How activated do you feel?");
  layer_add_child(window_layer, text_layer_get_layer(heading_text_layer));
  
  // add the menu
  s_more_bitmap = gbitmap_create_with_resource(RESOURCE_ID_more);
  s_less_bitmap = gbitmap_create_with_resource(RESOURCE_ID_less);
  s_go_bitmap = gbitmap_create_with_resource(RESOURCE_ID_NEXT);
  s_action_bar_layer = action_bar_layer_create();
  action_bar_layer_add_to_window(s_action_bar_layer, window);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_more_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_less_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_go_bitmap);
  
  // refresh the shown image
  refresh_activation_image();
  
  //set click providers
  window_set_click_config_provider(window, activation_click_config_provider);
}

/***********************************
* Unload event of the window       *
***********************************/
void activation_window_unload(){
  bitmap_layer_destroy(optionImageLayer);
  gbitmap_destroy(optionImage);
  optionImage = NULL;
}

/***********************************
* Init the window          *
***********************************/
void init_activation_input_window(void) {
  activationWindow = window_create();
  window_set_background_color(activationWindow, GColorWhite);
  window_set_window_handlers(activationWindow, (WindowHandlers){.load = activation_window_load, .unload = activation_window_unload});
}

/***********************************
* Deinit the window         *
***********************************/
void deinit_activation_input_window(void) {
  window_destroy(activationWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *activation_input_window_get_window(void) {
  return activationWindow;
}
