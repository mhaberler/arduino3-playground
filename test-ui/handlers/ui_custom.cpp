

#include "ui.h"
#include "ui_helpers.h"

void ui_custom_init(void)
{
    lv_label_set_text(ui_BLEStatus, LV_SYMBOL_BLUETOOTH);
    lv_label_set_text(ui_BatteryStatus, LV_SYMBOL_BATTERY_EMPTY);
    lv_label_set_text(ui_SdCardStatus, LV_SYMBOL_SD_CARD);
    lv_label_set_text(ui_WifiStatus, LV_SYMBOL_WIFI);   
}