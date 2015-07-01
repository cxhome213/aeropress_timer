#include <pebble.h>

#include "windows_include/the_classic_run.h"

static Window *s_main_window;
static TextLayer *s_text_layer;

void the_classic_run_click_handler(ClickRecognizerRef recognizer, void *context) {
  the_classic_run();
}

void config_provider(Window *window) {
 // single click / repeat-on-hold config:
  window_single_repeating_click_subscribe(BUTTON_ID_SELECT, 1000, the_classic_run_click_handler);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_text_layer = text_layer_create(GRect(0, 5, bounds.size.w, bounds.size.h));
  text_layer_set_text_color(s_text_layer, GColorWhite);
  text_layer_set_background_color(s_text_layer, GColorClear);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_text(s_text_layer, "BREWING METHOD\nTraditional\n\nCOFFEE VOLUME\n2  Scoops\n\nCOFFEE GRIND\nMedium Fine\n\nWater Volume\n2  Shots");

  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_text_layer);
}

void the_classic_start() {
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorTiffanyBlue);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);

	window_set_click_config_provider(s_main_window, (ClickConfigProvider) config_provider);
}
