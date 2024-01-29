#pragma once
#include "ArduinoJson.h"

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

    UM_SENSOR_ENVELOPE,
    UM_SENSOR_OAT,

} uiMessage_t;

bool sendUiMessage(const JsonDocument &jdoc);
void sendUiMessage(const char *msg);
