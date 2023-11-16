#ifdef LVGL_DEMO
#include "lv_setup.h"
#include "ui.h"

void clearCountPressed(lv_event_t *e) {}

void lvgl_setup(void)
{
    lv_begin();
    ui_init();
}

void lvgl_loop(void)
{
    lv_handler(); // Update UI
}

#else
void lvgl_setup(void) {}
void lvgl_loop(void) {}
#endif