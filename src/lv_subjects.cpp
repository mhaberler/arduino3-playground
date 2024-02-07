#include <lvgl.h>
#include "lv_setup.hpp"
#include "lv_util.h"
#include "lv_subjects.hpp"
#include "ui.h"

#include "nfc_input.h"
#include "esp_heap_caps.h"

extern bool psramFound();

lv_subject_t oat_temp, oat_hum, env_temp, env_hum, wifi_color, http_status, sdcard_status, ble_traffic;
lv_subject_t runCompassAnimation, animationSpeed;
lv_subject_t uiMessage;

uint16_t tanks_seen = 0;
static lv_coord_t tank_level_series[MAX_TANKS];  // =  {LV_CHART_POINT_NONE};
static lv_coord_t tank_pressure_series[MAX_TANKS]; //  = {LV_CHART_POINT_NONE};

static lv_color_t level_color = lv_color_hex(0x17E4B4);
static lv_color_t pressure_color = lv_color_hex(0xFF0063);

static lv_chart_series_t *ui_level_series;
static lv_chart_series_t *ui_pressure_series;

void set_level(int index, lv_coord_t value) {
    tank_level_series[index] = value;
    lv_chart_refresh(ui_tanksChart);
}

void set_pressure(int index, lv_coord_t value) {
    tank_pressure_series[index] = value;
    lv_chart_refresh(ui_tanksChart);
}

// ui_tanksChart = lv_chart_create(ui_Container10);
// lv_obj_set_width( ui_tanksChart, lv_pct(60));
// lv_obj_set_height( ui_tanksChart, lv_pct(80));
// lv_obj_set_align( ui_tanksChart, LV_ALIGN_TOP_MID );
// lv_chart_set_type( ui_tanksChart, LV_CHART_TYPE_BAR);
// lv_chart_set_point_count( ui_tanksChart, 2);
// lv_chart_set_range( ui_tanksChart, LV_CHART_AXIS_SECONDARY_Y, 1, 15);
// lv_chart_set_axis_tick( ui_tanksChart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 5, 5, true, 50);
// lv_chart_set_axis_tick( ui_tanksChart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);
// lv_chart_set_axis_tick( ui_tanksChart, LV_CHART_AXIS_SECONDARY_Y, 5, 5, 2, 5, true, 25);

// lv_chart_series_t* ui_tanksChart_series_1 = lv_chart_add_series(ui_tanksChart, lv_color_hex(0x1AE0E6), LV_CHART_AXIS_PRIMARY_Y);
// static lv_coord_t ui_tanksChart_series_1_array[] = { 0,10,20 };
// lv_chart_set_ext_y_array(ui_tanksChart, ui_tanksChart_series_1, ui_tanksChart_series_1_array);

// lv_chart_series_t* ui_tanksChart_series_2 = lv_chart_add_series(ui_tanksChart, lv_color_hex(0xED0D0D), LV_CHART_AXIS_SECONDARY_Y);
// static lv_coord_t ui_tanksChart_series_2_array[] = { 0,10,20 };
// lv_chart_set_ext_y_array(ui_tanksChart, ui_tanksChart_series_2, ui_tanksChart_series_2_array);

void lv_tanks_init(void) {

    ui_level_series = lv_chart_add_series(ui_tanksChart, level_color, LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(ui_tanksChart, ui_level_series, tank_level_series);

    ui_pressure_series = lv_chart_add_series(ui_tanksChart, pressure_color, LV_CHART_AXIS_SECONDARY_Y);
    lv_chart_set_ext_y_array(ui_tanksChart, ui_pressure_series, tank_pressure_series);
}

void lv_subjects_init(void) {
    lvgl_acquire();
    lv_subject_init_int(&oat_temp, 0);
    lv_subject_init_int(&oat_hum, 0);
    lv_subject_init_int(&env_temp, 0);
    lv_subject_init_int(&env_hum, 0);

    lv_subject_init_color(&wifi_color, STATUS_WIFI_UNCONFIGURED);
    lv_subject_init_int(&ble_traffic, 0);
    lv_subject_init_int(&sdcard_status, 0);

    lv_subject_init_int(&runCompassAnimation, 0);
    lv_subject_init_int(&animationSpeed, 30);

    void *p =  heap_caps_malloc(UI_MESSAGESIZE, MALLOC_CAP_SPIRAM);
    lv_subject_init_string(&uiMessage, (char *)p, NULL, UI_MESSAGESIZE, "");

    lv_tanks_init();
    lvgl_release();
}
