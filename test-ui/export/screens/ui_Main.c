// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "../ui.h"

void ui_Main_screen_init(void)
{
ui_Main = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Main, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_flex_flow(ui_Main,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_Main, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START);

ui_mainHeader = lv_obj_create(ui_Main);
lv_obj_set_width( ui_mainHeader, lv_pct(100));
lv_obj_set_height( ui_mainHeader, lv_pct(10));
lv_obj_set_x( ui_mainHeader, 9 );
lv_obj_set_y( ui_mainHeader, -326 );
lv_obj_set_align( ui_mainHeader, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_mainHeader,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_mainHeader, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_mainHeader, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_GPSStatus = lv_label_create(ui_mainHeader);
lv_obj_set_width( ui_GPSStatus, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_GPSStatus, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_GPSStatus, LV_ALIGN_CENTER );
lv_label_set_text(ui_GPSStatus,"G");

ui_SdCardStatus = lv_label_create(ui_mainHeader);
lv_obj_set_width( ui_SdCardStatus, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_SdCardStatus, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_SdCardStatus, -39 );
lv_obj_set_y( ui_SdCardStatus, -105 );
lv_obj_set_align( ui_SdCardStatus, LV_ALIGN_CENTER );
lv_label_set_text(ui_SdCardStatus,"");

ui_BLEStatus = lv_label_create(ui_mainHeader);
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

ui_WifiStatus = lv_label_create(ui_mainHeader);
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

ui_BatteryStatus = lv_label_create(ui_mainHeader);
lv_obj_set_width( ui_BatteryStatus, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_BatteryStatus, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_BatteryStatus, 78 );
lv_obj_set_y( ui_BatteryStatus, -106 );
lv_obj_set_align( ui_BatteryStatus, LV_ALIGN_CENTER );
lv_label_set_text(ui_BatteryStatus,"");
lv_obj_set_style_text_color(ui_BatteryStatus, lv_color_hex(0x7B7777), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_BatteryStatus, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_mainArea = lv_obj_create(ui_Main);
lv_obj_set_width( ui_mainArea, lv_pct(99));
lv_obj_set_height( ui_mainArea, lv_pct(87));
lv_obj_set_x( ui_mainArea, -47 );
lv_obj_set_y( ui_mainArea, 66 );
lv_obj_set_align( ui_mainArea, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_mainArea,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_mainArea, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_mainArea, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_text_font(ui_mainArea, &lv_font_montserrat_18, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_descriptions = lv_obj_create(ui_mainArea);
lv_obj_set_width( ui_descriptions, lv_pct(70));
lv_obj_set_height( ui_descriptions, lv_pct(100));
lv_obj_set_align( ui_descriptions, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_descriptions,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_descriptions, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_clear_flag( ui_descriptions, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_text_align(ui_descriptions, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_envTempLabel = lv_label_create(ui_descriptions);
lv_obj_set_width( ui_envTempLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_envTempLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_envTempLabel, -49 );
lv_obj_set_y( ui_envTempLabel, -20 );
lv_obj_set_align( ui_envTempLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_envTempLabel,"envelope temp");

ui_envelopeHumLabel = lv_label_create(ui_descriptions);
lv_obj_set_width( ui_envelopeHumLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_envelopeHumLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_envelopeHumLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_envelopeHumLabel,"envelope hum");

ui_outsideTempLabel = lv_label_create(ui_descriptions);
lv_obj_set_width( ui_outsideTempLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_outsideTempLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_outsideTempLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_outsideTempLabel,"outside temp");

ui_outsideHumLabel = lv_label_create(ui_descriptions);
lv_obj_set_width( ui_outsideHumLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_outsideHumLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_outsideHumLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_outsideHumLabel,"outside hum");

ui_values = lv_obj_create(ui_mainArea);
lv_obj_set_width( ui_values, lv_pct(30));
lv_obj_set_height( ui_values, lv_pct(100));
lv_obj_set_x( ui_values, -322 );
lv_obj_set_y( ui_values, -193 );
lv_obj_set_align( ui_values, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_values,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_values, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_clear_flag( ui_values, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_envTemp = lv_label_create(ui_values);
lv_obj_set_width( ui_envTemp, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_envTemp, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_envTemp, LV_ALIGN_CENTER );
lv_label_set_text(ui_envTemp,"");

ui_envHum = lv_label_create(ui_values);
lv_obj_set_width( ui_envHum, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_envHum, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_envHum, LV_ALIGN_CENTER );
lv_label_set_text(ui_envHum,"");

ui_outsideTemp = lv_label_create(ui_values);
lv_obj_set_width( ui_outsideTemp, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_outsideTemp, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_outsideTemp, LV_ALIGN_CENTER );
lv_label_set_text(ui_outsideTemp,"");

ui_outsideHum = lv_label_create(ui_values);
lv_obj_set_width( ui_outsideHum, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_outsideHum, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_outsideHum, LV_ALIGN_CENTER );
lv_label_set_text(ui_outsideHum,"");

lv_obj_add_event_cb(ui_Main, ui_event_Main, LV_EVENT_ALL, NULL);

}
