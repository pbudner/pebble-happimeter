#pragma once
#include <pebble.h>

extern void init_exit_window();
extern void deinit_exit_window();
extern void setNumberOfWindow(int n);
Window *exit_window_get_window();