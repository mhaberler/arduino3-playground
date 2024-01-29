#include <lvgl.h>
#include "lv_setup.hpp"
#include "lv_util.h"
#include "lv_subjects.hpp"

#include "nfc_input.h"
#include "esp_heap_caps.h"


extern bool psramFound();

lv_subject_t oat_temp, oat_hum, env_temp, env_hum, wifi_color, http_status, sdcard_status, ble_traffic;
lv_subject_t runCompassAnimation, animationSpeed;
lv_subject_t uiMessage;



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

    lvgl_release();
}
