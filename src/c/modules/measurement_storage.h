#pragma once
#include <pebble.h>

extern void remove_measure(int id, int type);
extern void remove_all_measures(int id);
extern int get_measure(int id, int type, int removeAfterRead);
extern int get_last_measure_id();
