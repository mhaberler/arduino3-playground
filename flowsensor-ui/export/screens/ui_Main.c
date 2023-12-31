// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.2.0
// Project name: SquareLine_Project

#include "../ui.h"

void ui_Main_screen_init(void)
{
ui_Main = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Main, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_flex_flow(ui_Main,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_Main, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

ui_Top = lv_obj_create(ui_Main);
lv_obj_set_width( ui_Top, lv_pct(90));
lv_obj_set_height( ui_Top, lv_pct(60));
lv_obj_set_align( ui_Top, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_Top,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_Top, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_Top, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_Values = lv_obj_create(ui_Top);
lv_obj_set_width( ui_Values, lv_pct(75));
lv_obj_set_height( ui_Values, lv_pct(110));
lv_obj_set_align( ui_Values, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_Values,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_Values, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_clear_flag( ui_Values, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_Count = lv_label_create(ui_Values);
lv_obj_set_width( ui_Count, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Count, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Count, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_Count,"count:");
lv_obj_set_style_text_font(ui_Count, &ui_font_Font2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Rate = lv_label_create(ui_Values);
lv_obj_set_width( ui_Rate, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Rate, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Rate, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_Rate,"rate:");
lv_obj_set_style_text_font(ui_Rate, &ui_font_Font2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_MaxRate = lv_label_create(ui_Values);
lv_obj_set_width( ui_MaxRate, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_MaxRate, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_MaxRate, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_MaxRate,"max:");
lv_obj_set_style_text_font(ui_MaxRate, &ui_font_Font2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_pressure = lv_label_create(ui_Values);
lv_obj_set_width( ui_pressure, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_pressure, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_pressure, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_pressure,"bar:");
lv_obj_set_style_text_font(ui_pressure, &ui_font_Font2, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Buttons = lv_obj_create(ui_Top);
lv_obj_set_width( ui_Buttons, lv_pct(30));
lv_obj_set_height( ui_Buttons, lv_pct(100));
lv_obj_set_align( ui_Buttons, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_Buttons, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_ClearCount = lv_btn_create(ui_Buttons);
lv_obj_set_width( ui_ClearCount, lv_pct(130));
lv_obj_set_height( ui_ClearCount, lv_pct(120));
lv_obj_set_align( ui_ClearCount, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ClearCount, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ClearCount, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ClearCount, lv_color_hex(0xCAEE2E), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ClearCount, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_ClearCount, lv_color_hex(0xEC380C), LV_PART_MAIN | LV_STATE_PRESSED );
lv_obj_set_style_bg_opa(ui_ClearCount, 255, LV_PART_MAIN| LV_STATE_PRESSED);

ui_ClearCountLabel = lv_label_create(ui_ClearCount);
lv_obj_set_width( ui_ClearCountLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_ClearCountLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_ClearCountLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_ClearCountLabel,"Reset");
lv_obj_set_style_text_color(ui_ClearCountLabel, lv_color_hex(0x040404), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_ClearCountLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Bottom = lv_obj_create(ui_Main);
lv_obj_set_width( ui_Bottom, lv_pct(90));
lv_obj_set_height( ui_Bottom, lv_pct(30));
lv_obj_set_align( ui_Bottom, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_Bottom,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_Bottom, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_Bottom, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_FlowRate = lv_bar_create(ui_Bottom);
lv_bar_set_value(ui_FlowRate,25,LV_ANIM_OFF);
lv_obj_set_width( ui_FlowRate, lv_pct(100));
lv_obj_set_height( ui_FlowRate, lv_pct(40));
lv_obj_set_align( ui_FlowRate, LV_ALIGN_CENTER );

ui_Battery = lv_bar_create(ui_Bottom);
lv_bar_set_value(ui_Battery,25,LV_ANIM_OFF);
lv_obj_set_width( ui_Battery, lv_pct(100));
lv_obj_set_height( ui_Battery, lv_pct(40));
lv_obj_set_align( ui_Battery, LV_ALIGN_CENTER );
lv_obj_add_state( ui_Battery, LV_STATE_USER_1 | LV_STATE_USER_2 | LV_STATE_USER_3 );     /// States

lv_obj_add_event_cb(ui_ClearCount, ui_event_ClearCount, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_Main, ui_event_Main, LV_EVENT_ALL, NULL);

}
