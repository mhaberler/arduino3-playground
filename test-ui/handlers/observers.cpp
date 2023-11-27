#include <lvgl.h>
#include "lv_setup.hpp"
#include "subjects.hpp"
#include "defs.hpp"
#include "ui_setters.hpp"
#include "ui_events.h"
#include "ui.h"
#include "lv_util.h"
#include "ruuvi.h"

extern lv_obj_t *ui_SdCardStatus;
extern lv_obj_t *ui_BatteryStatus;
extern lv_obj_t *ui_BLEStatus;
extern lv_obj_t *ui_WifiStatus;

static lv_timer_t *na_timer;

lv_subject_t oat_tmp, oat_hum, env_tmp, env_hum, wifi_status, http_status, battery_status, sdcard_status, ble_status;

static transient_subject_t oat_temp_fmt = {&oat_tmp, "%.1f°", "n/a", RUUVI_PERIOD, 0};
static transient_subject_t oat_hum_fmt = {&oat_hum, "%.1f%%", "n/a", RUUVI_PERIOD, 0};
static transient_subject_t env_temp_fmt = {&env_tmp, "%.1f°", "n/a", RUUVI_PERIOD, 0};
static transient_subject_t env_hum_fmt = {&env_hum, "%.1f%%", "n/a", RUUVI_PERIOD, 0};

static void value_available_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    transient_subject_t *tf = (transient_subject_t *)observer->user_data;
    // if (tf->user_data == NULL)
    //     return; // object not initialized (yet)
    lv_obj_t *target = (lv_obj_t *)lv_observer_get_target(observer);

    tf->last_heard_ms = millis();
    bool valid = lv_subject_get_type(subject) == LV_SUBJECT_TYPE_INT;

    const char *fmt = valid ? tf->available : tf->unavailable;
    int32_t value = lv_subject_get_int(subject);
    // LV_LOG_USER(fmt, ITOD100(value));
    lv_label_set_text_fmt(target, fmt, ITOD100(value));
}

static void wifi_status_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    LV_LOG_USER("status %ld", lv_subject_get_int(subject));
}

static void http_status_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    LV_LOG_USER("status %ld", lv_subject_get_int(subject));
}

static void sdcard_status_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    LV_LOG_USER("status %ld", lv_subject_get_int(subject));
}

static void ble_status_cb(lv_subject_t *subject, lv_observer_t *observer)
{

    LV_LOG_USER("status %ld", lv_subject_get_int(subject));
    // lv_obj_clear_state(ui_BLEStatus, LV_STATE_CHECKED);
}

static void battery_status_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    int32_t batval = lv_subject_get_int(subject);
    // LV_LOG_USER("status %ld", batval);
    lv_obj_t *target = (lv_obj_t *)lv_observer_get_target(observer);

    if (batval < 20)
    {
        lv_obj_set_style_text_color(target, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(target, LV_SYMBOL_BATTERY_EMPTY);
    }
    else if (batval < 50)
    {
        lv_obj_set_style_text_color(target, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(target, LV_SYMBOL_BATTERY_1);
    }
    else if (batval < 70)
    {
        lv_obj_set_style_text_color(target, lv_palette_main(LV_PALETTE_DEEP_ORANGE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(target, LV_SYMBOL_BATTERY_2);
    }
    else if (batval < 90)
    {
        lv_obj_set_style_text_color(target, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(target, LV_SYMBOL_BATTERY_3);
    }
    else
    {
        lv_obj_set_style_text_color(target, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(target, LV_SYMBOL_BATTERY_FULL);
    }
}

static void expire_fmt(uint32_t now, transient_subject_t *fmt)
{
    if (fmt->subject == NULL)
        return;
    if (now > (fmt->last_heard_ms + fmt->ttl_ms))
    {
        lv_subject_set_type(fmt->subject, LV_SUBJECT_TYPE_NONE);
        lv_subject_notify(fmt->subject);
    }
}

static void expire_values(lv_timer_t *timer)
{
    uint32_t now = millis();
    expire_fmt(now, &oat_temp_fmt);
    expire_fmt(now, &oat_hum_fmt);
    expire_fmt(now, &env_temp_fmt);
    expire_fmt(now, &env_hum_fmt);
    lv_subject_set_int(&ble_status, STATUS_BLE_IDLE);
    lv_subject_set_int(&wifi_status, STATUS_HTTP_IDLE);
}

static void init_value_expiry(void)
{
    lv_timer_create(expire_values, 1000, NULL);
}

static void register_observers(void)
{
    lv_subject_add_observer_with_target(&oat_tmp, value_available_cb, ui_outsideTemp, &oat_temp_fmt);
    lv_subject_add_observer_with_target(&oat_hum, value_available_cb, ui_outsideHum, &oat_hum_fmt);
    lv_subject_add_observer_with_target(&env_tmp, value_available_cb, ui_envTemp, &env_temp_fmt);
    lv_subject_add_observer_with_target(&env_hum, value_available_cb, ui_envHum, &env_hum_fmt);
    lv_subject_add_observer_with_target(&wifi_status, wifi_status_cb, ui_WifiStatus, NULL);
    lv_subject_add_observer_with_target(&battery_status, battery_status_cb, ui_BatteryStatus, NULL);
    lv_subject_add_observer_with_target(&sdcard_status, sdcard_status_cb, ui_SdCardStatus, NULL);
    lv_subject_add_observer_with_target(&ble_status, ble_status_cb, ui_BLEStatus, NULL);
    lv_subject_add_observer_with_target(&http_status, http_status_cb, ui_HTTPStatus, NULL);
}

static void subjects_init(void)
{
    lv_subject_init_none(&oat_tmp);
    lv_subject_init_none(&oat_hum);
    lv_subject_init_none(&env_tmp);
    lv_subject_init_none(&env_hum);
    lv_subject_init_int(&wifi_status, STATUS_WIFI_UNCONFIGURED);
    lv_subject_init_int(&battery_status, STATUS_BATTERY_DISCONNECTED);
    lv_subject_init_int(&sdcard_status, STATUS_SDCARD_MISSING);
    lv_subject_init_int(&ble_status, STATUS_BLE_IDLE);
    lv_subject_init_int(&http_status, STATUS_HTTP_STOPPED);
}

void lv_observer_init(void)
{
    lvgl_acquire();
    subjects_init();
    register_observers();
    init_value_expiry();
    lvgl_release();
}
