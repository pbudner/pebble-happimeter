#pragma once
#include <pebble.h>
#include "../modules/data_manager.h"
#include "tree_window.h"

extern void init_activation_input_window(void);
extern void deinit_activation_input_window(void);
extern Window *activation_input_window_get_window(void);
