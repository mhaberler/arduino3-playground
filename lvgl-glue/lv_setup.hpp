#ifndef _LVGL_SETUP_H
#define _LVGL_SETUP_H

#include <stdint.h>

void lv_begin();
void observer_init();

void lv_events_init(void);
void lv_updates_init(void);

extern "C"
{
    void lvgl_acquire(void);
    void lvgl_release(void);
}

#endif