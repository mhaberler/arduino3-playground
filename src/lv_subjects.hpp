#pragma once

#include <lvgl.h>
#include "lv_observer.h"
#include "ui_compass.h"

#define STATUS_WIFI_UNCONFIGURED lv_palette_main(LV_PALETTE_GREY)
#define STATUS_WIFI_STARTED lv_palette_main(LV_PALETTE_AMBER)
#define STATUS_WIFI_SCAN_COMPLETE lv_palette_main(LV_PALETTE_GREEN)
#define STATUS_WIFI_CONNECTED lv_palette_main(LV_PALETTE_TEAL)
#define STATUS_WIFI_GOT_IP lv_palette_main(LV_PALETTE_GREEN)
#define STATUS_WIFI_LOST_IP lv_palette_main(LV_PALETTE_DEEP_ORANGE)
#define STATUS_WIFI_DISCONNECTED lv_palette_main(LV_PALETTE_RED)
#define STATUS_WIFI_TRAFFIC lv_palette_main(LV_PALETTE_LIGHT_GREEN)

typedef enum {
    STATUS_BATTERY_DISCONNECTED = -1,
} battery_status_t;

#define F2I100(x) ((int)(((double)x) * 100.0))
#define ITOD100(x) ((double)(x) / 100.0)
#define NOT_AVAILABLE "n/a"

extern lv_subject_t oat_temp, oat_hum, env_temp, env_hum, wifi_color,  sdcard_status, ble_traffic;
extern lv_subject_t battery_all, battery_color, battery_label;
extern lv_subject_t runCompassAnimation, animationSpeed;
extern lv_subject_t nfcMessage;

void lv_subjects_init(void);
