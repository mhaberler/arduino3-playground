#ifndef _LVGL_SETUP_H
#define _LVGL_SETUP_H

void lv_begin();
void lvgl_acquire(void);
void lvgl_release(void);
void lv_events_init(void);
void lv_updates_init(void);

#endif