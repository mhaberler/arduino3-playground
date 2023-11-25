#ifndef _LVGL_SETUP_H
#define _LVGL_SETUP_H

#include <stdint.h>

void lv_begin();
void lvgl_acquire(void);
void lvgl_release(void);
void lvgl_msg_send_prot(uint32_t msg_id, const void *payload);

void lv_events_init(void);
void lv_updates_init(void);

#endif