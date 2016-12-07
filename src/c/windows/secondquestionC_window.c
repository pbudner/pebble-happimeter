#include "secondquestionC_window.h"

static Window *secondquestionCWindow;
static MenuLayer *secondquestionCMenuLayer;
static GBitmap *Nobody, *Pet, *Friend, *Colleague, *Family, *Significant_Other, *Other, *WhoAreYouWith;

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
  graphics_draw_bitmap_in_rect(ctx, WhoAreYouWith, bounds);
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

/***********************************
* Detects when select button is hit*
* and saves the answer!            *
***********************************/
void c_select_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{
  //setAnswer(0,(int) cell_index->row);
  window_stack_push(tree_window_get_window(), true);
  //window_stack_pop_all(true);
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

/***********************************
* Unload event of the window       *
***********************************/
void secondquestionC_window_unload(Window *window)
{
  menu_layer_destroy(secondquestionCMenuLayer);
  gbitmap_destroy(Nobody);
  gbitmap_destroy(Pet);
  gbitmap_destroy(Friend);
  gbitmap_destroy(Colleague);
  gbitmap_destroy(Family);
  gbitmap_destroy(Significant_Other);
  gbitmap_destroy(Other);
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
