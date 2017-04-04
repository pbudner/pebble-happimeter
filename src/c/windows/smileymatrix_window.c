#include "introduction_window.h"

static Window *smileyMatrixWindow;
static GBitmap *smileyMatrixImage;
static BitmapLayer *smileyMatrixImageLayer;
static int selectedMatrixOption = 0;

void refresh_menu_image() {
  if(smileyMatrixImage != NULL) {
    gbitmap_destroy(smileyMatrixImage);
  }

  switch (selectedMatrixOption)
  {
    case 0:
      smileyMatrixImage = gbitmap_create_with_resource(RESOURCE_ID_Smiley_Matrix_1);
      break;
    case 1:
      smileyMatrixImage = gbitmap_create_with_resource(RESOURCE_ID_Smiley_Matrix_2);
      break;
    case 2:
      smileyMatrixImage = gbitmap_create_with_resource(RESOURCE_ID_Smiley_Matrix_3);
      break;
    case 3:
      smileyMatrixImage = gbitmap_create_with_resource(RESOURCE_ID_Smiley_Matrix_4);
      break;
    default:
      smileyMatrixImage = NULL;
  }

  bitmap_layer_set_bitmap(smileyMatrixImageLayer, smileyMatrixImage);
  layer_mark_dirty(bitmap_layer_get_layer(smileyMatrixImageLayer));

}

void smileymatrix_single_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  selectedMatrixOption = selectedMatrixOption + 1;

  if(selectedMatrixOption > 3) {
    selectedMatrixOption = 0;
  }

  refresh_menu_image();
}

void smileymatrix_single_down_click_handler(ClickRecognizerRef recognizer, void *context) {
  selectedMatrixOption = selectedMatrixOption - 1;

  if(selectedMatrixOption < 0) {
    selectedMatrixOption = 3;
  }
  
  refresh_menu_image();
}

void smileymatrix_single_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_push(firstquestion_window_get_window(), true); // show the main window
}

void smileymatrix_back_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_pop_all(true);
}

/***********************************
* Right buttons click config       *
***********************************/
void smileymatrix_click_config_provider(void *context){
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)smileymatrix_single_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)smileymatrix_single_down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)smileymatrix_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)smileymatrix_back_click_handler);
}


/***********************************
* Load event of the window         *
***********************************/
void smileymatrix_window_load(Window *window){

  //set config providers
  window_set_click_config_provider(window, smileymatrix_click_config_provider);

  Layer *window_layer = window_get_root_layer(window);
  smileyMatrixImageLayer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_compositing_mode(smileyMatrixImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(smileyMatrixImageLayer));
  refresh_menu_image();

  // overwrite default setting for back button
  // force_back_button(window, firstquestionMenuLayer);
}

/***********************************
* Unload event of the window       *
***********************************/
void smileymatrix_window_unload(){
  bitmap_layer_destroy(smileyMatrixImageLayer);
  gbitmap_destroy(smileyMatrixImage);
}

/***********************************
* Init the window          *
***********************************/
void init_smileymatrix_window(void) {
  smileyMatrixWindow = window_create();
  window_set_background_color(smileyMatrixWindow, GColorWhite);
  window_set_window_handlers(smileyMatrixWindow, (WindowHandlers){.load = smileymatrix_window_load, .unload = smileymatrix_window_unload});
}

/***********************************
* Deinit the window         *
***********************************/
void deinit_smileymatrix_window(void) {
  window_destroy(smileyMatrixWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *smileymatrix_window_get_window(void) {
  return smileyMatrixWindow;
}
