#pragma once
#include <pebble.h>
#include "config_manager.h"
#include "data_manager.h"
#include "../windows/missingconfig_window.h"
#include "../windows/firstquestion_window.h"

extern void init_inbox_message_manager(void);
extern void deinit_inbox_message_manager(void);