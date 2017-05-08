#pragma once
#include <pebble.h>
#include "../modules/data_manager.h"
#include "activation_input_window.h"

extern void init_happiness_input_window(void);
extern void deinit_happiness_input_window(void);
extern Window *happiness_input_window_get_window(void);
