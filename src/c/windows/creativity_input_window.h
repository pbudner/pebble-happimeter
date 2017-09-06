#pragma once
#include <pebble.h>
#include "../modules/data_manager.h"
#include "creativity_input_window.h"
#include "tree_window.h"

extern void init_creativity_input_window(void);
extern void deinit_creativity_input_window(void);
extern void deinit_creativity_input_windows(void);
extern Window *creativity_input_window_get_window(void);
extern Window *creativity_input_window_get_next_window(void);
