#include <lvgl.h>
#include "hal/lv_hal_tick.h"
#include "lv_setup.hpp"
#include "lv_subjects.hpp"
#include "ui_events.h"
#include "ui_custom.hpp"
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
extern lv_subject_t uiMessage;

extern  void animate_battery_icon(int32_t batval) {
    lv_color_t color = lv_palette_main(LV_PALETTE_GREY);
    const char *label = "?";
    lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
    if (batval > -1) {
        if (batval < 20) {
            color = lv_palette_main(LV_PALETTE_RED);
            label = LV_SYMBOL_BATTERY_EMPTY;
        } else if (batval < 50) {
            color = lv_palette_main(LV_PALETTE_RED);
            label = LV_SYMBOL_BATTERY_1;
        } else if (batval < 70) {
            color = lv_palette_main(LV_PALETTE_DEEP_ORANGE);
            label = LV_SYMBOL_BATTERY_2;
        } else if (batval < 90) {
            color = lv_palette_main(LV_PALETTE_GREEN);
            label = LV_SYMBOL_BATTERY_3;
        } else {
            color = lv_palette_main(LV_PALETTE_GREEN);
            label = LV_SYMBOL_BATTERY_FULL;
        }
    }
    lv_obj_set_style_text_color(ui_BatteryStatus, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(ui_BatteryStatus, label);
}

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

static lv_obj_t *previous_screen = ui_Main;


// callbacks from the UI
extern "C"
{
    void ruuviEnvelope(lv_event_t *e)  {
        void *mac =  lv_obj_get_user_data(ui_Ruuvi);
        // https://jsontostring.com/
        const char* jstxt ="{\"id\":\"env\",\"ut\":3,\"sensors\":[{\"st\":1}]}";
        JsonDocument tmpl;
        deserializeJson(tmpl, jstxt);
        tmpl["sensors"][0]["mac"] = (const char *) mac;
        equipment.addUnit(tmpl.as<JsonObject>(), SRC_NFC);
        free(mac);
        lv_obj_set_user_data(ui_Ruuvi, NULL);
        lv_disp_load_scr(ui_Main);
    }

    void ruuviOAT(lv_event_t *e) {
        void *mac =  lv_obj_get_user_data(ui_Ruuvi);

        const char* jstxt ="{\"id\":\"oair\",\"ut\":4,\"sensors\":[{\"st\":1}]}";
        JsonDocument tmpl;
        deserializeJson(tmpl, jstxt);
        tmpl["sensors"][0]["mac"] =  (const char *) mac;
        equipment.addUnit(tmpl.as<JsonObject>(), SRC_NFC);
        free(mac);
        lv_obj_set_user_data(ui_Ruuvi, NULL);
        lv_disp_load_scr(ui_Main);
    }

    void ruuviCancel(lv_event_t *e) {
        free(lv_obj_get_user_data(ui_Ruuvi));
        lv_obj_set_user_data(ui_Ruuvi, NULL);
    }

    void setUnit(lv_event_t * e) {
        JsonDocument *jdoc = (JsonDocument *)lv_obj_get_user_data(ui_Unit);
        JsonVariant jv = (*jdoc)["payload"];
        if (jv.is<JsonArray>()) {
            JsonArray units = jv.as<JsonArray>();
            for(JsonObject u: units) {
                equipment.addUnit(u, SRC_NFC);
            }
        }
        delete jdoc;
        lv_obj_set_user_data(ui_Unit, NULL);
        lv_disp_load_scr(ui_Main);
    }

    void wipeConfig(lv_event_t * e) {
        LV_LOG_USER("wipinf Littlefs & reboot");
        wipeLittleFS();
        ESP.restart();
    }
}

static void ui_message_cb(lv_subject_t *subject, lv_observer_t *observer) {
    const char *input = lv_subject_get_string(subject);
    JsonDocument jdoc;

    // LV_LOG_USER("-> '%s'", input);

    DeserializationError error = deserializeJson(jdoc, input);
    if (error) {
        LV_LOG_ERROR("deserializeJson() failed: %s",error.f_str());
        return;
    }
    if (jdoc.isNull()) {
        LV_LOG_USER("empty jdoc");
        return;
    }

    uiMessage_t code = jdoc["um"].as<uiMessage_t>();

    switch (code) {
        case UM_NFCMSG_NO_MATCH:
            // beep
            break;
        case UM_NFCMSG_RUUVI: {
                serializeJsonPretty(jdoc, Serial);

                previous_screen = lv_scr_act();
                lv_label_set_text(ui_ruuviHeader, "Ruuvi Sensor detected");
                String mac = jdoc["payload"]["MAC"];
                String sw = jdoc["payload"]["SW"];
                lv_label_set_text_fmt(ui_ruuviBody, "\nMAC: %s\nFirmware: %s",
                                      mac.c_str(), sw.c_str());

                lv_obj_set_user_data(ui_Ruuvi, strdup(mac.c_str()));
                lv_disp_load_scr(ui_Ruuvi);
            }
            break;
        case UM_NFCMSG_PROXY_TAG: {
                serializeJsonPretty(jdoc, Serial);

                JsonVariant jv = jdoc["payload"];

                if (!jv.is<JsonArray>()) {
                    // format error
                    // beep
                    return ;
                }
                previous_screen = lv_scr_act();
                String header, body;
                lv_color_t tagcolor = getDefaultColor();
                bool colorfound = false;

                JsonArray units = jv.as<JsonArray>();
                for(JsonObject u: units) {
                    if (!colorfound && u.containsKey("col")) {
                        tagcolor = lv_color_from_sharpRGB(u["col"]);
                        colorfound = true;
                    }
                    header += unitTypeStr(u["ut"].as<unit_t>());
                    header += " ";

                    body += unitTypeStr(u["ut"].as<unit_t>());
                    body += ":";
                    body += u["id"].as<const char *>();
                    body += "\n";

                    JsonArray sensors = u["sensors"].as<JsonArray>();
                    for(JsonObject s: sensors) {
                        body += "  ";
                        body += sensorTypeStr(s["st"].as<actorType_t>());
                        body += ":";
                        body += s["mac"].as<const char *>();
                        body += "\n";
                    }
                    lv_label_set_text(ui_unitHeader, header.c_str());
                    lv_label_set_text(ui_unitBody, body.c_str());
                }

                lv_obj_set_style_bg_color(ui_UnitSave, tagcolor, LV_PART_MAIN | LV_STATE_DEFAULT );
                lv_obj_set_style_bg_opa(ui_UnitSave, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

                // retain a copy for callback
                JsonDocument *jd = new JsonDocument();
                *jd = jdoc;
                lv_obj_set_user_data(ui_Unit, jd);
                lv_disp_load_scr(ui_Unit);
            }
            break;
        case UM_STATUS_BATTERY: {
                int32_t batval = jdoc["v"].as<int32_t>();
                // LV_LOG_USER("batval %d", batval);
                animate_battery_icon(batval);
            }
            break;
        case UM_STATUS_SDCARD:
            break;
        case UM_STATUS_BLE:
            break;
        case UM_STATUS_WIFI:
            break;
        case UM_STATUS_NFC:
            break;

        case UM_SENSOR_GPS:
            break;
        case UM_SENSOR_BAROMETR:
            break;
        case UM_SENSOR_IMU:
            break;
        case UM_SENSOR_ENVELOPE:
            break;
        case UM_SENSOR_OAT:
            break;

        default:
            break;
    }
    jdoc.clear();
}
static void register_observers(void) {
    lv_subject_add_observer_with_target(&oat_temp, ruuvi_report_cb, ui_outsideTemp, (void*)"%.1f°");
    lv_subject_add_observer_with_target(&oat_hum, ruuvi_report_cb, ui_outsideHum, (void*)"%.1f%%");
    lv_subject_add_observer_with_target(&env_temp, ruuvi_report_cb, ui_envTemp,(void*) "%.1f°");
    lv_subject_add_observer_with_target(&env_hum, ruuvi_report_cb, ui_envHum, (void*)"%.1f%%");

    lv_subject_add_observer_with_target(&uiMessage, ui_message_cb, ui_Main, NULL);
}

void lv_observer_init(void) {
    lvgl_acquire();
    register_observers();
    lvgl_release();
}
