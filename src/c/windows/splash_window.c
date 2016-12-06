#include "splash_window.h"

static Window *splashWindow;
static GBitmap *splashImage;
static BitmapLayer *splashImageLayer;

/***********************************
* Load event of the window         *
***********************************/
void splash_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  splashImage = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SPLASH); // Loads a png Image from ressources
  splashImageLayer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(splashImageLayer, splashImage);
  bitmap_layer_set_compositing_mode(splashImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(splashImageLayer));
}

/***********************************
* Unload event of the window       *
***********************************/
void splash_window_unload(Window *window) {
  gbitmap_destroy(splashImage);
  bitmap_layer_destroy(splashImageLayer);
}

/***********************************
* Inits the splash window          *
***********************************/
void init_splash_window(void) {
  splashWindow = window_create();
  window_set_window_handlers(splashWindow, (WindowHandlers){.load = splash_window_load, .unload = splash_window_unload});
}

/***********************************
* Deinit the splash window         *
***********************************/
void deinit_splash_window(void) {
  window_destroy(splashWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *splash_window_get_window(void) {
  return splashWindow;
}