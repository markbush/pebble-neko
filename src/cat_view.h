#pragma once
#include <pebble.h>

#define CAT_WIDTH 32
#define CAT_HEIGHT 32

void cat_view_init(Layer *root_layer);
void cat_view_move_by(GPoint translation);
GRect cat_view_location(void);
void cat_view_deinit(void);
