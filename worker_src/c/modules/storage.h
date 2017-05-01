#pragma once

#define SETTINGS_STORAGE_KEY 10

extern enum StatusCode save_measure(int type, int value, int id);
extern int get_last_measure_id();
extern int is_configured();