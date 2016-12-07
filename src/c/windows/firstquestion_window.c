#include "firstquestion_window.h"

static Window *firstquestionWindow;
static MenuLayer *firstquestionMenuLayer;
static GBitmap *Very_Happy, *Happy, *Normal, *Unhappy, *Very_Unhappy, *HowAreYouFeeling;
static int16_t menu_header_height(struct MenuLayer *menu, uint16_t section_index, void *callback_context);
static void menu_draw_header(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *callback_context);

/***********************************
* Get num of sections of the menu  *
***********************************/
uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data)
{
  return 1;
}

/***********************************
* Get num of rows of the menu      *
***********************************/
uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data)
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
static void menu_draw_header(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *callback_context)
{
  GRect bounds = layer_get_bounds(cell_layer);
  //menu_cell_basic_draw(ctx, cell_layer, "", NULL, HowAreYouFeeling);
  graphics_context_set_fill_color(ctx, GColorWhite);
  //graphics_context_set_text_color(ctx, GColorBlack);
  // draw the box; 3rd and 4th variables: rounding the corners of the box
  // the watch has a displey of 200 p width
  graphics_fill_rect(ctx, GRect((bounds.size.w - 144) / 2, 4, 144, 40), 8, GCornersAll);
  graphics_draw_bitmap_in_rect(ctx, HowAreYouFeeling, bounds);
  // text in the box
  //graphics_draw_text(ctx, ("How are you feeling?"),fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),GRect(0, 0, bounds.size.w, 80), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

/***********************************
* Size of the menu header box      *
***********************************/
int16_t menu_header_height(struct MenuLayer *menu, uint16_t section_index, void *callback_context)
{
  return 40;
}

//void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
//  switch(section_index){
//  case 0:
//  menu_cell_basic_header_draw(ctx, cell_layer, "Moods");
//break;
//}
//}

//int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
//  return MENU_CELL_BASIC_HEADER_HEIGHT;
//}

/***********************************
* Draw rows of the menu            *
***********************************/
void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data)
{
  switch (cell_index->section)
  {
  case 0:
    switch (cell_index->row)
    {
    case 0:
      // NULL = Smily icon to input
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, Very_Happy);
      break;
    case 1:
      // NULL = Smily icon to input
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, Happy);
      break;
    case 2:
      // NULL = Smily icon to input
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, Normal);
      break;
    case 3:
      // NULL = Smily icon to input
      menu_cell_basic_draw(ctx, cell_layer, "", NULL, Unhappy);
      break;
    }

    break;
  }
}

/***********************************
* Detects when select button is hit*
* and saves the answer!            *
***********************************/
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{
  //setHappinessAnswer(0, (int)cell_index->row);
  if ((int)cell_index->row == 0)
  {
    window_stack_push(secondquestionA_window_get_window(), true);
  }
  if ((int)cell_index->row == 1)
  {
    window_stack_push(secondquestionB_window_get_window(), true);
  }
  if ((int)cell_index->row == 2)
  {
    window_stack_push(secondquestionC_window_get_window(), true);
  }
  if ((int)cell_index->row == 3)
  {
    window_stack_push(secondquestionD_window_get_window(), true);
  }
}

/***********************************
* Setup the menu layer             *
***********************************/
void setup_menu_layer(Window *window)
{
  Layer *window_layer = window_get_root_layer(window);

  GRect bounds = layer_get_bounds(window_layer);

  firstquestionMenuLayer = menu_layer_create(bounds);
  menu_layer_set_normal_colors(firstquestionMenuLayer, GColorWhite, GColorWhite);
  menu_layer_set_highlight_colors(firstquestionMenuLayer, GColorLightGray, GColorBlack);

  menu_layer_set_callbacks(firstquestionMenuLayer, NULL, (MenuLayerCallbacks){
                                                             .get_num_sections = menu_get_num_sections_callback,
                                                             .get_num_rows = menu_get_num_rows_callback,
                                                             .get_header_height = menu_header_height,
                                                             .draw_header = menu_draw_header,
                                                             .draw_row = menu_draw_row_callback,
                                                             .select_click = menu_select_callback});

  menu_layer_set_click_config_onto_window(firstquestionMenuLayer, window);

  //menu_layer_set_center_focused(mainMenuLayer, true);

  // overwrite default setting for back button
  force_back_button(window, firstquestionMenuLayer);

  layer_add_child(window_layer, menu_layer_get_layer(firstquestionMenuLayer));
}

/***********************************
* Load event of the main window    *
***********************************/
void firstquestion_window_load(Window *window)
{
  setup_menu_layer(window);

#if defined(PBL_PLATFORM_EMERY)
  Very_Happy = gbitmap_create_with_resource(RESOURCE_ID_Very_Happy_Smiley_Emery);
#else
  Very_Happy = gbitmap_create_with_resource(RESOURCE_ID_Very_Happy_Smiley_Time);
#endif

#if defined(PBL_PLATFORM_EMERY)
  Happy = gbitmap_create_with_resource(RESOURCE_ID_Happy_Smiley_Emery);
#else
  Happy = gbitmap_create_with_resource(RESOURCE_ID_Happy_Smiley_Time);
#endif

#if defined(PBL_PLATFORM_EMERY)
  Normal = gbitmap_create_with_resource(RESOURCE_ID_Normal_Smiley_Emery);
#else
  Normal = gbitmap_create_with_resource(RESOURCE_ID_Normal_Smiley_Time);
#endif

#if defined(PBL_PLATFORM_EMERY)
  Unhappy = gbitmap_create_with_resource(RESOURCE_ID_Unhappy_Smiley_Emery);
#else
  Unhappy = gbitmap_create_with_resource(RESOURCE_ID_Unhappy_Smiley_Time);
#endif

#if defined(PBL_PLATFORM_EMERY)
  Very_Unhappy = gbitmap_create_with_resource(RESOURCE_ID_Very_Unhappy_Smiley_Emery);
#else
  Very_Unhappy = gbitmap_create_with_resource(RESOURCE_ID_Very_Unhappy_Smiley_Time);
#endif

#if defined(PBL_PLATFORM_EMERY)
  HowAreYouFeeling = gbitmap_create_with_resource(RESOURCE_ID_HowAreYouFeelingBolt_Emery);
#else
  HowAreYouFeeling = gbitmap_create_with_resource(RESOURCE_ID_HowAreYouFeelingBolt_Time);
#endif
}

/***********************************
* Unload event of the window       *
***********************************/
void firstquestion_window_unload(Window *window)
{
  gbitmap_destroy(Very_Happy);
  gbitmap_destroy(Happy);
  gbitmap_destroy(Normal);
  gbitmap_destroy(Unhappy);
  gbitmap_destroy(Very_Unhappy);
}

/***********************************
* Appear event of the window       *
***********************************/
void firstquestion_window_appear(Window *window)
{
  setNumberOfWindow(0);
}

/***********************************
* Init the window                  *
***********************************/
void init_firstquestion_window()
{
  firstquestionWindow = window_create();
  window_set_window_handlers(firstquestionWindow, (WindowHandlers){.load = firstquestion_window_load, .unload = firstquestion_window_unload, .appear = firstquestion_window_appear});
}

/***********************************
* Deinit the window                *
***********************************/
void deinit_firstquestion_window()
{
  menu_layer_destroy(firstquestionMenuLayer);
  window_destroy(firstquestionWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *firstquestion_window_get_window()
{
  return firstquestionWindow;
}