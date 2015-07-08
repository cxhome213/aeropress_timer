#include <pebble.h>
#include <main.h>
#include "windows_include/standard_menu_sub/the_classic_run.h"
#include "aeropress_timer.h"

#define PROCESS_NUM 4

const int process_time[PROCESS_NUM] = {10, 20, 15, 15};
const char *process_buf[PROCESS_NUM] = { "Pour\n2 Shots",
																				"\nStir",
																				"\nSteep",
																				"\nPlunge"
																			};

static char num_buf[10];
static volatile int count;
static volatile int num;
static volatile int pre_count;

static Window *s_main_window;
static TextLayer *s_num_text_layer;
static TextLayer *s_process_text_layer;
static TextLayer *s_pre_text_layer;
static Layer *s_canvas_layer;

static GPoint s_circle;
static GPoint s_num_text;
static GPoint s_process_text;

int flag_classic_run_click, flag_text_layer_destroy = 0;

void the_classic_run_click_handler(ClickRecognizerRef recognizer, void *context) {
  main_menu_init();
}

/*
void the_classic_run_config_provider(Window *window) {
 // single click / repeat-on-hold config:
  window_single_repeating_click_subscribe(BUTTON_ID_SELECT, 1000, the_classic_run_click_handler);
}*/

static void tick_handler(struct tm *tick_time, TimeUnits changed) {
  // Redraw
  if(s_canvas_layer) {
    layer_mark_dirty(s_canvas_layer);
  }
}

static void update_proc(Layer *layer, GContext *ctx) {

	graphics_context_set_stroke_color(ctx, NormalTextColor);
	graphics_context_set_stroke_width(ctx, 4);

	text_layer_set_text_color(s_num_text_layer, NormalTextColor);
	text_layer_set_background_color(s_num_text_layer, GColorClear);
	text_layer_set_font(s_num_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
	text_layer_set_text_alignment(s_num_text_layer, GTextAlignmentCenter);

	text_layer_set_text_color(s_process_text_layer, NormalTextColor);
	text_layer_set_background_color(s_process_text_layer, GColorClear);
	text_layer_set_font(s_process_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(s_process_text_layer, GTextAlignmentCenter);

	text_layer_set_text_color(s_pre_text_layer, NormalTextColor);
  text_layer_set_background_color(s_pre_text_layer, GColorClear);
  text_layer_set_font(s_pre_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_pre_text_layer, GTextAlignmentCenter);

	if(pre_count < PRE_COUNT_INFO)
	{
		text_layer_set_text(s_pre_text_layer, "BREWING METHOD\nTraditional\n\nCOFFEE VOLUME\n2  Scoops\n\nCOFFEE GRIND\nMedium Fine\n\nWater Volume\n2  Shots");
		
		pre_count++;

		return;
	}

	if(pre_count < PRE_COUNT_READY)
	{
		if(flag_text_layer_destroy == 0)
		{
			flag_text_layer_destroy = 1;
			text_layer_destroy(s_pre_text_layer);
		}
		text_layer_set_font(s_num_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
		text_layer_set_text(s_num_text_layer, "Prepare!\n\nTo Pour");
		
		pre_count++;

		return;
	}

	if(count  == -1)
	{
		if(++num < PROCESS_NUM)
		{
			count = process_time[num];
		}
	}

	if(num < PROCESS_NUM)
	{
		graphics_draw_circle(ctx, s_circle, 40);

		memset(num_buf, '\0', 10);
		snprintf(num_buf, sizeof(num_buf), "%d", count--);
		text_layer_set_text(s_num_text_layer, num_buf);
		text_layer_set_text(s_process_text_layer, process_buf[num]);

		if(count == -1)
			vibes_short_pulse();
	}
	else
	{
		memset(num_buf, '\0', 10);
		text_layer_set_text(s_process_text_layer, num_buf);

		text_layer_set_font(s_num_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
		text_layer_set_text(s_num_text_layer, "Enjoy!\n\nFresh Coffee!");
		
		if(flag_classic_run_click)
		{
			vibes_long_pulse();
			//window_set_click_config_provider(s_main_window, (ClickConfigProvider) the_classic_run_config_provider);
			flag_classic_run_click = 0;
		}
	}
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

	s_circle = grect_center_point(&bounds);
	s_circle.y = s_circle.y - 20;

	s_num_text = grect_center_point(&bounds);
	s_num_text.y = s_num_text.y - 50;

	s_process_text = grect_center_point(&bounds);
	s_process_text.y = s_process_text.y + 20;

  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, update_proc);
  layer_add_child(window_layer, s_canvas_layer);

	s_pre_text_layer = text_layer_create(GRect(0, 5, bounds.size.w, bounds.size.h));
	layer_add_child(s_canvas_layer, text_layer_get_layer(s_pre_text_layer));

	s_num_text_layer = text_layer_create(GRect(0, s_num_text.y, bounds.size.w, bounds.size.h));
	layer_add_child(s_canvas_layer, text_layer_get_layer(s_num_text_layer));

	s_process_text_layer = text_layer_create(GRect(0, s_process_text.y, bounds.size.w, bounds.size.h));
	layer_add_child(s_canvas_layer, text_layer_get_layer(s_process_text_layer));
}

static void main_window_unload(Window *window) {
	window_set_click_config_provider(s_main_window, NULL);
	text_layer_destroy(s_process_text_layer);
  text_layer_destroy(s_num_text_layer);
	layer_destroy(s_canvas_layer);
	tick_timer_service_unsubscribe();
}

void the_classic_run() {
	if(!s_main_window) {
    s_main_window = window_create();
    window_set_background_color(s_main_window, BackGroundColor);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload,
    });
  }
  window_stack_push(s_main_window, true);

	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

	flag_classic_run_click = 1;
	count = -1;
	num = -1;
	pre_count = 0;
}
