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
  // graphics_draw_text(context, "Standup", fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  graphics_fill_rect(context, bounds, 0, 0);
  graphics_context_set_stroke_width(context, 9);
  graphics_context_set_stroke_color(context, GColorWhite);
  graphics_draw_circle(context, (GPoint){.x=bounds.size.w/2, .y=bounds.size.h/2}, bounds.size.w*0.4);

  graphics_context_set_fill_color(context, GColorWhite);
  GPath *cardPath = gpath_create(&CARD_PATH_INFO);
  gpath_move_to(cardPath, (GPoint){.x=bounds.size.w/2, .y=bounds.size.h/2});
  gpath_draw_filled(context, cardPath);

  gpath_move_to(cardPath, (GPoint){.x=bounds.size.w/2, .y=bounds.size.h/2});
  gpath_rotate_to(cardPath, TRIG_MAX_ANGLE/6);
  gpath_draw_filled(context, cardPath);

  gpath_move_to(cardPath, (GPoint){.x=bounds.size.w/2, .y=bounds.size.h/2});
  gpath_rotate_to(cardPath, 2*(TRIG_MAX_ANGLE/6));
  gpath_draw_filled(context, cardPath);

  gpath_move_to(cardPath, (GPoint){.x=bounds.size.w/2, .y=bounds.size.h/2});
  gpath_rotate_to(cardPath, 3*(TRIG_MAX_ANGLE/6));
  gpath_draw_filled(context, cardPath);
  gpath_move_to(cardPath, (GPoint){.x=bounds.size.w/2, .y=bounds.size.h/2});
  gpath_rotate_to(cardPath, 4*(TRIG_MAX_ANGLE/6));
  gpath_draw_filled(context, cardPath);
  gpath_move_to(cardPath, (GPoint){.x=bounds.size.w/2, .y=bounds.size.h/2});
  gpath_rotate_to(cardPath, 5*(TRIG_MAX_ANGLE/6));
  gpath_draw_filled(context, cardPath);
}

static void splashScreenLoad(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  layer_set_update_proc(window_layer, splashScreenLayerUpdateProc);
  // GRect bounds = layer_get_bounds(window_layer);
  // BitmapLayer *splashLogoLayer = bitmap_layer_create(bounds);
  // splashLogo = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOGO_SPLASH);

  // bitmap_layer_set_bitmap(splashLogoLayer, splashLogo);

  // layer_add_child(window_layer, bitmap_layer_get_layer(splashLogoLayer));
}

static void splashScreenUnload() {
  // gbitmap_destroy(splashLogo);
}

static void syncChangedHandler(const uint32_t key, const Tuple *newTuple, const Tuple *oldTuple, void *context) {
  // update UI here
}

static void syncErrorHandler(DictionaryResult dictError, AppMessageResult appMessageError, void *context) {
  // An error occured!
}

static void init(void) {
  splashWindow = window_create();
  // window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(splashWindow, (WindowHandlers) {
    .load = splashScreenLoad,
    .unload = splashScreenUnload
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