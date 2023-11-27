
#include "defs.hpp"
#include "Esp.h"
#include "ui.h"
#include "ui_setters.hpp"

void lv_update_battery(int32_t batval)
{
    LV_LOG_USER("lv_update_battery %ld", batval);
}

void lv_update_wifi_status(int32_t wifi_status)
{
    LV_LOG_USER("lv_update_wifi_status %ld", wifi_status);
}

void lv_update_ble_status(int32_t ble_status)
{
    LV_LOG_USER("lv_update_ble_status %ld", ble_status);
}