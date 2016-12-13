#include "secondquestionB_window.h"

static Window *secondquestionBWindow;
static MenuLayer *secondquestionBMenuLayer;
static GBitmap *sad, *depressed, *bored, *fatigued,*which_Mood_Header;

/***********************************
* Get num of sections of the menu  *
***********************************/
uint16_t b_select_get_num_sections_callback(MenuLayer *menu_layer, void *data)
{
  return 1;
}

/***********************************
* Get num of rows of the menu      *
***********************************/
uint16_t b_select_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data)
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
static void b_select_draw_header(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *callback_context)
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
int16_t b_select_header_height(struct MenuLayer *secondquestionBMenuLayer, uint16_t section_index, void *callback_context)
{
  return 40;
}

/***********************************
* Header height callback           *
***********************************/
int16_t b_select_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

/***********************************
* Draw rows of the menu            *
***********************************/
void b_select_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data)
{
  switch (cell_index->section)
  {
  case 0:
    switch (cell_index->row)
    {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, sad);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, depressed);
      break;
    case 2:
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, bored);
      break;
    case 3:
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, fatigued);
      break;
    }

    break;
  }
}

/***********************************
* Detects when select button is hit*
* and saves the answer!            *
***********************************/
void b_select_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{
  switch (cell_index->row)
  {
  case 0:
    setMoodAnswer(0, 1);
    break;
  case 1:
    setMoodAnswer(1, 1);
    break;
  case 2:
    setMoodAnswer(0, 0);
    break;
  case 3:
    setMoodAnswer(1, 0);
    break;
  }

  window_stack_pop_all(false);
  window_stack_push(tree_window_get_window(), true);
}

/***********************************
* Setup the menu layer             *
***********************************/
void b_select_menu_layer(Window *window)
{

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  secondquestionBMenuLayer = menu_layer_create(bounds);
  menu_layer_set_normal_colors(secondquestionBMenuLayer, GColorWhite, GColorWhite);
  menu_layer_set_highlight_colors(secondquestionBMenuLayer, GColorLightGray, GColorBlack);
  menu_layer_set_callbacks(secondquestionBMenuLayer, NULL, (MenuLayerCallbacks){
                                                               .get_num_sections = b_select_get_num_sections_callback,
                                                               .get_num_rows = b_select_get_num_rows_callback,
                                                               .get_header_height = b_select_header_height,
                                                               .draw_header = b_select_draw_header,
                                                               .draw_row = b_select_draw_row_callback,
                                                               .select_click = b_select_select_callback});

  menu_layer_set_click_config_onto_window(secondquestionBMenuLayer, window);

  // overwrite default setting for back button
  //force_back_button(window, secondquestionBMenuLayer);
  layer_add_child(window_layer, menu_layer_get_layer(secondquestionBMenuLayer));
}

/***********************************
* Load event of the main window    *
***********************************/
void secondquestionB_window_load(Window *window)
{
  b_select_menu_layer(window);

  #if defined(PBL_PLATFORM_EMERY)
  sad = gbitmap_create_with_resource(RESOURCE_ID_Sad_Emery);
  #else
  sad = gbitmap_create_with_resource(RESOURCE_ID_Sad_Time);
  #endif

  #if defined(PBL_PLATFORM_EMERY)
  depressed = gbitmap_create_with_resource(RESOURCE_ID_Depressed_Emery);
  #else
  depressed = gbitmap_create_with_resource(RESOURCE_ID_Depressed_Time);
  #endif

  #if defined(PBL_PLATFORM_EMERY)
  bored = gbitmap_create_with_resource(RESOURCE_ID_Bored_Emery);
  #else
  bored = gbitmap_create_with_resource(RESOURCE_ID_Bored_Time);
  #endif

  #if defined(PBL_PLATFORM_EMERY)
  fatigued = gbitmap_create_with_resource(RESOURCE_ID_Fatigued_Emery);
  #else
  fatigued = gbitmap_create_with_resource(RESOURCE_ID_Fatigued_Time);
  #endif

  #if defined(PBL_PLATFORM_EMERY)
  which_Mood_Header = gbitmap_create_with_resource(RESOURCE_ID_Which_Mood_Emery);
  #else
  which_Mood_Header = gbitmap_create_with_resource(RESOURCE_ID_Which_Mood_Time);
  #endif

}

/***********************************
* Unload event of the window       *
***********************************/
void secondquestionB_window_unload(Window *window)
{
  menu_layer_destroy(secondquestionBMenuLayer);
  gbitmap_destroy(sad);
  gbitmap_destroy(depressed);
  gbitmap_destroy(bored);
  gbitmap_destroy(fatigued);
  gbitmap_destroy(which_Mood_Header);

}

/***********************************
* Appear event of the window       *
***********************************/
void secondquestionB_window_appear(Window *window)
{
  setNumberOfWindow(2);
}

/***********************************
* Init the window                  *
***********************************/
void init_secondquestionB_window()
{
  secondquestionBWindow = window_create();
  window_set_window_handlers(secondquestionBWindow, (WindowHandlers){
                                                        .load = secondquestionB_window_load,
                                                        .unload = secondquestionB_window_unload,
                                                        .appear = secondquestionB_window_appear});
  window_stack_push(secondquestionBWindow, true);
}

/***********************************
* Deinit the window                *
***********************************/
void deinit_secondquestionB_window()
{
  window_destroy(secondquestionBWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *secondquestionB_window_get_window()
{
  return secondquestionBWindow;
}
