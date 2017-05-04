#pragma once
#include <pebble.h>
#include "../modules/data_manager.h"

extern void init_friends_window(void);
extern void deinit_friends_window(void);
extern Window *friends_window_get_window(void);
extern void add_friend(char*, char*, int32_t, int32_t);