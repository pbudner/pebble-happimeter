#pragma once
#include <pebble.h>

extern void init_upload_window(void);
extern void deinit_upload_window(void);
extern Window *upload_window_get_window(void);
