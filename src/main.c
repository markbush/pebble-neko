#include <pebble.h>

extern void setup_main_window(void);
extern void teardown_main_window(void);

static void setup_app(void);
static void teardown_app(void);

int main(void) {
  setup_app();
  app_event_loop();
  teardown_app();
}

static void setup_app(void) {
  setup_main_window();
}

static void teardown_app(void) {
  teardown_main_window();
}
