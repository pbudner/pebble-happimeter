#pragma once
#include <pebble.h>
#include "measurement_storage.h"
#include "../windows/upload_data_window.h"
#include "config_manager.h"

#define SOURCE_FOREGROUND  0
#define SOURCE_BACKGROUND  2

extern bool is_open_upload_task();
extern void init_data_manager(void);
extern bool upload_iteration();
extern void upload_mood(int pleasant, int activation, int creativity);
extern void upload_measure();
extern void outbox_sent_handler(DictionaryIterator *iter, void *context);
extern bool upload_iteration();
extern void worker_message_handler(uint16_t type, AppWorkerMessage *message);
extern void setHappiness(int _pleasant);
extern void setActivation(int _activation);
extern void setCreativity(int _creativity);
extern int getPleasant();
extern int getActivation();
extern int getCreativity();
extern void received_finished_upload();
extern void request_friends();
extern void request_mood();
extern void ping_js();