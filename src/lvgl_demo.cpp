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

void lvgl_setup(void)
{
    lv_begin();
    ui_init(); // Squareline UI
    lv_observer_init();
}

void lvgl_loop(void)
{
}

#else
void lvgl_setup(void) {}
void lvgl_loop(void) {}
#endif