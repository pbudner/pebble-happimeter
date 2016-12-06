#pragma once
#include <pebble.h>

extern void init_splash_window(void);
extern void deinit_splash_window(void);
extern Window *splash_window_get_window(void);