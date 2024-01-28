#include <lvgl.h>
#include "lv_setup.hpp"
#include "lv_util.h"
#include "lv_subjects.hpp"
#include "nfc_input.h"
#include "esp_heap_caps.h"

#define NFC_MESSAGESIZE 8192

lv_subject_t oat_temp, oat_hum, env_temp, env_hum, wifi_color, http_status, sdcard_status, ble_traffic;
lv_subject_t battery_all, battery_color, battery_label;
lv_subject_t runCompassAnimation, animationSpeed;
lv_subject_t nfcMessage;
extern bool psramFound();

static lv_subject_t *battery_list[] = {&battery_label, &battery_color};

void lv_subjects_init(void) {
    lvgl_acquire();
    lv_subject_init_int(&oat_temp, 0);
    lv_subject_init_int(&oat_hum, 0);
    lv_subject_init_int(&env_temp, 0);
    lv_subject_init_int(&env_hum, 0);

    lv_subject_init_color(&wifi_color, STATUS_WIFI_UNCONFIGURED);
    lv_subject_init_int(&ble_traffic, 0);
    lv_subject_init_int(&sdcard_status, 0);

    lv_subject_init_color(&battery_color, lv_palette_main(LV_PALETTE_RED));
    lv_subject_init_pointer(&battery_label, (void *)LV_SYMBOL_BATTERY_EMPTY);

    lv_subject_init_group(&battery_all, battery_list, 2);

    lv_subject_init_int(&runCompassAnimation, 0);
    lv_subject_init_int(&animationSpeed, 30);

    void *p =  heap_caps_malloc(NFC_MESSAGESIZE, MALLOC_CAP_SPIRAM);
    lv_subject_init_string(&nfcMessage, (char *)p, NULL, NFC_MESSAGESIZE, "{}");

    lvgl_release();
}
