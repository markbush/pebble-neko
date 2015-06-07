#include <pebble.h>
#include "mouse_controller.h"
#include "mouse_view.h"

void mouse_controller_init(Layer *root_layer) {
  mouse_view_init(root_layer);
}

void mouse_controller_deinit(void) {
  mouse_view_deinit();
}
