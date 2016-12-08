#include "secondquestionA_window.h"

static Window *secondquestionAWindow;
static MenuLayer *secondquestionAMenuLayer;
static GBitmap *alert, *exited, *elated, *happy, *which_Mood_Header;

/***********************************
* Get num of sections of the menu  *
***********************************/
uint16_t a_select_get_num_sections_callback(MenuLayer *menu_layer, void *data)
{
  return 1;
}

/***********************************
* Get num of rows of the menu      *
***********************************/
uint16_t a_select_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data)
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
static void a_select_draw_header(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *callback_context)
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
int16_t a_select_header_height(struct MenuLayer *secondquestionAMenuLayer, uint16_t section_index, void *callback_context)
{
  return 40;
}

/***********************************
* Header height callback           *
***********************************/
int16_t a_select_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

/***********************************
* Draw rows of the menu            *
***********************************/
void a_select_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data)
{
  switch (cell_index->section)
  {
  case 0:
    switch (cell_index->row)
    {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, alert);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, exited);
      break;
    case 2:
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, elated);
      break;
    case 3:
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, happy);
      break;
    }

    break;
  }
}

/***********************************
* Detects when select button is hit*
* and saves the answer!            *
***********************************/
void a_select_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{
  //setAnswer(0,(int) cell_index->row);
  window_stack_push(tree_window_get_window(), true);
  //window_stack_pop_all(true);
}

/***********************************
* Setup the menu layer             *
***********************************/
void a_select_menu_layer(Window *window)
{

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  secondquestionAMenuLayer = menu_layer_create(bounds);
  menu_layer_set_normal_colors(secondquestionAMenuLayer, GColorWhite, GColorWhite);
  menu_layer_set_highlight_colors(secondquestionAMenuLayer, GColorLightGray, GColorBlack);
  menu_layer_set_callbacks(secondquestionAMenuLayer, NULL, (MenuLayerCallbacks){
                                                               .get_num_sections = a_select_get_num_sections_callback,
                                                               .get_num_rows = a_select_get_num_rows_callback,
                                                               .get_header_height = a_select_header_height,
                                                               .draw_header = a_select_draw_header,
                                                               .draw_row = a_select_draw_row_callback,
                                                               .select_click = a_select_select_callback});

  menu_layer_set_click_config_onto_window(secondquestionAMenuLayer, window);

  // overwrite default setting for back button
  //force_back_button(window, secondquestionAMenuLayer);
  layer_add_child(window_layer, menu_layer_get_layer(secondquestionAMenuLayer));
}

/***********************************
* Load event of the main window    *
***********************************/
void secondquestionA_window_load(Window *window)
{
  a_select_menu_layer(window);

//#if defined(PBL_PLATFORM_EMERY)
 // Nobody = gbitmap_create_with_resource(RESOURCE_ID_Nobody_Emery);
//#else
  //Nobody = gbitmap_create_with_resource(RESOURCE_ID_Nobody_Time);
//#endif


  alert = gbitmap_create_with_resource(RESOURCE_ID_Alert_Emery);

  exited = gbitmap_create_with_resource(RESOURCE_ID_Exited_Emery);

  elated = gbitmap_create_with_resource(RESOURCE_ID_Elated_Emery);

  happy = gbitmap_create_with_resource(RESOURCE_ID_Happy_Emery);

  which_Mood_Header = gbitmap_create_with_resource(RESOURCE_ID_WhoAreYouWithBolt_Emery);

}

/***********************************
* Unload event of the window       *
***********************************/
void secondquestionA_window_unload(Window *window)
{
  menu_layer_destroy(secondquestionAMenuLayer);
  gbitmap_destroy(alert);
  gbitmap_destroy(exited);
  gbitmap_destroy(elated);
  gbitmap_destroy(happy);
  gbitmap_destroy(which_Mood_Header);

}

/***********************************
* Appear event of the window       *
***********************************/
void secondquestionA_window_appear(Window *window)
{
  setNumberOfWindow(1);
}

/***********************************
* Init the window                  *
***********************************/
void init_secondquestionA_window()
{
  secondquestionAWindow = window_create();
  window_set_window_handlers(secondquestionAWindow, (WindowHandlers){
                                                        .load = secondquestionA_window_load,
                                                        .unload = secondquestionA_window_unload,
                                                        .appear = secondquestionA_window_appear});
  window_stack_push(secondquestionAWindow, true);
}

/***********************************
* Deinit the window                *
***********************************/
void deinit_secondquestionA_window()
{
  window_destroy(secondquestionAWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *secondquestionA_window_get_window()
{
  return secondquestionAWindow;
}