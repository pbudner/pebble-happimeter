#pragma once
#include <pebble.h>
#include "exit_window.h"
#include "firstquestion_window.h"
#include "../main.h"
#include "../modules/data_manager.h"
#include "tree_window.h"

extern void init_secondquestionD_window();
extern void deinit_secondquestionD_window();
extern Window *secondquestionD_window_get_window();