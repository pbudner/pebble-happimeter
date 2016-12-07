#pragma once

#include <pebble.h>

#define DIALOG_MESSAGE_WINDOW_MESSAGE  "You need to enter your email address first. Go to app configuration, enter Your email address and restart Happimeter"
#define DIALOG_MESSAGE_WINDOW_MARGIN   10

extern void init_missingconfig_window(void);
extern void deinit_missingconfig_window(void);
extern Window *missingconfig_window_get_window(void);