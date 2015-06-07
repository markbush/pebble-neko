#pragma once
#include <pebble.h>

#define MOUSE_WIDTH 17
#define MOUSE_HEIGHT 12

void mouse_view_init(Layer *root_layer);
void mouse_view_move_by(GPoint translation);
GRect mouse_view_location(void);
void mouse_view_deinit(void);
