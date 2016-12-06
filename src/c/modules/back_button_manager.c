#include "back_button_manager.h"

static ClickConfigProvider previous_ccp; // stores the reference to the ClickConfigProvider originally set by the menu layer

/***********************************
* Defines the back button action   *
***********************************/
void back_button_handler(ClickRecognizerRef recognizer, void *context) {
  window_stack_push(exit_window_get_window(), true);
}

/***********************************
* New click config provider that   *
* simply calls the old one and then*
* subscribes for back button events*
***********************************/
void replace_click_config_provider(void *context) {
  previous_ccp(context);
  window_single_click_subscribe(BUTTON_ID_BACK, back_button_handler);
}

/***********************************
* Forces the overriden click conf- *
* ig provider to replace the orig- *
* inal ccp.                        *
***********************************/
void force_back_button(Window *window, MenuLayer *menu_layer) {
  previous_ccp = window_get_click_config_provider(window);
  window_set_click_config_provider_with_context(window, replace_click_config_provider, menu_layer);
}