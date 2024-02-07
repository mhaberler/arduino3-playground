#pragma once

#include <lvgl.h>
#include "lv_observer.h"
#include "ui_compass.h"


#define UI_MESSAGESIZE 8192

#define MAX_TANKS 12
#define MAX_PRESSURE 15
#define MAX_LEVEL 90


#define STATUS_WIFI_UNCONFIGURED lv_palette_main(LV_PALETTE_GREY)
#define STATUS_WIFI_STARTED lv_palette_main(LV_PALETTE_AMBER)
#define STATUS_WIFI_SCAN_COMPLETE lv_palette_main(LV_PALETTE_GREEN)
#define STATUS_WIFI_CONNECTED lv_palette_main(LV_PALETTE_TEAL)
#define STATUS_WIFI_GOT_IP lv_palette_main(LV_PALETTE_GREEN)
#define STATUS_WIFI_LOST_IP lv_palette_main(LV_PALETTE_DEEP_ORANGE)
#define STATUS_WIFI_DISCONNECTED lv_palette_main(LV_PALETTE_RED)
#define STATUS_WIFI_TRAFFIC lv_palette_main(LV_PALETTE_LIGHT_GREEN)

#define F2I100(x) ((int)(((double)x) * 100.0))
#define ITOD100(x) ((double)(x) / 100.0)
#define NOT_AVAILABLE "n/a"

#include "nfc_input.h"

typedef enum {
    UM_NONE,

    UM_NFCMSG_NO_MATCH = BWTAG_NO_MATCH,
    UM_NFCMSG_RUUVI = BWTAG_RUUVI,
    UM_NFCMSG_PROXY_TAG = BWTAG_PROXY_TAG,

    UM_STATUS_BATTERY,
    UM_STATUS_SDCARD,
    UM_STATUS_BLE,
    UM_STATUS_WIFI,
    UM_STATUS_NFC,

    UM_SENSOR_GPS,
    UM_SENSOR_BAROMETR,
    UM_SENSOR_IMU,

    UM_SENSOR_ENVELOPE, //11
    UM_SENSOR_OAT,
    UM_SENSOR_TANK,

    UM_SENSOR_TANK_LAYOUT,
} uiMessage_t;


extern lv_subject_t oat_temp, oat_hum, env_temp, env_hum, wifi_color, 
     sdcard_status, ble_traffic;
extern lv_subject_t runCompassAnimation, animationSpeed;
// extern lv_subject_t uiMessage;

extern uint16_t tanks_seen;
void set_level(int index, lv_coord_t value);
void set_pressure(int index, lv_coord_t value);

void lv_subjects_init(void);
