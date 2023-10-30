// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.1
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
// SCREEN: ui_Main
void ui_Main_screen_init(void);
extern lv_obj_t *ui_Main;
extern lv_obj_t *ui_Top;
extern lv_obj_t *ui_Values;
extern lv_obj_t *ui_Count;
extern lv_obj_t *ui_Rate;
extern lv_obj_t *ui_MaxRate;
extern lv_obj_t *ui_pressure;
extern lv_obj_t *ui_Buttons;
void ui_event_ClearCount( lv_event_t * e);
extern lv_obj_t *ui_ClearCount;
extern lv_obj_t *ui_ClearCountLabel;
extern lv_obj_t *ui_Bottom;
extern lv_obj_t *ui_FlowRate;
extern lv_obj_t *ui_Battery;
extern lv_obj_t *ui____initial_actions0;

LV_FONT_DECLARE( ui_font_Font1);
LV_FONT_DECLARE( ui_font_Font2);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
