#ifdef LVGL_DEMO

#if defined(M5UNIFIED)
#include "M5Unified.h"
#else
#include <Arduino.h>
#endif

#include "lv_setup.h"
#include "ui.h"

#define UPDATE_EVERY 500

void update_ui_values(void);

unsigned long last_ui_upd = 0;

void lvgl_setup(void)
{
    lv_begin();
    ui_init();
}

void lvgl_loop(void)
{
    if (millis() - last_ui_upd > UPDATE_EVERY)
    {
        lvgl_acquire();
        update_ui_values();
        lvgl_release();
        last_ui_upd = millis();
    }
}

#else
void lvgl_setup(void) {}
void lvgl_loop(void) {}
#endif