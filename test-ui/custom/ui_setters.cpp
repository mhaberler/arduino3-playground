
#include "defs.hpp"
#include "Esp.h"
#include "ui.h"
#include "ui_setters.hpp"

void lv_update_battery(int32_t v)
{
    LV_LOG_USER("lv_update_battery %ld", v);
}

void lv_update_wifi_status(int32_t v)
{
    LV_LOG_USER("lv_update_wifi_status %ld", v);
}

void lv_update_ble_status(int32_t v)
{
    LV_LOG_USER("lv_update_ble_status %ld", v);
}