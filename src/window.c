#include <pebble.h>
#include "neko_controller.h"

static void main_window_load(Window *window);
static void main_window_unload(Window *window);

static Window *s_main_window;

void setup_main_window(void) {
  s_main_window = window_create();
#ifdef PBL_PLATFORM_APLITE
  window_set_fullscreen(s_main_window, true);
#endif

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);
}

void teardown_main_window(void) {
  window_destroy(s_main_window);
}

static void main_window_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);

  neko_controller_init(root_layer);
}

static void main_window_unload(Window *window) {
  neko_controller_deinit();
}
