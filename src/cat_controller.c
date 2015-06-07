#include <pebble.h>
#include "cat_controller.h"
#include "mouse_view.h"
#include "cat_view.h"

static int16_t cat_speed = 10;

void cat_controller_init(Layer *root_layer) {
  cat_view_init(root_layer);
}

void cat_controller_move(void) {
  GPoint cat_location = cat_view_location().origin;
  GPoint mouse_location = mouse_view_location().origin;

  int16_t delta_x = mouse_location.x - cat_location.x;
  if (delta_x != 0 && abs(delta_x) > cat_speed) {
    delta_x = delta_x * cat_speed / abs(delta_x);
  }
  int16_t delta_y = mouse_location.y - cat_location.y;
  if (delta_y != 0 && abs(delta_y) > cat_speed) {
    delta_y = delta_y * cat_speed / abs(delta_y);
  }

  cat_view_move_by(GPoint(delta_x, delta_y));
}

void cat_controller_deinit(void) {
  cat_view_deinit();
}
