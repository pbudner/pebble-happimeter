#include "missingconfig_window.h"

static Window *emailWindow;
static GBitmap *emailImage;
static BitmapLayer *emailImageLayer;

/***********************************
* Load event of the window         *
***********************************/
void missingconfig_window_load(Window *window){
  Layer *window_layer = window_get_root_layer(window);
  
  // Loads a png Image from ressources
  #if defined(PBL_PLATFORM_EMERY)
  emailImage = gbitmap_create_with_resource(RESOURCE_ID_Alert_Sign_Emery);
  emailImageLayer = bitmap_layer_create(GRect(0,0,200,228));
  #else
  emailImage = gbitmap_create_with_resource(RESOURCE_ID_Alert_Sign_Time);
  emailImageLayer = bitmap_layer_create(GRect(0,0,144,168));
  #endif
  bitmap_layer_set_bitmap(emailImageLayer, emailImage);
  bitmap_layer_set_compositing_mode(emailImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(emailImageLayer));  
}

/***********************************
* Unload event of the window       *
***********************************/
void missingconfig_window_unload(){
  bitmap_layer_destroy(emailImageLayer);
  gbitmap_destroy(emailImage);
  window_destroy(emailWindow);
}

/***********************************
* Inits the splash window          *
***********************************/
void init_missingconfig_window(void) {
  emailWindow = window_create();
  window_set_background_color(emailWindow, GColorBlack);
  window_set_window_handlers(emailWindow, (WindowHandlers){.load = missingconfig_window_load, .unload = missingconfig_window_unload});
}

/***********************************
* Deinit the splash window         *
***********************************/
void deinit_missingconfig_window(void) {
  window_destroy(emailWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *missingconfig_window_get_window(void) {
  return emailWindow;
}