#include "happiness_input_window.h"

static Window *activationWindow;
static GBitmap *optionImage, *smileyImage;
static BitmapLayer *optionImageLayer, *smileyImageLayer;
static TextLayer *heading_text_layer;
static ActionBarLayer *s_action_bar_layer;
static GBitmap *s_more_bitmap, *s_less_bitmap, *s_go_bitmap;
static int activation = 1;

void refresh_activation_image() {
  if(optionImage != NULL) {
    gbitmap_destroy(optionImage);
    optionImage = NULL;
  }
  
  if(smileyImage != NULL) {
    gbitmap_destroy(smileyImage);
    smileyImage = NULL;
  }
  
  int happiness = getPleasant();

  switch (activation)
  {
    case 0:
      optionImage = gbitmap_create_with_resource(RESOURCE_ID_activation_0);
      if(happiness == 0) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_0_h_0);
      } else if(happiness == 1) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_0_h_1);
      } else if(happiness == 2) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_0_h_2);
      }
      break;
    case 1:
      optionImage = gbitmap_create_with_resource(RESOURCE_ID_activation_1);
      if(happiness == 0) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_1_h_0);
      } else if(happiness == 1) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_1_h_1);
      } else if(happiness == 2) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_1_h_2);
      }
      break;
    case 2:
      optionImage = gbitmap_create_with_resource(RESOURCE_ID_activation_2);
      if(happiness == 0) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_2_h_0);
      } else if(happiness == 1) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_2_h_1);
      } else if(happiness == 2) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_2_h_2);
      }
      break;
    default:
      optionImage = NULL;
      smileyImage = NULL;
  }

  bitmap_layer_set_bitmap(optionImageLayer, optionImage);
  layer_mark_dirty(bitmap_layer_get_layer(optionImageLayer));
  bitmap_layer_set_bitmap(smileyImageLayer, smileyImage);
  layer_mark_dirty(bitmap_layer_get_layer(smileyImageLayer));

}

void activation_single_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  activation++;

  if(activation > 2) {
    activation = 2;
  }

  refresh_activation_image();
}

void activation_single_down_click_handler(ClickRecognizerRef recognizer, void *context) {
  activation--;

  if(activation < 0) {
    activation = 0;
  }
  
  refresh_activation_image();
}

void activation_single_click_handler(ClickRecognizerRef recognizer, void *context){
  setActivation(activation);
  window_stack_push(creativity_input_window_get_next_window(), true);
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
  optionImageLayer = bitmap_layer_create(GRect(0, 168 - 75, 144 - ACTION_BAR_WIDTH, 70));
  bitmap_layer_set_compositing_mode(optionImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(optionImageLayer));
  
  smileyImageLayer = bitmap_layer_create(GRect(-ACTION_BAR_WIDTH / 2, 0, 144 - ACTION_BAR_WIDTH, 168));
  bitmap_layer_set_compositing_mode(smileyImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(smileyImageLayer));
  
  // add heading
  GRect bounds = layer_get_bounds(window_layer);
  const GEdgeInsets label_insets = {.top = 5, .right = ACTION_BAR_WIDTH + 5, .left = 5, .bottom = 5};
  heading_text_layer = text_layer_create(grect_inset(bounds, label_insets));
  text_layer_set_background_color(heading_text_layer, GColorClear);
  text_layer_set_text_alignment(heading_text_layer, GTextAlignmentCenter);
  text_layer_set_font(heading_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text(heading_text_layer, "How active do you feel?");
  layer_add_child(window_layer, text_layer_get_layer(heading_text_layer));
  
  // add the menu
  s_more_bitmap = gbitmap_create_with_resource(RESOURCE_ID_more);
  s_less_bitmap = gbitmap_create_with_resource(RESOURCE_ID_less);
  s_go_bitmap = gbitmap_create_with_resource(RESOURCE_ID_NEXT);
  s_action_bar_layer = action_bar_layer_create();
  action_bar_layer_add_to_window(s_action_bar_layer, window);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_more_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_less_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_go_bitmap);
  
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
  bitmap_layer_destroy(smileyImageLayer);
  gbitmap_destroy(optionImage);
  gbitmap_destroy(smileyImage);
  optionImage = NULL;
  smileyImage = NULL;
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
