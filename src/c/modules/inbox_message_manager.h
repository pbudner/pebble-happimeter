#pragma once
#include <pebble.h>
#include "config_manager.h"
#include "data_manager.h"
#include "../windows/missingconfig_window.h"
#include "../windows/introduction_window.h"
#include "../windows/friends_window.h"

#define SHOW_GENERIC_QUESTIONS_MODE_STORAGE_KEY 8


extern void init_inbox_message_manager(void);
extern void deinit_inbox_message_manager(void);