#include <pebble.h>
#include "mouse_view.h"

static BitmapLayer *mouse_view_layer;
static GBitmap *mouse_bitmap;

void mouse_view_init(Layer *root_layer) {
  GRect root_frame = layer_get_frame(root_layer);
  int16_t x = (root_frame.size.w - MOUSE_WIDTH) / 2;
  int16_t y = (root_frame.size.h - MOUSE_HEIGHT) / 2;
  GRect mouse_view_frame = GRect(x, y, MOUSE_WIDTH, MOUSE_HEIGHT);
  mouse_view_layer = bitmap_layer_create(mouse_view_frame);

  mouse_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MOUSE);
  bitmap_layer_set_bitmap(mouse_view_layer, mouse_bitmap);

  layer_add_child(root_layer, bitmap_layer_get_layer(mouse_view_layer));
}

void mouse_view_move_by(GPoint translation) {
  GRect frame = mouse_view_location();

  frame.origin.x = frame.origin.x + translation.x;
  if (frame.origin.x > 144 - MOUSE_WIDTH) frame.origin.x = 144 - MOUSE_WIDTH;
  if (frame.origin.x < 0) frame.origin.x = 0;

  frame.origin.y = frame.origin.y + translation.y;
  if (frame.origin.y > 168 - MOUSE_HEIGHT) frame.origin.y = 168 - MOUSE_HEIGHT;
  if (frame.origin.y < 0) frame.origin.y = 0;

  layer_set_frame(bitmap_layer_get_layer(mouse_view_layer), frame);
}

GRect mouse_view_location(void) {
  return layer_get_frame(bitmap_layer_get_layer(mouse_view_layer));
}

void mouse_view_deinit(void) {
  gbitmap_destroy(mouse_bitmap);
  bitmap_layer_destroy(mouse_view_layer);
}
