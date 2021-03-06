#include <pebble.h>

#include "aeropress_timer.h"
#include "windows_include/championship_menu.h"
#include "windows_include/championship_run.h"


#define NUM_WINDOWS 9

static Window *s_main_window;
static MenuLayer *s_menu_layer;
static TextLayer *s_text_layer;

static GBitmap *s_icon_bitmap_menu_1;
static GBitmap *s_icon_bitmap_menu_2;
static GBitmap *s_icon_bitmap_menu_3;
static GBitmap *s_icon_bitmap_menu_4;
static GBitmap *s_icon_bitmap_menu_5;
static GBitmap *s_icon_bitmap_menu_6;
static GBitmap *s_icon_bitmap_menu_7;
static GBitmap *s_icon_bitmap_menu_8;
static GBitmap *s_icon_bitmap_menu_9;

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return NUM_WINDOWS;
}

static void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, "1st Place", NULL, s_icon_bitmap_menu_1);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, "2nd Place", NULL, s_icon_bitmap_menu_2);
      break;
		case 2:
      menu_cell_basic_draw(ctx, cell_layer, "3rd Place", NULL, s_icon_bitmap_menu_3);
      break;
		case 3:
      menu_cell_basic_draw(ctx, cell_layer, "United Kingdom", NULL, s_icon_bitmap_menu_4);
      break;
		case 4:
      menu_cell_basic_draw(ctx, cell_layer, "Japan", NULL, s_icon_bitmap_menu_5);
      break;
		case 5:
      menu_cell_basic_draw(ctx, cell_layer, "Australia", NULL, s_icon_bitmap_menu_6);
      break;
		case 6:
      menu_cell_basic_draw(ctx, cell_layer, "United States", NULL, s_icon_bitmap_menu_7);
      break;
		case 7:
      menu_cell_basic_draw(ctx, cell_layer, "Norway", NULL, s_icon_bitmap_menu_8);
      break;
		case 8:
      menu_cell_basic_draw(ctx, cell_layer, "Italy", NULL, s_icon_bitmap_menu_9);
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
      championship_run(0);
      break;
    case 1:
      championship_run(1);
      break;
		case 2:
      championship_run(2);
      break;
		case 3:
      championship_run(3);
      break;
		case 4:
      championship_run(4);
      break;
		case 5:
      championship_run(5);
      break;
		case 6:
      championship_run(6);
      break;
		case 7:
      championship_run(7);
      break;
		case 8:
      championship_run(8);
      break;
    default:
      break;
  }
}

static void draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *context) {
  //menu_cell_basic_header_draw(ctx, cell_layer, "Standard Recipes");
	text_layer_set_text_color(s_text_layer, HighlightTextColor);
  text_layer_set_background_color(s_text_layer, ForeGroundColor);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
	text_layer_set_text(s_text_layer, "CHAMPIONSHIP");
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

	s_icon_bitmap_menu_1 = gbitmap_create_with_resource(RESOURCE_ID_SUB_MENU_1);
	s_icon_bitmap_menu_2 = gbitmap_create_with_resource(RESOURCE_ID_SUB_MENU_2);
	s_icon_bitmap_menu_3 = gbitmap_create_with_resource(RESOURCE_ID_SUB_MENU_3);
	s_icon_bitmap_menu_4 = gbitmap_create_with_resource(RESOURCE_ID_UK);
	s_icon_bitmap_menu_5 = gbitmap_create_with_resource(RESOURCE_ID_JP);
	s_icon_bitmap_menu_6 = gbitmap_create_with_resource(RESOURCE_ID_AU);
	s_icon_bitmap_menu_7 = gbitmap_create_with_resource(RESOURCE_ID_US);
	s_icon_bitmap_menu_8 = gbitmap_create_with_resource(RESOURCE_ID_NW);
	s_icon_bitmap_menu_9 = gbitmap_create_with_resource(RESOURCE_ID_IT);

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

	menu_layer_set_normal_colors(s_menu_layer, BackGroundColor, NormalTextColor);
	menu_layer_set_highlight_colors(s_menu_layer, ForeGroundColor, HighlightTextColor);
	menu_layer_pad_bottom_enable(s_menu_layer, true);

	s_text_layer = text_layer_create(GRect(0, 0, bounds.size.w, 16));
	layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
	text_layer_destroy(s_text_layer);
	gbitmap_destroy(s_icon_bitmap_menu_1);
	gbitmap_destroy(s_icon_bitmap_menu_2);
	gbitmap_destroy(s_icon_bitmap_menu_3);
	gbitmap_destroy(s_icon_bitmap_menu_4);
	gbitmap_destroy(s_icon_bitmap_menu_5);
	gbitmap_destroy(s_icon_bitmap_menu_6);
	gbitmap_destroy(s_icon_bitmap_menu_7);
	gbitmap_destroy(s_icon_bitmap_menu_8);
	gbitmap_destroy(s_icon_bitmap_menu_9);
}

void championship_menu_push() {
	if(!s_main_window) {
    s_main_window = window_create();
    window_set_background_color(s_main_window, BackGroundColor);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}

