#include <lvgl.h>
#include "lv_setup.hpp"
#include "lv_util.h"
#include "lv_subjects.hpp"

lv_subject_t oat_temp, oat_hum, env_temp, env_hum, wifi_color, http_status, sdcard_status, ble_color;
lv_subject_t battery_all, battery_color, battery_label;
lv_subject_t runCompassAnimation, animationSpeed;

static lv_subject_t *battery_list[] = {&battery_label, &battery_color};

void lv_subjects_init(void)
{
    lvgl_acquire();
    lv_subject_init_int(&oat_temp, 0);
    lv_subject_init_int(&oat_hum, 0);
    lv_subject_init_int(&env_temp, 0);
    lv_subject_init_int(&env_hum, 0);

    lv_subject_init_color(&wifi_color, STATUS_WIFI_UNCONFIGURED);
    lv_subject_init_color(&ble_color, STATUS_BLE_IDLE);
    lv_subject_init_int(&sdcard_status, STATUS_SDCARD_MISSING);

    lv_subject_init_color(&battery_color, lv_palette_main(LV_PALETTE_RED));
    lv_subject_init_pointer(&battery_label, (void *)LV_SYMBOL_BATTERY_EMPTY);

    lv_subject_init_group(&battery_all, battery_list, 2);

    lv_subject_init_int(&runCompassAnimation, 0);
    lv_subject_init_int(&animationSpeed, 30);

    lvgl_release();
}
