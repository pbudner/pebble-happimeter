#include "friends_window.h"

static Window *friendsWindow;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[1];
static SimpleMenuItem s_menu_items[10];
static char s_mail_buffer[10][1024];
static char s_name_buffer[10][1024];
static GBitmap *s_menu_icon_image_1_1;
static GBitmap *s_menu_icon_image_1_0;
static GBitmap *s_menu_icon_image_0_1;
static GBitmap *s_menu_icon_image_0_0;
static int num_a_items = 0;

/***********************************
* Adds a friend to the window      *
***********************************/
void add_friend(char *mail, char *name, int32_t happiness, int32_t activation) {
  snprintf(s_mail_buffer[num_a_items], sizeof(s_mail_buffer[num_a_items]), "%s", mail);
  snprintf(s_name_buffer[num_a_items], sizeof(s_name_buffer[num_a_items]), "%s", name);
  APP_LOG(APP_LOG_LEVEL_INFO, "Received friend %s", mail);
  
  GBitmap *icon = NULL;
  
  if(happiness == 1 && activation == 1) {
    icon = s_menu_icon_image_1_1;
  } else if(happiness == 1 && activation == 0) {
    icon = s_menu_icon_image_1_0;
  } else if(happiness == 0 && activation == 0) {
    icon = s_menu_icon_image_0_0;
  } else if(happiness == 0 && activation == 1) {
    icon = s_menu_icon_image_0_1;
  } 
  
  s_menu_items[num_a_items] = (SimpleMenuItem) {
    .title = s_name_buffer[num_a_items],
    .subtitle = s_mail_buffer[num_a_items],
    .icon = icon,
  };
  
  num_a_items++;
  s_menu_sections[0] = (SimpleMenuSection) {
    .title = "Your Friends' Mood",
    .num_items = num_a_items,
    .items = s_menu_items,
  };
  
  MenuLayer *s_menu_layer = simple_menu_layer_get_menu_layer(s_simple_menu_layer);
  menu_layer_reload_data(s_menu_layer);
}

/***********************************
* Load event of the window         *
***********************************/
void window_load(Window *window){
  // clear the old items
  num_a_items = 0;
  memset(&s_menu_items[0], 0, sizeof(s_menu_items));
  
  // add the layers to the window
  Layer *window_layer = window_get_root_layer(window);
  s_menu_icon_image_1_1 = gbitmap_create_with_resource(RESOURCE_ID_mood0_24x24);
  s_menu_icon_image_1_0 = gbitmap_create_with_resource(RESOURCE_ID_mood1_24x24);
  s_menu_icon_image_0_0 = gbitmap_create_with_resource(RESOURCE_ID_mood2_24x24);
  s_menu_icon_image_0_1 = gbitmap_create_with_resource(RESOURCE_ID_mood3_24x24);
  GRect bounds = layer_get_frame(window_layer);
  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, 1, NULL);
  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

/***********************************
* Unload event of the window       *
***********************************/
void window_unload(){
  simple_menu_layer_destroy(s_simple_menu_layer);
  gbitmap_destroy(s_menu_icon_image_1_1);
  gbitmap_destroy(s_menu_icon_image_1_0);
  gbitmap_destroy(s_menu_icon_image_0_0);
  gbitmap_destroy(s_menu_icon_image_0_1);
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
