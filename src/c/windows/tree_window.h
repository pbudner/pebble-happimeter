#pragma once
#include <pebble.h>
#include "../main.h"
#include "../modules/data_manager.h"

#define TREE_KEY 0
Window *treeWindow;
GBitmap *treeImage = NULL;
BitmapLayer *treeImageLayer;
uint32_t counter;

int       frame_no1, 
          frame_no2, 
          frame_no3,
          frame_no4;
const int NO_OF_FRAMES1 = 49, 
          NO_OF_FRAMES2 = 49, 
          NO_OF_FRAMES3 = 49, 
          NO_OF_FRAMES4 = 49; 

extern void init_tree_window(void);
extern void deinit_tree_window(void);
extern Window *tree_window_get_window(void);
