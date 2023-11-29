#pragma once

#include <lvgl.h>
#include "lv_subjects.hpp"

#define LV_SYMBOL_DOUBLE_LEFT LV_SYMBOL_LEFT " " LV_SYMBOL_LEFT
#define LV_SYMBOL_DOUBLE_RIGHT LV_SYMBOL_RIGHT " " LV_SYMBOL_RIGHT
#define LV_SYMBOL_DEGREES "\xC2\xB0"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef COMPASS_HEADING_MAG
#define COMPASS_HEADING_MAG 1
#endif
#ifndef COMPASS_HEADING_TRUE
#define COMPASS_HEADING_TRUE 1
#endif
#ifndef COMPASS_MAG_VAR
#define COMPASS_MAG_VAR 1
#endif
#ifndef COMPASS_COG_TRUE
#define COMPASS_COG_TRUE 1
#endif
#ifndef COMPASS_SUN_POS
#define COMPASS_SUN_POS 0
#endif

    extern lv_subject_t compass_all, heading_mag, heading_true, course_over_ground_true, mag_var, sun_pos;

    void lv_compass_display(lv_obj_t *parent);

#ifdef __cplusplus
} /*extern "C"*/
#endif
