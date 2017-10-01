#pragma once
#include <pebble.h>
#include "../main.h"
#include "../modules/data_manager.h"
#include "./happiness_input_window.h"
#include "./activation_input_window.h"
#include "./creativity_input_window.h"

extern void init_tree_window(void);
extern void deinit_tree_window(void);
extern Window *tree_window_get_window(void);
