#include <pebble.h>
#include "smileymatrix_window.h"

#define TREE_KEY 0

extern void init_introduction_window(void);
extern void deinit_introduction_window(void);
extern void set_mood_window_text(int happiness, int activation);
extern Window *introduction_window_get_window(void);
