
#include "defs.hpp"
#include "Esp.h"
#include "ui.h"
#include "ui_setters.hpp"

void lv_update_battery(int32_t batval)
{
    LV_LOG_USER("lv_update_battery %d", batval);
}

void lv_update_wifi_status(int32_t wifi_status)
{
    LV_LOG_USER("lv_update_wifi_status %d", wifi_status);
}

void lv_update_ble_status(int32_t ble_status)
{
    LV_LOG_USER("lv_update_ble_status %d", ble_status);
}