#pragma once
#include <pebble.h>
#include "../main.h"
#include "../modules/data_manager.h"

#define TREE_KEY 0


extern void init_tree_window(void);
extern void deinit_tree_window(void);
extern Window *tree_window_get_window(void);
