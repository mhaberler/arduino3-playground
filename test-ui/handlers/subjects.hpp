#pragma once

#include "defs.hpp"

typedef enum
{
    STATUS_WIFI_UNCONFIGURED,
    STATUS_WIFI_STARTED,
    STATUS_WIFI_SCAN_COMPLETE,
    STATUS_WIFI_CONNECTED,
    STATUS_WIFI_DISCONNECTED,
} wifi_status_t;

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

extern lv_subject_t oat_tmp, oat_hum, env_tmp, env_hum, wifi_status, battery_status, sdcard_status, ble_status;
