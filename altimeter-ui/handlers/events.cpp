// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: lvgl_altimeter_UI
#if defined(M5UNIFIED)
#include "M5Unified.h"
#else
#include <Arduino.h>
#endif

#include "ui.h"
extern "C"
{
    void mainPreviousPage(lv_event_t *e)
    {
        Serial.printf("--->  mainPreviousPage \n");
    }
    void back2main(lv_event_t *e)
    {
        Serial.printf("--->  back2main \n");
    }
}