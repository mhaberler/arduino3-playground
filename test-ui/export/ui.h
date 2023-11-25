// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
  #if __has_include("lvgl.h")
    #include "lvgl.h"
  #elif __has_include("lvgl/lvgl.h")
    #include "lvgl/lvgl.h"
  #else
    #include "lvgl.h"
  #endif
#else
  #include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "ui_events.h"
void bleLed_Animation( lv_obj_t *TargetObject, int delay);
// SCREEN: ui_Main
void ui_Main_screen_init(void);
void ui_event_Main( lv_event_t * e);
extern lv_obj_t *ui_Main;
extern lv_obj_t *ui_mainHeader;
extern lv_obj_t *ui_SdCardStatus;
extern lv_obj_t *ui_BatteryStatus;
extern lv_obj_t *ui_BLEStatus;
extern lv_obj_t *ui_WifiStatus;
extern lv_obj_t *ui_mainArea;
extern lv_obj_t *ui_descriptions;
extern lv_obj_t *ui_envTempLabel;
extern lv_obj_t *ui_envelopeHumLabel;
extern lv_obj_t *ui_outsideTempLabel;
extern lv_obj_t *ui_outsideHumLabel;
extern lv_obj_t *ui_values;
extern lv_obj_t *ui_envTemp;
extern lv_obj_t *ui_envHum;
extern lv_obj_t *ui_outsideTemp;
extern lv_obj_t *ui_outsideHum;
// SCREEN: ui_Status
void ui_Status_screen_init(void);
void ui_event_Status( lv_event_t * e);
extern lv_obj_t *ui_Status;
extern lv_obj_t *ui_StatusText;
extern lv_obj_t *ui_Slider1;
extern lv_obj_t *ui_Chart1;
// SCREEN: ui_Params
void ui_Params_screen_init(void);
void ui_event_Params( lv_event_t * e);
extern lv_obj_t *ui_Params;
void ui_event_option1( lv_event_t * e);
extern lv_obj_t *ui_option1;
void ui_event_Switch1( lv_event_t * e);
extern lv_obj_t *ui_Switch1;
extern lv_obj_t *ui____initial_actions0;




void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif