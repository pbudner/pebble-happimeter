#pragma once
#include <pebble.h>
#include "happiness_input_window.h"
#include "friends_window.h"
#include "../modules/data_manager.h"
#include "../modules/wakeup_manager.h"
#include "tree_window.h"

extern void init_introduction_window(void);
extern void deinit_introduction_window(void);
extern void set_mood_window_text(int happiness, int activation);
extern Window *introduction_window_get_window(void);
extern void message_timeout_callback(void *data);
