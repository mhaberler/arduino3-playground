#include "ui.h"
#include "ui_compass.h"
#include "lv_subjects.hpp"
#include "Esp.h"

void ui_custom_init(void)
{
    lv_label_set_text(ui_BLEStatus, LV_SYMBOL_BLUETOOTH);
    lv_label_set_text(ui_BatteryStatus, LV_SYMBOL_BATTERY_EMPTY);
    lv_label_set_text(ui_SdCardStatus, LV_SYMBOL_SD_CARD);
    lv_label_set_text(ui_WifiStatus, LV_SYMBOL_WIFI);
    lv_label_set_text(ui_GPSStatus, LV_SYMBOL_GPS);

    lv_compass_display(ui_Compass);

    lv_button_bind_checked(ui_runCompassAnimation, &runCompassAnimation);
    lv_button_bind_checked(ui_runCompassAnimation, &runCompassAnimation);
    lv_button_bind_checked(ui_BLEStatus, &ble_traffic);
    lv_button_bind_checked(ui_SdCardStatus, &sdcard_status);
    lv_slider_bind_value(ui_animationSpeed, &animationSpeed);
}

// callbacks from the UI
extern "C"
{
    void showMemory(lv_event_t *e)
    {
        LV_LOG_USER("--->  emitEvent\n");
        LV_LOG_USER("free heap: %lu\n", ESP.getFreeHeap());
        LV_LOG_USER("used psram: %lu\n", ESP.getPsramSize() - ESP.getFreePsram());
    }

    void mainScreenLoaded(lv_event_t *e)
    {
        LV_LOG_USER("xxx");
    }

    void statusScreenLoaded(lv_event_t *e)
    {
        lv_label_set_text_fmt(ui_statusText, "free heap: %lu\nused psram: %lu\n",
                              ESP.getFreeHeap(), ESP.getPsramSize() - ESP.getFreePsram());
    }
}
