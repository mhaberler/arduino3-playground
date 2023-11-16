// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "../ui.h"

void ui_second_screen_init(void)
{
ui_second = lv_obj_create(NULL);
lv_obj_clear_flag( ui_second, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_Button1 = lv_btn_create(ui_second);
lv_obj_set_width( ui_Button1, 100);
lv_obj_set_height( ui_Button1, 50);
lv_obj_set_x( ui_Button1, -61 );
lv_obj_set_y( ui_Button1, -70 );
lv_obj_set_align( ui_Button1, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Button1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_Label1 = lv_label_create(ui_second);
lv_obj_set_width( ui_Label1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label1, -71 );
lv_obj_set_y( ui_Label1, 40 );
lv_obj_set_align( ui_Label1, LV_ALIGN_CENTER );

ui_Switch1 = lv_switch_create(ui_second);
lv_obj_set_width( ui_Switch1, 50);
lv_obj_set_height( ui_Switch1, 25);
lv_obj_set_x( ui_Switch1, 102 );
lv_obj_set_y( ui_Switch1, -70 );
lv_obj_set_align( ui_Switch1, LV_ALIGN_CENTER );


lv_obj_add_event_cb(ui_second, ui_event_second, LV_EVENT_ALL, NULL);

}
