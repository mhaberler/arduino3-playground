#ifdef LVGL_DEMO
#include "M5Unified.h"
#include "lv_setup.h"
#include "ui.h"

void clearCountPressed(lv_event_t *e) {}

// void scr_event_cb(lv_event_t *e)
// {
//     lv_event_code_t event_code = lv_event_get_code(e);
//     lv_obj_t *target = lv_event_get_target(e);
//     if ( event_code == LV_EVENT_GESTURE) {
//         Serial.printf("gesture\n");
//     }
// }



void lvgl_setup(void)
{
    lv_begin();
    ui_init();
    // lv_obj_add_event_cb(lv_scr_act(), scr_event_cb, LV_EVENT_ALL, NULL);
}

void lvgl_loop(void)
{
    lv_handler(); // Update UI
}

#else
void lvgl_setup(void) {}
void lvgl_loop(void) {}
#endif