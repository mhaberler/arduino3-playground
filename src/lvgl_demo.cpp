#ifdef LVGL_DEMO

#if defined(M5UNIFIED)
#include "M5Unified.h"
#else
#include <Arduino.h>
#endif
#include <Ticker.h>

#include "lv_setup.hpp"
#include "lv_util.h"
#include "subjects.hpp"
#include "ui.h"
#include "ui_custom.hpp"

static Ticker batteryChange;
static bool update_battery = false;
static int32_t battery_value;

void lvgl_setup(void)
{
    lv_begin();
    ui_init(); // Squareline UI
    ui_custom_init(); // stuff which cant be easily done in Squareline 
    lv_observer_init();
    batteryChange.attach_ms(2000, []()
                            { update_battery = true; });
}

void lvgl_loop(void)
{
    if (update_battery)
    {
        // Battery icon animation
        if (battery_value > 100)
            battery_value = 0;
        battery_value += 10;
        lvgl_acquire();
        lv_subject_set_int(&battery_status, battery_value);
        lvgl_release();
        update_battery = false;
    }
}

#else
void lvgl_setup(void) {}
void lvgl_loop(void) {}
#endif