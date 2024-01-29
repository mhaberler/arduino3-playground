#pragma once

#include <lvgl.h>

void ui_custom_init(void);
lv_color_t getDefaultColor(void);

extern "C"
{
    void showMemory(lv_event_t *e);
    void mainScreenLoaded(lv_event_t *e);
    void statusScreenLoaded(lv_event_t *e);
    void nfcEvent(lv_event_t *e);
    void ruuviEnvelope(lv_event_t *e);
    void ruuviOAT(lv_event_t *e);
    void ruuviCancel(lv_event_t *e);
    void setUnit(lv_event_t * e);
}