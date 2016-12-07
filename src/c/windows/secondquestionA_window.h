#pragma once
#include <pebble.h>
#include "exit_window.h"
#include "tree_window.h"
#include "firstquestion_window.h"
#include "../main.h"

extern void init_secondquestionA_window();
extern void deinit_secondquestionA_window();
extern Window *secondquestionA_window_get_window();