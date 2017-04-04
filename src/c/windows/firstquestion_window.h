#pragma once
#include <pebble.h>
#include "exit_window.h"
#include "../modules/back_button_manager.h"
#include "src/c/windows/tree_window.h"

extern void init_firstquestion_window();
extern void deinit_firstquestion_window();
Window *firstquestion_window_get_window();