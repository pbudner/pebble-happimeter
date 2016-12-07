#pragma once
#include <pebble.h>
#include "exit_window.h"
#include "firstquestion_window.h"
#include "../main.h"
#include "tree_window.h"

extern void init_secondquestionC_window();
extern void deinit_secondquestionC_window();
extern Window *secondquestionC_window_get_window();