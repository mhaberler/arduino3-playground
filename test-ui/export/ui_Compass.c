// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "ui.h"

void ui_Compass_screen_init(void)
{
ui_Compass = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Compass, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

lv_obj_add_event_cb(ui_Compass, ui_event_Compass, LV_EVENT_ALL, NULL);

}
