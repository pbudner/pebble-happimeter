#include "secondquestionC_window.h"

static Window *secondquestionCWindow;
static MenuLayer *secondquestionCMenuLayer;
static GBitmap *contented, *serene, *relaxed, *calm, *which_Mood_Header;

/***********************************
* Get num of sections of the menu  *
***********************************/
uint16_t c_select_get_num_sections_callback(MenuLayer *menu_layer, void *data)
{
  return 1;
}

/***********************************
* Get num of rows of the menu      *
***********************************/
uint16_t c_select_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
  switch (section_index)
  {
  case 0:
    return 4;
  default:
    return 0;
  }
}

/***********************************
* Drawing of the menu header       *
***********************************/
static void c_select_draw_header(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *callback_context)
{
  GRect bounds = layer_get_bounds(cell_layer);
  //menu_cell_basic_draw(ctx, cell_layer, "", NULL, HowAreYouFeeling);
  graphics_context_set_fill_color(ctx, GColorWhite);
  //graphics_context_set_text_color(ctx, GColorBlack);
  // draw the box; 3rd and 4th variables: rounding the corners of the box
  // the watch has a displey of 200 p width
  graphics_fill_rect(ctx, GRect((bounds.size.w - 144) / 2, 4, 144, 40), 8, GCornersAll);
  graphics_draw_bitmap_in_rect(ctx, which_Mood_Header, bounds);
  // text in the box
  //graphics_draw_text(ctx, ("How are you feeling?"),fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),GRect(0, 0, bounds.size.w, 80), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

/***********************************
* Size of the menu header box      *
***********************************/
int16_t c_select_header_height(struct MenuLayer *secondquestionCMenuLayer, uint16_t section_index, void *callback_context)
{
  return 40;
}

/***********************************
* Header height callback           *
***********************************/
int16_t c_select_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

/***********************************
* Draw rows of the menu            *
***********************************/
void c_select_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data)
{
  switch (cell_index->section)
  {
  case 0:
    switch (cell_index->row)
    {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, contented);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, serene);
      break;
    case 2:
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, relaxed);
      break;
    case 3:
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, calm);
      break;
    }

    break;
  }
}

/***********************************
* Detects when select button is hit*
* and saves the answer!            *
***********************************/
void c_select_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{
  switch (cell_index->row)
  {
  case 0:
    setMoodAnswer(3, 1);
    break;
  case 1:
    setMoodAnswer(2, 1);
    break;
  case 2:
    setMoodAnswer(3, 0);
    break;
  case 3:
    setMoodAnswer(2, 0);
    break;
  }

  window_stack_pop_all(false);
  window_stack_push(tree_window_get_window(), true);
}

/***********************************
* Setup the menu layer             *
***********************************/
void c_select_menu_layer(Window *window)
{

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  secondquestionCMenuLayer = menu_layer_create(bounds);
  menu_layer_set_normal_colors(secondquestionCMenuLayer, GColorWhite, GColorWhite);
  menu_layer_set_highlight_colors(secondquestionCMenuLayer, GColorLightGray, GColorBlack);
  menu_layer_set_callbacks(secondquestionCMenuLayer, NULL, (MenuLayerCallbacks){
                                                               .get_num_sections = c_select_get_num_sections_callback,
                                                               .get_num_rows = c_select_get_num_rows_callback,
                                                               .get_header_height = c_select_header_height,
                                                               .draw_header = c_select_draw_header,
                                                               .draw_row = c_select_draw_row_callback,
                                                               .select_click = c_select_select_callback});

  menu_layer_set_click_config_onto_window(secondquestionCMenuLayer, window);

  // overwrite default setting for back button
  //force_back_button(window, secondquestionCMenuLayer);
  layer_add_child(window_layer, menu_layer_get_layer(secondquestionCMenuLayer));
}

/***********************************
* Load event of the main window    *
***********************************/
void secondquestionC_window_load(Window *window)
{
  c_select_menu_layer(window);

  
#if defined(PBL_PLATFORM_BASALT)
 contented = gbitmap_create_with_resource(RESOURCE_ID_Contented_Time);
 #else
 contented = gbitmap_create_with_resource(RESOURCE_ID_Contented_Black_White);
 #endif
  
#if defined(PBL_PLATFORM_BASALT)
  serene = gbitmap_create_with_resource(RESOURCE_ID_Serene_Time);
  #else
  serene = gbitmap_create_with_resource(RESOURCE_ID_Serene_Black_White);
  #endif
  
#if defined(PBL_PLATFORM_BASALT)
  relaxed = gbitmap_create_with_resource(RESOURCE_ID_Relaxed_Time);
  #else
  relaxed = gbitmap_create_with_resource(RESOURCE_ID_Relaxed_Black_White);
  #endif
  
#if defined(PBL_PLATFORM_BASALT)
  calm = gbitmap_create_with_resource(RESOURCE_ID_Calm_Time);
  #else
  calm = gbitmap_create_with_resource(RESOURCE_ID_Calm_Black_White);
  #endif
  
#if defined(PBL_PLATFORM_BASALT)
  which_Mood_Header = gbitmap_create_with_resource(RESOURCE_ID_Which_Mood_Time);
  #else
  which_Mood_Header = gbitmap_create_with_resource(RESOURCE_ID_Which_Mood_Black_White);
  #endif
  










}

/***********************************
* Unload event of the window       *
***********************************/
void secondquestionC_window_unload(Window *window)
{
  menu_layer_destroy(secondquestionCMenuLayer);
  gbitmap_destroy(contented);
  gbitmap_destroy(serene);
  gbitmap_destroy(relaxed);
  gbitmap_destroy(calm);
  gbitmap_destroy(which_Mood_Header);
}

/***********************************
* Appear event of the window       *
***********************************/
void secondquestionC_window_appear(Window *window)
{
  setNumberOfWindow(3);
}

/***********************************
* Init the window                  *
***********************************/
void init_secondquestionC_window()
{
  secondquestionCWindow = window_create();
  window_set_window_handlers(secondquestionCWindow, (WindowHandlers){
                                                        .load = secondquestionC_window_load,
                                                        .unload = secondquestionC_window_unload,
                                                        .appear = secondquestionC_window_appear});
  window_stack_push(secondquestionCWindow, true);
}

/***********************************
* Deinit the window                *
***********************************/
void deinit_secondquestionC_window()
{
  window_destroy(secondquestionCWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *secondquestionC_window_get_window()
{
  return secondquestionCWindow;
}
