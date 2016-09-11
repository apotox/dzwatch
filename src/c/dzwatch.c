#include <pebble.h>
#include "dzwatch.h"

static Window *s_main_window;
static TextLayer *s_time_layer;
static GBitmap *s_bitmap;
static GFont s_font;
static BitmapLayer *s_bitmap_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
   update_time();
}


static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52)+40, bounds.size.w, 60));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  
  
  
  s_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_arcade_40));
 // text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
   text_layer_set_font(s_time_layer, s_font);
  
  
  
  
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  
  
  
  
  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_safi);
  s_bitmap_layer = bitmap_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
  
  layer_add_child(window_layer,bitmap_layer_get_layer(s_bitmap_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  gbitmap_destroy(s_bitmap);
  bitmap_layer_destroy(s_bitmap_layer);
}


static void init() {
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  
  
  

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  update_time();
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  window_destroy(s_main_window);
}



int main(void) {
  init();
  app_event_loop();
  deinit();
}