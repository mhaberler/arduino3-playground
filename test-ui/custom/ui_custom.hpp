#pragma once

#include <lvgl.h>

void ui_custom_init(void);

extern "C"
{
    void showMemory(lv_event_t *e);
    void mainScreenLoaded(lv_event_t *e);
    void statusScreenLoaded(lv_event_t *e);
    void nfcEvent(lv_event_t *e);
    void setEnvelopeMac(lv_event_t *e);
    void setOATMac(lv_event_t *e);
}