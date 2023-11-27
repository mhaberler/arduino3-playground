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

void set_battery_indicator(int32_t batval)
{
    lv_color_t color = lv_palette_main(LV_PALETTE_GREY);
    const void *label = "?";
        lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;

    if (batval < 20)
    {
        color = lv_palette_main(LV_PALETTE_RED);
        label = LV_SYMBOL_BATTERY_EMPTY;
    }
    else if (batval < 50)
    {
        color = lv_palette_main(LV_PALETTE_RED);
        label = LV_SYMBOL_BATTERY_1;
    }
    else if (batval < 70)
    {
        color = lv_palette_main(LV_PALETTE_DEEP_ORANGE);
        label = LV_SYMBOL_BATTERY_2;
    }
    else if (batval < 90)
    {
        color = lv_palette_main(LV_PALETTE_GREEN);
        label = LV_SYMBOL_BATTERY_3;
    }
    else
    {
        color = lv_palette_main(LV_PALETTE_GREEN);
        label = LV_SYMBOL_BATTERY_FULL;
    }
    lvgl_acquire();
    battery_color.user_data = (void*)sel;
    lv_subject_set_color(&battery_color, color);
    lv_subject_set_pointer(&battery_label, (void*)label);
    lv_subject_notify(&battery_all);
    lvgl_release();
}

void lvgl_setup(void)
{
    lv_begin();
    ui_init();        // Squareline UI
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
        set_battery_indicator(battery_value);

        update_battery = false;
    }
}

#else
void lvgl_setup(void) {}
void lvgl_loop(void) {}
#endif