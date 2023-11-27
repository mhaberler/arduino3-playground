#pragma once

#include "defs.hpp"
#include <lvgl.h>

#define STATUS_WIFI_UNCONFIGURED lv_palette_main(LV_PALETTE_GREY)
#define STATUS_WIFI_STARTED lv_palette_main(LV_PALETTE_AMBER)
#define STATUS_WIFI_SCAN_COMPLETE lv_palette_main(LV_PALETTE_GREEN)
#define STATUS_WIFI_CONNECTED lv_palette_main(LV_PALETTE_TEAL)
#define STATUS_WIFI_GOT_IP lv_palette_main(LV_PALETTE_GREEN)
#define STATUS_WIFI_LOST_IP lv_palette_main(LV_PALETTE_DEEP_ORANGE)
#define STATUS_WIFI_DISCONNECTED lv_palette_main(LV_PALETTE_RED)
#define STATUS_WIFI_TRAFFIC lv_palette_main(LV_PALETTE_LIGHT_GREEN)

typedef enum
{
    STATUS_BATTERY_DISCONNECTED = -1,
} battery_status_t;

typedef enum
{
    STATUS_SDCARD_MISSING,
    STATUS_SDCARD_INSERTED,
    STATUS_SDCARD_MOUNTED,
    STATUS_SDCARD_READING,
    STATUS_SDCARD_WRITING
} sdcard_status_t;

#define STATUS_BLE_IDLE lv_palette_main(LV_PALETTE_BLUE_GREY)
#define STATUS_BLE_TRAFFIC lv_palette_main(LV_PALETTE_BLUE_GREY)
#define STATUS_BLE_TRAFFIC_FOR_US lv_palette_main(LV_PALETTE_CYAN)

extern lv_subject_t oat_tmp, oat_hum, env_tmp, env_hum, wifi_color,  sdcard_status, ble_color;
extern lv_subject_t battery_all, battery_color, battery_label;
