// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "ui.h"

void ui_Main_screen_init(void)
{
ui_Main = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Main, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_flex_flow(ui_Main,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_Main, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START);

ui_Container8 = lv_obj_create(ui_Main);
lv_obj_remove_style_all(ui_Container8);
lv_obj_set_width( ui_Container8, lv_pct(100));
lv_obj_set_height( ui_Container8, lv_pct(10));
lv_obj_set_align( ui_Container8, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_Container8,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_Container8, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_Container8, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_pad_left(ui_Container8, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_Container8, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_Container8, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_Container8, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_row(ui_Container8, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_column(ui_Container8, 10, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_GPSStatus = lv_label_create(ui_Container8);
lv_obj_set_width( ui_GPSStatus, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_GPSStatus, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_GPSStatus, LV_ALIGN_CENTER );
lv_label_set_text(ui_GPSStatus,"G");

ui_SdCardStatus = lv_label_create(ui_Container8);
lv_obj_set_width( ui_SdCardStatus, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_SdCardStatus, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_SdCardStatus, -39 );
lv_obj_set_y( ui_SdCardStatus, -105 );
lv_obj_set_align( ui_SdCardStatus, LV_ALIGN_CENTER );
lv_label_set_text(ui_SdCardStatus,"");
lv_obj_set_style_text_color(ui_SdCardStatus, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_SdCardStatus, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_color(ui_SdCardStatus, lv_color_hex(0xCDF011), LV_PART_MAIN | LV_STATE_CHECKED );
lv_obj_set_style_text_opa(ui_SdCardStatus, 255, LV_PART_MAIN| LV_STATE_CHECKED);

ui_BLEStatus = lv_label_create(ui_Container8);
lv_obj_set_width( ui_BLEStatus, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_BLEStatus, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_BLEStatus, 21 );
lv_obj_set_y( ui_BLEStatus, -106 );
lv_obj_set_align( ui_BLEStatus, LV_ALIGN_CENTER );
lv_label_set_text(ui_BLEStatus,"");
lv_obj_add_flag( ui_BLEStatus, LV_OBJ_FLAG_CHECKABLE );   /// Flags
lv_obj_clear_flag( ui_BLEStatus, LV_OBJ_FLAG_CLICK_FOCUSABLE );    /// Flags
lv_obj_set_style_text_color(ui_BLEStatus, lv_color_hex(0x0060B5), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_BLEStatus, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_color(ui_BLEStatus, lv_color_hex(0x00F7FF), LV_PART_MAIN | LV_STATE_CHECKED );
lv_obj_set_style_text_opa(ui_BLEStatus, 255, LV_PART_MAIN| LV_STATE_CHECKED);

ui_WifiStatus = lv_label_create(ui_Container8);
lv_obj_set_width( ui_WifiStatus, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_WifiStatus, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_WifiStatus, -220 );
lv_obj_set_y( ui_WifiStatus, 90 );
lv_obj_set_align( ui_WifiStatus, LV_ALIGN_TOP_RIGHT );
lv_label_set_text(ui_WifiStatus,"");
lv_obj_set_style_text_color(ui_WifiStatus, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_WifiStatus, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_WifiStatus, lv_color_hex(0x07FA08), LV_PART_MAIN | LV_STATE_CHECKED );
lv_obj_set_style_bg_opa(ui_WifiStatus, 255, LV_PART_MAIN| LV_STATE_CHECKED);

ui_BatteryStatus = lv_label_create(ui_Container8);
lv_obj_set_width( ui_BatteryStatus, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_BatteryStatus, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_BatteryStatus, 78 );
lv_obj_set_y( ui_BatteryStatus, -106 );
lv_obj_set_align( ui_BatteryStatus, LV_ALIGN_CENTER );
lv_label_set_text(ui_BatteryStatus,"");
lv_obj_set_style_text_color(ui_BatteryStatus, lv_color_hex(0x7B7777), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_BatteryStatus, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_TankContainer2 = lv_obj_create(ui_Main);
lv_obj_remove_style_all(ui_TankContainer2);
lv_obj_set_width( ui_TankContainer2, lv_pct(100));
lv_obj_set_height( ui_TankContainer2, lv_pct(90));
lv_obj_set_align( ui_TankContainer2, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_TankContainer2,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_TankContainer2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_TankContainer2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_Container10 = lv_obj_create(ui_TankContainer2);
lv_obj_remove_style_all(ui_Container10);
lv_obj_set_width( ui_Container10, lv_pct(70));
lv_obj_set_height( ui_Container10, lv_pct(90));
lv_obj_set_align( ui_Container10, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_Container10, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_tanksChart = lv_chart_create(ui_Container10);
lv_obj_set_width( ui_tanksChart, lv_pct(60));
lv_obj_set_height( ui_tanksChart, lv_pct(80));
lv_obj_set_align( ui_tanksChart, LV_ALIGN_TOP_MID );
lv_chart_set_type( ui_tanksChart, LV_CHART_TYPE_BAR);
lv_chart_set_point_count( ui_tanksChart, 2);
lv_chart_set_range( ui_tanksChart, LV_CHART_AXIS_SECONDARY_Y, 0, 15);
lv_chart_set_div_line_count( ui_tanksChart, 5, 0);
lv_chart_set_axis_tick( ui_tanksChart, LV_CHART_AXIS_PRIMARY_X, 0, 0, 5, 5, false, 50);
lv_chart_set_axis_tick( ui_tanksChart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);
lv_chart_set_axis_tick( ui_tanksChart, LV_CHART_AXIS_SECONDARY_Y, 5, 0, 4, 5, true, 25);



ui_Container11 = lv_obj_create(ui_TankContainer2);
lv_obj_remove_style_all(ui_Container11);
lv_obj_set_width( ui_Container11, lv_pct(20));
lv_obj_set_height( ui_Container11, lv_pct(90));
lv_obj_set_align( ui_Container11, LV_ALIGN_TOP_MID );
lv_obj_set_flex_flow(ui_Container11,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_Container11, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_Container11, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_pad_row(ui_Container11, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_column(ui_Container11, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_envTemp = lv_label_create(ui_Container11);
lv_obj_set_width( ui_envTemp, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_envTemp, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_envTemp, LV_ALIGN_CENTER );
lv_label_set_text(ui_envTemp,"");

ui_envHum = lv_label_create(ui_Container11);
lv_obj_set_width( ui_envHum, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_envHum, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_envHum, LV_ALIGN_CENTER );
lv_label_set_text(ui_envHum,"");

ui_outsideTemp = lv_label_create(ui_Container11);
lv_obj_set_width( ui_outsideTemp, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_outsideTemp, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_outsideTemp, LV_ALIGN_CENTER );
lv_label_set_text(ui_outsideTemp,"");

ui_outsideHum = lv_label_create(ui_Container11);
lv_obj_set_width( ui_outsideHum, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_outsideHum, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_outsideHum, LV_ALIGN_CENTER );
lv_label_set_text(ui_outsideHum,"");

ui_fuelLiters = lv_label_create(ui_Container11);
lv_obj_set_width( ui_fuelLiters, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_fuelLiters, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_fuelLiters, LV_ALIGN_CENTER );
lv_label_set_text(ui_fuelLiters,"");

ui_fuelPct = lv_label_create(ui_Container11);
lv_obj_set_width( ui_fuelPct, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_fuelPct, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_fuelPct, LV_ALIGN_CENTER );
lv_label_set_text(ui_fuelPct,"");

lv_obj_add_event_cb(ui_TankContainer2, ui_event_TankContainer2, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_Main, ui_event_Main, LV_EVENT_ALL, NULL);

}