#include <pebble.h>
#include "enums.h"
#include "constants.h"
#include "paths.h"

static Window *splashWindow;
// static GBitmap *splashLogo;

static AppSync sync;
static uint8_t syncBuffer[32];

static void splashScreenLayerUpdateProc(struct Layer *layer, GContext *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Drawing splash screen");
  GRect bounds = layer_get_bounds(layer);

  graphics_context_set_text_color(context, GColorWhite);
  graphics_context_set_fill_color(context, BRAND_BLUE);
  graphics_fill_rect(context, bounds, 0, 0);

  GRect titleBox = {
    .origin = (GPoint){.x = 0, .y = 130},
    .size = (GSize){ .w = bounds.size.w, .h = 28}
  };

  graphics_draw_text(context, "Standup", fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), titleBox, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  graphics_context_set_stroke_width(context, 10);
  graphics_context_set_stroke_color(context, GColorWhite);
  graphics_draw_circle(context, (GPoint){.x=bounds.size.w/2, .y=(bounds.size.h/2)-15}, 55);

  graphics_context_set_fill_color(context, GColorWhite);
  GPath *cardPath = gpath_create(&CARD_PATH_INFO);

  for(int i = 0; i < 6; i++) {
    gpath_move_to(cardPath, (GPoint){.x=bounds.size.w/2, .y=(bounds.size.h/2)-15});
    gpath_rotate_to(cardPath, i*(TRIG_MAX_ANGLE/6));
    gpath_draw_filled(context, cardPath);
  }

  gpath_destroy(cardPath);
}

static void splashScreenLoad(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  layer_set_update_proc(window_layer, splashScreenLayerUpdateProc);
}

static void syncChangedHandler(const uint32_t key, const Tuple *newTuple, const Tuple *oldTuple, void *context) {
  // update UI here
  // a change here means the XHR has resolved. Build the menu using the dictionary!
}

static void syncErrorHandler(DictionaryResult dictError, AppMessageResult appMessageError, void *context) {
  // An error occured!
  // Display an error page?
}

static void init(void) {
  splashWindow = window_create();
  // window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(splashWindow, (WindowHandlers) {
    .load = splashScreenLoad,
    .unload = NULL
  });
  const bool animated = true;
  window_stack_push(splashWindow, animated);

  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  Tuplet initialValues[] = {
    // TupletInteger(KEY_COUNT, 0),
  };

  app_sync_init(&sync, syncBuffer, sizeof(syncBuffer), initialValues, ARRAY_LENGTH(initialValues), syncChangedHandler, syncErrorHandler, NULL);
}

static void deinit(void) {
  window_destroy(splashWindow);
  app_sync_deinit(&sync);
}

int main(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Initializing app.");

  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, entering event loop...");

  app_event_loop();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Exiting app.");
  deinit();
}
