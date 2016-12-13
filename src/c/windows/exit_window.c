#include "exit_window.h"

static Window *exitWindow;
static MenuLayer *exitMenuLayer;
static TextLayer *s_label_layer;
static BitmapLayer *s_icon_layer;
static ActionBarLayer *s_action_bar_layer;
static GBitmap *s_icon_bitmap, *s_tick_bitmap, *s_cross_bitmap, *exit_Window;
int number = 0;

/***********************************
* Exit the app                     *
***********************************/
void exit_up_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
  window_stack_pop_all(true);
}

/***********************************
* Going back to the window where   *
* we clicked on the back button    *
***********************************/
void exit_down_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
  if (number == 0)
  {
    window_stack_push(firstquestion_window_get_window(), true);
  }
  if (number == 1)
  {
    window_stack_push(secondquestionA_window_get_window(), true);
  }
  if (number == 2)
  {
    window_stack_push(secondquestionB_window_get_window(), true);
  }
  if (number == 3)
  {
    window_stack_push(secondquestionC_window_get_window(), true);
  }
  if (number == 4)
  {
    window_stack_push(secondquestionD_window_get_window(), true);
  }
  if (number < 0 || number > 4)
  {
    window_stack_pop_all(true);
  }
}

/***********************************
* Single click handler for exit    *
* window                           *
***********************************/
void exit_back_single_click_handler(ClickRecognizerRef recognizer, void *context)
{
  // behaviour on 'back' equals 'down'
  exit_down_single_click_handler(recognizer, &context);
}

/***********************************
* CCP for the exit window          *
***********************************/
void exit_click_config_provider(void *context)
{
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)exit_up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)exit_down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)exit_back_single_click_handler);
}

/***********************************
* Set window number of the exit    *
***********************************/
void setNumberOfWindow(int n)
{
  number = n;
}

/***********************************
* Load event of the exit window    *
***********************************/
void exit_window_load(Window *window)
{

  Layer *window_layer = window_get_root_layer(window);
// GRect bounds = layer_get_bounds(window_layer);
#if defined(PBL_PLATFORM_EMERY)
  exit_Window = gbitmap_create_with_resource(RESOURCE_ID_Exit_Window_Emery);
#else
  exit_Window = gbitmap_create_with_resource(RESOURCE_ID_Exit_Window_Time);
#endif

#if defined(PBL_PLATFORM_EMERY)
  s_icon_layer = bitmap_layer_create(GRect(0, 0, 200, 228));
#else
  s_icon_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
#endif

  bitmap_layer_set_bitmap(s_icon_layer, exit_Window);
  bitmap_layer_set_compositing_mode(s_icon_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_icon_layer));

  s_tick_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TICK);
  s_cross_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CROSS);

  s_action_bar_layer = action_bar_layer_create();
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_tick_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_cross_bitmap);
  action_bar_layer_add_to_window(s_action_bar_layer, window);

  // Set the click config provider:
  action_bar_layer_set_click_config_provider(s_action_bar_layer, exit_click_config_provider);
}

/***********************************
* Unload event of the exit window  *
***********************************/
void exit_window_unload(Window *window)
{
  text_layer_destroy(s_label_layer);
  action_bar_layer_destroy(s_action_bar_layer);
  bitmap_layer_destroy(s_icon_layer);

  gbitmap_destroy(s_icon_bitmap);
  gbitmap_destroy(s_tick_bitmap);
  gbitmap_destroy(s_cross_bitmap);
  gbitmap_destroy(exit_Window);

  window_destroy(window);
  exitWindow = NULL;
}

/***********************************
* Inits the exit window            *
***********************************/
void init_exit_window()
{
  if (!exitWindow)
  {
    exitWindow = window_create();
    window_set_background_color(exitWindow, PBL_IF_COLOR_ELSE(GColorWhite, GColorWhite));
    window_set_window_handlers(exitWindow, (WindowHandlers){
                                               .load = exit_window_load,
                                               .unload = exit_window_unload,
                                           });
  }
  window_stack_push(exitWindow, true);
}

/***********************************
* Deinit the exit window           *
***********************************/
void deinit_exit_window()
{
  window_destroy(exitWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *exit_window_get_window()
{
  return exitWindow;
}