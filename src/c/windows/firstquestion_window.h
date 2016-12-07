#pragma once
#include <pebble.h>
#include "exit_window.h"
#include "secondquestionA_window.h"
#include "secondquestionB_window.h"
#include "secondquestionC_window.h"
#include "secondquestionD_window.h"
#include "../modules/back_button_manager.h"

extern void init_firstquestion_window();
extern void deinit_firstquestion_window();
Window *firstquestion_window_get_window();