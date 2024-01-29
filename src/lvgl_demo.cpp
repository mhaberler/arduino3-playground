#ifdef LVGL_DEMO

#if defined(M5UNIFIED)
#include "M5Unified.h"
#else
#include <Arduino.h>
#endif
#include <Ticker.h>

#include "lv_setup.hpp"
#include "lv_util.h"
#include "lv_subjects.hpp"
#include "lv_observer.hpp"
#include "ui.h"
#include "ui_message.hpp"
#include "ui_compass.h"
#include "ui_custom.hpp"
#include "ArduinoJsonCustom.h"

static Ticker batteryChange;
static bool update_battery = false;
static int32_t battery_value;
static int32_t heading, mvar, sunp;

#ifdef M5UNIFIED
#include <SD.h>

#ifdef CORES3
#define AW9523_ADDR 0x58
#define SD_CS 4
static bool SDCardExist()
{
    return (bool)!((M5.In_I2C.readRegister8(AW9523_ADDR, 0x00, 100000L) >> 4) & 0x01);
}

bool SDInit()
{
    return SD.begin(SD_CS, SPI, 25000000);
}
#endif
#else
#endif

void set_battery_indicator(int32_t batval)
{
    sendUiMessage("{\"um\":%d,\"v\":%d}", UM_STATUS_BATTERY, batval);
    // works the same:
    // JsonDocument jdoc;
    // jdoc["um"] = UM_STATUS_BATTERY;
    // jdoc["v"] = batval;
    // sendUiMessage(jdoc);
}

void lvgl_setup(void)
{
    lv_begin();
    ui_init();          // Squareline UI
    lv_subjects_init(); // order is important
    lv_observer_init();
    ui_custom_init(); // stuff which cant be easily done in Squareline

#if CORES3 && M5UNIFIED
    if (SDCardExist())
        SDInit();
    lv_subject_set_int(&sdcard_status, SDCardExist());
#endif

    heading_mag.user_data = (void *)1;
    heading_true.user_data = (void *)1;
    course_over_ground_true.user_data = (void *)1;
    mag_var.user_data = (void *)1;
    sun_pos.user_data = (void *)1;
    batteryChange.attach_ms(1000, []()
                            { update_battery = true; });
}

void lvgl_loop(void)
{
    if (update_battery)
    {
        // Battery icon animation
        battery_value += 10;
        set_battery_indicator(battery_value);
        if (battery_value >= 100)
            battery_value = 0;

        heading += 3;
        heading %= 360;
        mvar++;
        mvar %= 6;
        sunp++;
        sunp %= 360;

        if (lv_scr_act() == ui_Compass)
        {
            lvgl_acquire();
            lv_subject_set_int(&heading_mag, heading);
            lv_subject_set_int(&heading_true, heading + 7);
            lv_subject_set_int(&course_over_ground_true, heading + 33);
            lv_subject_set_int(&mag_var, mvar);
            lv_subject_set_int(&sun_pos, sunp);
            lv_subject_notify(&compass_all);
            lvgl_release();
        }
#if CORES3 && M5UNIFIED
        lv_subject_set_int(&sdcard_status, SDCardExist());
#else
        // just toggle the label checked/unchecked
        lv_subject_set_int(&sdcard_status, lv_subject_get_int(&sdcard_status) ^ 1);
#endif
        update_battery = false;
    }
}

#else
void lvgl_setup(void) {}
void lvgl_loop(void) {}
#endif