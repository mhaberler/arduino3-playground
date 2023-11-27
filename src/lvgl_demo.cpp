#ifdef LVGL_DEMO

#if defined(M5UNIFIED)
#include "M5Unified.h"
#else
#include <Arduino.h>
#endif

#include "lv_setup.hpp"
#include "ui.h"

void lvgl_assert_fail(void)
{
    LV_LOG_USER("-------> ASSERT FAIL");
}
static bool observers_registered = false;
bool main_loaded = false;

void lvgl_setup(void)
{
    lv_begin();
    LV_ASSERT_MEM_INTEGRITY();

    ui_init(); // Squareline UI
    LV_ASSERT_MEM_INTEGRITY();

    lv_observer_init();
    LV_ASSERT_MEM_INTEGRITY();
}

void lvgl_loop(void)
{
    // if (main_loaded && !observers_registered)
    // {
    //     LV_ASSERT_MEM_INTEGRITY();

    //     LV_LOG_USER("lv_observer_register");

    //     lv_observer_register();
    //     LV_ASSERT_MEM_INTEGRITY();

    //     observers_registered = true;
    // }
}

#else
void lvgl_setup(void) {}
void lvgl_loop(void) {}
#endif