#include <pebble.h>

#include "windows_include/two_cup_menu.h"

#define NUM_WINDOWS 2
#define CHECKBOX_WINDOW_CELL_HEIGHT 30

static Window *s_main_window;
static MenuLayer *s_menu_layer;

static GBitmap *s_icon_bitmap_menu;

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return NUM_WINDOWS;
}

static void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, "1", NULL, s_icon_bitmap_menu);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, "2", NULL, NULL);
      break;
    default:
      break;
  }
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return CHECKBOX_WINDOW_CELL_HEIGHT;
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
      //dialog_choice_window_push();
      break;
    case 1:
      //dialog_choice_window_push();
      break;
    case 2:
    default:
      break;
  }
}

static void draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *context) {
  menu_cell_basic_header_draw(ctx, cell_layer, "Two Cup Recipes");
}

static int16_t get_header_height_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static uint16_t get_num_sections_callback(struct MenuLayer *menu_layer, void *context) {
  return 1;
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

	s_icon_bitmap_menu = gbitmap_create_with_resource(RESOURCE_ID_TICK);

  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback)get_num_rows_callback,
      .draw_row = (MenuLayerDrawRowCallback)draw_row_callback,
      .get_cell_height = (MenuLayerGetCellHeightCallback)get_cell_height_callback,
      .select_click = (MenuLayerSelectCallback)select_callback,
      .draw_header = (MenuLayerDrawHeaderCallback)draw_header_callback,
      .get_header_height = (MenuLayerGetHeaderHeightCallback)get_header_height_callback,
      .get_num_sections = (MenuLayerGetNumberOfSectionsCallback)get_num_sections_callback,
  });
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));

	menu_layer_set_normal_colors(s_menu_layer, GColorTiffanyBlue, GColorPastelYellow);
	menu_layer_set_highlight_colors(s_menu_layer, GColorMidnightGreen, GColorWhite);
	menu_layer_pad_bottom_enable(s_menu_layer, true);
}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
}

void two_cup_menu_push() {
	if(!s_main_window) {
    s_main_window = window_create();
    window_set_background_color(s_main_window, GColorJaegerGreen);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}

