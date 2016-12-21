#pragma once
#include <pebble.h>
#include "firstquestion_window.h"
#include "splash_window.h"

#define TREE_KEY 0

extern void init_wakeup_window(void);
extern void deinit_wakeup_window(void);
extern Window *wakeup_window_get_window(void);
