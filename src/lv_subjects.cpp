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
static float tank_liters_left[MAX_TANKS];
static float tank_cap_liters[MAX_TANKS];

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

void set_liters(int index, float value) {
    tank_liters_left[index] = value;
    float total_left = 0;
    float cap_total = 0;
    for (int i = 0; i < tanks_seen; i++) {
        total_left += tank_liters_left[i];
        cap_total += tank_cap_liters[i];
    }
    float pct_left = 100.0 * total_left/cap_total;
    lv_label_set_text_fmt(ui_fuelLiters, "%d l", (int32_t) total_left);
    lv_label_set_text_fmt(ui_fuelPct, "%d%%", (int32_t) pct_left);
}

void set_cap(int index, float value) {
    tank_cap_liters[index] = value;
}

void lv_tanks_init(void) {

    ui_level_series = lv_chart_add_series(ui_tanksChart, level_color, LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(ui_tanksChart, ui_level_series, tank_level_series);

    ui_pressure_series = lv_chart_add_series(ui_tanksChart, pressure_color, LV_CHART_AXIS_SECONDARY_Y);
    lv_chart_set_ext_y_array(ui_tanksChart, ui_pressure_series, tank_pressure_series);
}

void lv_subjects_init(void) {
    lvgl_acquire();

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
