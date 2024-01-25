#include <lvgl.h>
#include "hal/lv_hal_tick.h"
#include "lv_setup.hpp"
#include "lv_subjects.hpp"
#include "ui_events.h"
#include "ui.h"
#include "lv_util.h"
#include "ruuvi.h"
#include "nfc_input.h"
#include "ArduinoJsonCustom.h"
#include "Sensor.hpp"

extern lv_obj_t *ui_SdCardStatus;
extern lv_obj_t *ui_BatteryStatus;
extern lv_obj_t *ui_BLEStatus;
extern lv_obj_t *ui_WifiStatus;

static lv_timer_t *na_timer;

static void ruuvi_report_cb(lv_subject_t *subject, lv_observer_t *observer) {
    const char *fmt = (const char *)observer->user_data;
    uint32_t last_heard = (uint32_t)subject->user_data;
    int32_t value = lv_subject_get_int(subject);

    if ((last_heard == 0) || (lv_tick_elaps(last_heard) > RUUVI_PERIOD)) {
        fmt = NOT_AVAILABLE;
    } else {
        if (lv_subject_get_previous_int(subject) == value) // no change
            return;
    }
    lv_obj_t *target = (lv_obj_t *)lv_observer_get_target(observer);
    lv_label_set_text_fmt(target, fmt, ITOD100(value));
}

static void battery_group_cb(lv_subject_t *subject, lv_observer_t *observer) {
    lv_obj_t *target = (lv_obj_t *)lv_observer_get_target(observer);
    lv_subject_t *subject_label = lv_subject_get_group_element(subject, 0);
    lv_subject_t *subject_color = lv_subject_get_group_element(subject, 1);

    const char *label = (const char *)lv_subject_get_pointer(subject_label);
    lv_color_t color = lv_subject_get_color(subject_color);

    if (compare_colors(lv_subject_get_previous_color(subject_color), color))
        lv_obj_set_style_text_color(target, color, (lv_style_selector_t)subject_color->user_data);

    if (label != lv_subject_get_previous_pointer(subject_label))
        lv_label_set_text(target, label);
}

static lv_obj_t *previous_screen = ui_Main;
static JsonDocument *jdoc;

// callbacks from the UI
extern "C"
{
    void returnToPrevious(lv_event_t *e) {
        LV_LOG_USER("%lu\n", e->code);
        lv_disp_load_scr(previous_screen);
    }

    void setEnvelopeMac(lv_event_t *e)  {
        std::string mac((*jdoc)["payload"]["MAC"]);

        LV_LOG_USER("%lu  '%s'\n", e->code, mac.c_str());

        Sensor *sp = new Ruuvi(mac);

        Unit *u = new Unit("envelope");
        u->add(sp);

        // Unit *u = setupUnit(UT_ENVELOPE, ST_RUUVI, mac);

        lv_disp_load_scr(ui_Main);

    }

    void setOATMac(lv_event_t *e) {
        std::string mac((*jdoc)["payload"]["MAC"]);

        LV_LOG_USER("%lu  '%s'\n", e->code, mac.c_str());

        // Unit *u = setupUnit(UT_OAT, ST_RUUVI, mac);
        // setupSensor(mac, FT_TEMPERATURE, &oat_temp);
        // setupSensor(mac, FT_HUMIDITY, &oat_hum);

        lv_disp_load_scr(ui_Main);

    }

    void setUnit(lv_event_t * e) {
        std::string id((*jdoc)["payload"]["id"]);
        LV_LOG_USER("%lu  '%s'\n", e->code, id.c_str());
        addUnit((*jdoc)["payload"]);
        lv_disp_load_scr(ui_Main);
    }

}

static void nfc_message_cb(lv_subject_t *subject, lv_observer_t *observer) {
    uint32_t code = (uint32_t)subject->user_data;
    // lv_obj_t *target = (lv_obj_t *)lv_observer_get_target(observer);
    jdoc = (JsonDocument *)lv_subject_get_pointer(subject);

    switch (code) {
        case BWTAG_NO_MATCH:
            // beep
            break;
        case BWTAG_RUUVI:
        case BWTAG_RUUVI_OAT:
        case BWTAG_RUUVI_ENV: {
                previous_screen = lv_scr_act();

                lv_label_set_text(ui_ruuviHeader, "Ruuvi Sensor detected");
                String mac =  (*jdoc)["payload"]["MAC"];
                String sw =  (*jdoc)["payload"]["SW"];
                lv_label_set_text_fmt(ui_ruuviBody, "\nMAC: %s\nFirmware: %s",
                                      mac.c_str(), sw.c_str());
                // lv_obj_add_event_cb(ui_Envelope, setEnvelopeMac, LV_EVENT_CLICKED, jdoc);
                // lv_obj_add_event_cb(ui_OAT, setOATMac, LV_EVENT_CLICKED, jdoc);
                lv_disp_load_scr(ui_Ruuvi);

            }
            break;
        case BWTAG_TANK: {
                previous_screen = lv_scr_act();
                String id =  (*jdoc)["payload"]["id"];
                String dsc =  (*jdoc)["payload"]["dsc"];
                int32_t ut = (*jdoc)["payload"]["ut"];
                lv_label_set_text_fmt(ui_unitHeader, "%s %s %s",
                                      unitText(ut), id.c_str(), dsc.c_str());
                String stxt = "\n";
                JsonArray sensors = (*jdoc)["payload"]["sensors"].as<JsonArray>();
                for(JsonObject s: sensors) {
                    stxt += sensorTypeText(s["st"].as<int>());
                    stxt += " " + s["mac"].as<String>();
                    stxt += "\n";
                }
                lv_label_set_text(ui_unitBody, stxt.c_str());
                String rem =  (*jdoc)["payload"]["rem"];
                if (rem.length()) {
                    lv_label_set_text(ui_unitColor, rem.c_str());
                    lv_obj_clear_flag(ui_unitColor, LV_OBJ_FLAG_HIDDEN);
                }
                String col =  (*jdoc)["payload"]["col"];
                if (col.length()) {
                    lv_color_t tagcolor = lv_color_from_sharpRGB(col.c_str());
                    lv_obj_clear_flag(ui_unitColor, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_set_style_bg_color(ui_unitColor, tagcolor, LV_PART_MAIN | LV_STATE_DEFAULT );
                    lv_obj_set_style_bg_opa(ui_unitColor, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
                }
                lv_disp_load_scr(ui_Unit);
            }
            break;
        default:
            break;
    }
}

static void register_observers(void) {
    lv_subject_add_observer_with_target(&oat_temp, ruuvi_report_cb, ui_outsideTemp, (void*)"%.1f°");
    lv_subject_add_observer_with_target(&oat_hum, ruuvi_report_cb, ui_outsideHum, (void*)"%.1f%%");
    lv_subject_add_observer_with_target(&env_temp, ruuvi_report_cb, ui_envTemp,(void*) "%.1f°");
    lv_subject_add_observer_with_target(&env_hum, ruuvi_report_cb, ui_envHum, (void*)"%.1f%%");

    lv_subject_add_observer_with_target(&battery_all, battery_group_cb, ui_BatteryStatus, NULL);
    lv_subject_add_observer_with_target(&nfcMessage, nfc_message_cb, ui_Main, NULL);
}

void lv_observer_init(void) {
    lvgl_acquire();
    register_observers();
    lvgl_release();
}
