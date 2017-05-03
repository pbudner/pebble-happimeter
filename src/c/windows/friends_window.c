#include "friends_window.h"

static Window *friendsWindow;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[1];
static SimpleMenuItem s_menu_items[5];
static GBitmap *s_menu_icon_image;

/***********************************
* Load event of the window         *
***********************************/
void window_load(Window *window){
  Layer *window_layer = window_get_root_layer(window);int num_a_items = 0;
  s_menu_icon_image = gbitmap_create_with_resource(RESOURCE_ID_SOCIAL);
  s_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Peter Gloor",
    .subtitle = "pgloor@mit.edu",
    .icon = s_menu_icon_image,
  };
  s_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Joscha Eirich",
    .subtitle = "jeirich@mit.edu",
    .icon = s_menu_icon_image,
  };
  s_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Pascal Budner",
    .subtitle = "pbudner@mit.edu",
    .icon = s_menu_icon_image,
  };
  s_menu_sections[0] = (SimpleMenuSection) {
    .title = "Friends' Mood",
    .num_items = num_a_items,
    .items = s_menu_items,
  };

  GRect bounds = layer_get_frame(window_layer);
  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, 1, NULL);
  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

/***********************************
* Unload event of the window       *
***********************************/
void window_unload(){
  simple_menu_layer_destroy(s_simple_menu_layer);
  gbitmap_destroy(s_menu_icon_image);
}

/***********************************
* Init the window          *
***********************************/
void init_friends_window(void) {
  friendsWindow = window_create();
  window_set_background_color(friendsWindow, GColorWhite);
  window_set_window_handlers(friendsWindow, (WindowHandlers){.load = window_load, .unload = window_unload});
}

/***********************************
* Deinit the window         *
***********************************/
void deinit_friends_window(void) {
  window_destroy(friendsWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *friends_window_get_window(void) {
  return friendsWindow;
}
