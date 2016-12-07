#include "secondquestionB_window.h"

static Window *secondquestionBWindow;
static MenuLayer *secondquestionBMenuLayer;
static GBitmap *Nobody,*Pet, *Friend, *Colleague, *Family, *Significant_Other, *Other, *WhoAreYouWith;



uint16_t b_select_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 1;
}

uint16_t b_select_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
   switch(section_index){
      case 0:
          return 4;
      default:
          return 0;
    }
}

// Content of the header
static void b_select_draw_header(GContext* ctx, const Layer* cell_layer, uint16_t section_index, void* callback_context) {
   GRect bounds = layer_get_bounds(cell_layer);
  //menu_cell_basic_draw(ctx, cell_layer, "", NULL, HowAreYouFeeling);
   graphics_context_set_fill_color(ctx, GColorWhite);
   //graphics_context_set_text_color(ctx, GColorBlack);
   // draw the box; 3rd and 4th variables: rounding the corners of the box
  // the watch has a displey of 200 p width
   graphics_fill_rect(ctx,GRect((bounds.size.w-144)/2,4,144,40),8,GCornersAll);
  graphics_draw_bitmap_in_rect(ctx, WhoAreYouWith, bounds);
   // text in the box
   //graphics_draw_text(ctx, ("How are you feeling?"),fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),GRect(0, 0, bounds.size.w, 80), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

// Size of the header box
int16_t b_select_header_height(struct MenuLayer *secondquestionBMenuLayer, uint16_t section_index, void *callback_context) {
  return 40;

}

int16_t b_select_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}



//void select_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  //menu_cell_basic_header_draw(ctx, cell_layer, "Who are you with");
//}

void b_select_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index->section){
      case 0:
        switch (cell_index->row){
          case 0:
              menu_cell_basic_draw(ctx, cell_layer, "", NULL, Nobody);
            break;
           case 1:
              menu_cell_basic_draw(ctx, cell_layer, "", NULL, Pet);
            break;
           case 2:
              menu_cell_basic_draw(ctx, cell_layer, "", NULL, Friend);
            break;
           case 3:
              menu_cell_basic_draw(ctx, cell_layer, "", NULL, Colleague);
            break;
        }
    
       break;
      
    }
}


  // save the given answer: question 1, answer 'cell_index'
  // PROBLEM: no proof, if it functions correctly
  // Pascal: Tested it and it works! ;-)
void b_select_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  //setAnswer(0,(int) cell_index->row);
  window_stack_push(tree_window_get_window(), true);
   //window_stack_pop_all(true);
}

void b_select_menu_layer(Window *window) {
  
	Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

    secondquestionBMenuLayer = menu_layer_create(bounds);
    menu_layer_set_normal_colors(secondquestionBMenuLayer,  GColorWhite, GColorWhite);
    menu_layer_set_highlight_colors(secondquestionBMenuLayer, GColorLightGray, GColorBlack);
    menu_layer_set_callbacks(secondquestionBMenuLayer, NULL, (MenuLayerCallbacks){
        .get_num_sections = b_select_get_num_sections_callback,
        .get_num_rows = b_select_get_num_rows_callback,
        .get_header_height = b_select_header_height,
        .draw_header = b_select_draw_header,
        .draw_row = b_select_draw_row_callback,
        .select_click = b_select_select_callback
    });

    menu_layer_set_click_config_onto_window(secondquestionBMenuLayer, window);
  
    // overwrite default setting for back button
    //force_back_button(window, secondquestionBMenuLayer);
    layer_add_child(window_layer, menu_layer_get_layer(secondquestionBMenuLayer));
}


void secondquestionB_window_load(Window *window){
  b_select_menu_layer(window);
  
  #if defined(PBL_PLATFORM_EMERY)
   Nobody = gbitmap_create_with_resource(RESOURCE_ID_Nobody_Emery);
   #else
    Nobody = gbitmap_create_with_resource(RESOURCE_ID_Nobody_Time);
   #endif 
  
  #if defined(PBL_PLATFORM_EMERY)
     Pet = gbitmap_create_with_resource(RESOURCE_ID_Pet_Emery);
   #else
     Pet = gbitmap_create_with_resource(RESOURCE_ID_Pet_Time);
   #endif 
  
  #if defined(PBL_PLATFORM_EMERY)
    Friend = gbitmap_create_with_resource(RESOURCE_ID_Friend_Emery);
   #else
    Friend = gbitmap_create_with_resource(RESOURCE_ID_Friend_Time);
   #endif 
  
  #if defined(PBL_PLATFORM_EMERY)
    Colleague = gbitmap_create_with_resource(RESOURCE_ID_Colleauge_Emery);
   #else
    Colleague = gbitmap_create_with_resource(RESOURCE_ID_Colleauge_Time);
   #endif 
  
  #if defined(PBL_PLATFORM_EMERY)
    Family = gbitmap_create_with_resource(RESOURCE_ID_Family_Emery);
   #else
    Family = gbitmap_create_with_resource(RESOURCE_ID_Family_Time);
   #endif 
  
  #if defined(PBL_PLATFORM_EMERY)
    Significant_Other = gbitmap_create_with_resource(RESOURCE_ID_SignificantOther_Emery);
   #else
    Significant_Other = gbitmap_create_with_resource(RESOURCE_ID_SignificantOther_Time);
   #endif 
  
  #if defined(PBL_PLATFORM_EMERY)
    Other = gbitmap_create_with_resource(RESOURCE_ID_Other_Emery);
   #else
    Other = gbitmap_create_with_resource(RESOURCE_ID_Other_Time);
   #endif 
  
   #if defined(PBL_PLATFORM_EMERY)
    WhoAreYouWith = gbitmap_create_with_resource(RESOURCE_ID_WhoAreYouWithBolt_Emery);
   #else
    WhoAreYouWith = gbitmap_create_with_resource(RESOURCE_ID_WhoAreYouWithBolt_Time);
   #endif 
  
  
}



void secondquestionB_window_unload(Window *window){
  menu_layer_destroy(secondquestionBMenuLayer); 
  gbitmap_destroy(Nobody);
  gbitmap_destroy(Pet);
  gbitmap_destroy(Friend);
  gbitmap_destroy(Colleague);
  gbitmap_destroy(Family);
  gbitmap_destroy(Significant_Other);
  gbitmap_destroy(Other);
}

// to remember where we were, should we fetch the exit_window
void secondquestionB_window_appear(Window *window){
   setNumberOfWindow(2);
}

void init_secondquestionB_window(){
  secondquestionBWindow = window_create();
  window_set_window_handlers(secondquestionBWindow, (WindowHandlers) {
        .load = secondquestionB_window_load,
        .unload =secondquestionB_window_unload,
      .appear = secondquestionB_window_appear
    });
  window_stack_push(secondquestionBWindow, true);
}

void deinit_secondquestionB_window(){
  window_destroy(secondquestionBWindow);  
}

Window *secondquestionB_window_get_window(){
  return secondquestionBWindow;
}