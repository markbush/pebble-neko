#include <pebble.h>
#include "neko_controller.h"
#include "mouse_controller.h"
#include "cat_controller.h"
#include "mouse_view.h"

static void accel_data_handler(AccelData *data, uint32_t num_samples);

void neko_controller_init(Layer *root_layer) {
  cat_controller_init(root_layer);
  mouse_controller_init(root_layer);

  accel_data_service_subscribe(1, accel_data_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
}

static void accel_data_handler(AccelData *data, uint32_t num_samples) {
  static int move = 0;

  if (num_samples > 0) {
    int16_t x_accel = data[0].x / 50;
    int16_t y_accel = data[0].y / 50;
    mouse_view_move_by(GPoint(x_accel, -y_accel));
  }

  move = (move + 1) % 5;
  if (move == 0) {
    cat_controller_move();
  }
}

void neko_controller_deinit(void) {
  accel_data_service_unsubscribe();
  cat_controller_deinit();
  mouse_controller_deinit();
}
