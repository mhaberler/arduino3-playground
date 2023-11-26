#ifndef _LVGL_SETUP_H
#define _LVGL_SETUP_H

#include <lvgl.h>

void lv_begin(void);
#ifdef __cplusplus
extern "C" {
#endif

void subjects_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif


void lvgl_acquire(void);
void lvgl_release(void);

void lv_events_init(void);
void lv_updates_init(void);

#endif