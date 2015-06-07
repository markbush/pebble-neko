#include <pebble.h>
#include "cat_view.h"
#include "mouse_view.h"

static BitmapLayer *cat_view_layer;
static GBitmap *cat_bitmap;

static int image_step = 0;
typedef enum {
  CAT_ASLEEP,
  CAT_AWAKE,
  CAT_RUN_E,
  CAT_RUN_SE,
  CAT_RUN_S,
  CAT_RUN_SW,
  CAT_RUN_W,
  CAT_RUN_NW,
  CAT_RUN_N,
  CAT_RUN_NE,
  CAT_PAUSE,
  CAT_SCRATCH_WALL_E,
  CAT_SCRATCH_WALL_SE,
  CAT_SCRATCH_WALL_S,
  CAT_SCRATCH_WALL_SW,
  CAT_SCRATCH_WALL_W,
  CAT_SCRATCH_WALL_NW,
  CAT_SCRATCH_WALL_N,
  CAT_SCRATCH_WALL_NE,
  CAT_YAWN,
  CAT_LICK,
  CAT_SCRATCH_SELF,
  NUM_CAT_STATES
} CatState;
static CatState cat_state = CAT_ASLEEP;
static CatState lastRunDirection = CAT_RUN_S;
static uint32_t cat_resource[NUM_CAT_STATES][2] = {
  {RESOURCE_ID_IMAGE_SLEEP1, RESOURCE_ID_IMAGE_SLEEP2},
  {RESOURCE_ID_IMAGE_AWAKE1, RESOURCE_ID_IMAGE_AWAKE2},
  {RESOURCE_ID_IMAGE_RUN_E1, RESOURCE_ID_IMAGE_RUN_E2},
  {RESOURCE_ID_IMAGE_RUN_SE1, RESOURCE_ID_IMAGE_RUN_SE2},
  {RESOURCE_ID_IMAGE_RUN_S1, RESOURCE_ID_IMAGE_RUN_S2},
  {RESOURCE_ID_IMAGE_RUN_SW1, RESOURCE_ID_IMAGE_RUN_SW2},
  {RESOURCE_ID_IMAGE_RUN_W1, RESOURCE_ID_IMAGE_RUN_W2},
  {RESOURCE_ID_IMAGE_RUN_NW1, RESOURCE_ID_IMAGE_RUN_NW2},
  {RESOURCE_ID_IMAGE_RUN_N1, RESOURCE_ID_IMAGE_RUN_N2},
  {RESOURCE_ID_IMAGE_RUN_NE1, RESOURCE_ID_IMAGE_RUN_NE2},
  {RESOURCE_ID_IMAGE_AWAKE2, RESOURCE_ID_IMAGE_AWAKE2},
  {RESOURCE_ID_IMAGE_SCRATCH_WALL_E1, RESOURCE_ID_IMAGE_SCRATCH_WALL_E2},
  {RESOURCE_ID_IMAGE_SCRATCH_WALL_E1, RESOURCE_ID_IMAGE_SCRATCH_WALL_E2},
  {RESOURCE_ID_IMAGE_SCRATCH_WALL_S1, RESOURCE_ID_IMAGE_SCRATCH_WALL_S2},
  {RESOURCE_ID_IMAGE_SCRATCH_WALL_S1, RESOURCE_ID_IMAGE_SCRATCH_WALL_S2},
  {RESOURCE_ID_IMAGE_SCRATCH_WALL_W1, RESOURCE_ID_IMAGE_SCRATCH_WALL_W2},
  {RESOURCE_ID_IMAGE_SCRATCH_WALL_W1, RESOURCE_ID_IMAGE_SCRATCH_WALL_W2},
  {RESOURCE_ID_IMAGE_SCRATCH_WALL_N1, RESOURCE_ID_IMAGE_SCRATCH_WALL_N2},
  {RESOURCE_ID_IMAGE_SCRATCH_WALL_N1, RESOURCE_ID_IMAGE_SCRATCH_WALL_N2},
  {RESOURCE_ID_IMAGE_YAWN, RESOURCE_ID_IMAGE_AWAKE2},
  {RESOURCE_ID_IMAGE_LICK, RESOURCE_ID_IMAGE_AWAKE2},
  {RESOURCE_ID_IMAGE_SCRATCH_SELF1, RESOURCE_ID_IMAGE_SCRATCH_SELF2}
};


static void cat_view_update_state(GPoint movement);
static CatState cat_run_direction(GPoint movement);

void cat_view_init(Layer *root_layer) {
  GRect root_frame = layer_get_frame(root_layer);
  int16_t x = (root_frame.size.w - CAT_WIDTH) / 2;
  int16_t y = ((root_frame.size.h - MOUSE_HEIGHT) / 2) - CAT_HEIGHT;
  GRect cat_view_frame = GRect(x, y, CAT_WIDTH, CAT_HEIGHT);
  cat_view_layer = bitmap_layer_create(cat_view_frame);

  cat_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SLEEP1);
  bitmap_layer_set_bitmap(cat_view_layer, cat_bitmap);

  layer_add_child(root_layer, bitmap_layer_get_layer(cat_view_layer));
}

void cat_view_move_by(GPoint translation) {
  GRect location = cat_view_location();
  GRect frame = GRect(0, 0, location.size.w, location.size.h);

  frame.origin.x = location.origin.x + translation.x;
  if (frame.origin.x > 144 - CAT_WIDTH) frame.origin.x = 144 - CAT_WIDTH;
  if (frame.origin.x < 0) frame.origin.x = 0;

  frame.origin.y = location.origin.y + translation.y;
  if (frame.origin.y > 168 - CAT_HEIGHT) frame.origin.y = 168 - CAT_HEIGHT;
  if (frame.origin.y < 0) frame.origin.y = 0;

  GPoint movement = GPoint(frame.origin.x-location.origin.x, frame.origin.y-location.origin.y);
  cat_view_update_state(movement);
  uint32_t resource = cat_resource[cat_state][image_step];
  gbitmap_destroy(cat_bitmap);
  cat_bitmap = gbitmap_create_with_resource(resource);
  bitmap_layer_set_bitmap(cat_view_layer, cat_bitmap);
  layer_set_frame(bitmap_layer_get_layer(cat_view_layer), frame);
  layer_mark_dirty(bitmap_layer_get_layer(cat_view_layer));
}

GRect cat_view_location(void) {
  return layer_get_frame(bitmap_layer_get_layer(cat_view_layer));
}

static void cat_view_update_state(GPoint movement) {
  static int repeat = 0;
  image_step = (image_step + 1) % 2;

  switch (cat_state) {
    case CAT_ASLEEP:
      if (movement.x != 0 || movement.y != 0) {
        cat_state = CAT_AWAKE;
        image_step = 0;
      }
      break;
    case CAT_AWAKE:
      if (image_step == 0) {
        cat_state = cat_run_direction(movement);
      }
      break;
    case CAT_RUN_E:
    case CAT_RUN_SE:
    case CAT_RUN_S:
    case CAT_RUN_SW:
    case CAT_RUN_W:
    case CAT_RUN_NW:
    case CAT_RUN_N:
    case CAT_RUN_NE:
      if (movement.x == 0 && movement.y == 0) {
        cat_state = CAT_PAUSE;
        image_step = 0;
      } else {
        cat_state = cat_run_direction(movement);
      }
      break;
    case CAT_PAUSE:
      if (image_step == 0) {
        if (movement.x != 0 || movement.y != 0) {
          cat_state = CAT_AWAKE;
          image_step = 0;
        } else {
          cat_state = lastRunDirection - CAT_RUN_E + CAT_SCRATCH_WALL_E;
          repeat = 0;
        }
      }
      break;
    case CAT_SCRATCH_WALL_E:
    case CAT_SCRATCH_WALL_SE:
    case CAT_SCRATCH_WALL_S:
    case CAT_SCRATCH_WALL_SW:
    case CAT_SCRATCH_WALL_W:
    case CAT_SCRATCH_WALL_NW:
    case CAT_SCRATCH_WALL_N:
    case CAT_SCRATCH_WALL_NE:
      if (movement.x != 0 || movement.y != 0) {
        cat_state = CAT_AWAKE;
        image_step = 0;
      } else {
        if (repeat == 0) {
          cat_state = CAT_YAWN;
          image_step = 0;
        }
      }
      break;
    case CAT_YAWN:
      if (image_step == 0) {
        if (movement.x != 0 || movement.y != 0) {
          cat_state = CAT_AWAKE;
          image_step = 0;
        } else {
          cat_state = CAT_SCRATCH_SELF;
          image_step = 0;
          repeat = 0;
        }
      }
      break;
    case CAT_SCRATCH_SELF:
      if (movement.x != 0 || movement.y != 0) {
        cat_state = CAT_AWAKE;
        image_step = 0;
      } else {
        if (repeat == 0) {
          cat_state = CAT_ASLEEP;
          image_step = 0;
        }
      }
      break;
    default:
      cat_state = CAT_ASLEEP;
  }
  repeat = (repeat + 1) % 4;
}

static CatState cat_run_direction(GPoint movement) {
  CatState result = CAT_RUN_E;
  if (movement.y == 0) {
    if (movement.x > 0) {
      result = CAT_RUN_E;
    } else {
      result = CAT_RUN_W;
    }
  } else {
    double ratio = 1.0 * movement.x / movement.y;
    if (ratio >= -2.0 && ratio < -0.5) {
      if (movement.x > 0) {
        result = CAT_RUN_NE;
      } else {
        result = CAT_RUN_SW;
      }
    } else {
      if (ratio >= -0.5 && ratio < 0.5) {
        if (movement.y > 0) {
          result = CAT_RUN_S;
        } else {
          result = CAT_RUN_N;
        }
      } else {
        if (ratio >= 0.5 && ratio < 2) {
          if (movement.x > 0) {
            result = CAT_RUN_SE;
          } else {
            result = CAT_RUN_NW;
          }
        } else {
          if (movement.x > 0) {
            result = CAT_RUN_E;
          } else {
            result = CAT_RUN_W;
          }
        }
      }
    }
  }
  lastRunDirection = result;
  return result;
}

void cat_view_deinit(void) {
  gbitmap_destroy(cat_bitmap);
  bitmap_layer_destroy(cat_view_layer);
}
