#include "ui.h"
#include "ui_compass.h"
#include "lv_subjects.hpp"
#include "lv_util.h"
#include "Esp.h"
#include "nfc_input.h"

uint32_t BW_EVENT_TAG_NO_MATCH,
         BW_EVENT_RUUVI,
         BW_EVENT_RUUVI_OAT,
         BW_EVENT_RUUVI_ENV,
         BW_EVENT_TANK,
         BW_EVENT_BURNER,
         BW_EVENT_FLOWSENSOR,
         BW_EVENT_PRESSURESENSOR,
         BW_EVENT_BAROSENSOR;

void bw_event_MAIN_PAGE( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    void * param = lv_event_get_param(e);
    LV_LOG_USER("--->  bw_event_MAIN_PAGE %d '%s'\n", event_code, param? param:"NULL");

}

void ui_custom_init(void) {
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
    lv_obj_bind_text_style_color(ui_WifiStatus, &wifi_color, LV_PART_MAIN | LV_STATE_DEFAULT);

    BW_EVENT_TAG_NO_MATCH= lv_event_register_id();
    BW_EVENT_RUUVI= lv_event_register_id();
    BW_EVENT_RUUVI_OAT= lv_event_register_id();
    BW_EVENT_RUUVI_ENV= lv_event_register_id();
    BW_EVENT_TANK= lv_event_register_id();
    BW_EVENT_BURNER= lv_event_register_id();
    BW_EVENT_FLOWSENSOR= lv_event_register_id();
    BW_EVENT_PRESSURESENSOR= lv_event_register_id();
    BW_EVENT_BAROSENSOR =  lv_event_register_id();
    
    lv_obj_add_event_cb(ui_Main, bw_event_MAIN_PAGE, (lv_event_code_t)BW_EVENT_TAG_NO_MATCH, NULL);   /*Assign an event callback*/
    lv_obj_add_event_cb(ui_Main, bw_event_MAIN_PAGE, (lv_event_code_t)BW_EVENT_RUUVI, NULL);   /*Assign an event callback*/
    lv_obj_add_event_cb(ui_Main, bw_event_MAIN_PAGE, (lv_event_code_t)BW_EVENT_TANK, NULL);   /*Assign an event callback*/
}

// callbacks from the UI
extern "C"
{
    void showMemory(lv_event_t *e) {
        LV_LOG_USER("--->  emitEvent\n");
        LV_LOG_USER("free heap: %lu\n", ESP.getFreeHeap());
        LV_LOG_USER("used psram: %lu\n", ESP.getPsramSize() - ESP.getFreePsram());
    }

    void mainScreenLoaded(lv_event_t *e) {
        LV_LOG_USER("xxx");
    }

    void statusScreenLoaded(lv_event_t *e) {
        lv_label_set_text_fmt(ui_statusText, "free heap: %lu\nused psram: %lu\n",
                              ESP.getFreeHeap(), ESP.getPsramSize() - ESP.getFreePsram());
    }

    void nfcEvent(lv_event_t *e) {
        LV_LOG_USER("nfcEvent: %lu\n", e->code);
    }

    void longPressed(lv_event_t *e) {
        LV_LOG_USER("longPressed: %lu\n", e->code);
    }

}
