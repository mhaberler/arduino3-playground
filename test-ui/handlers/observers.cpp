

#include <lvgl.h>
#include "lv_setup.hpp"
#include "subjects.hpp"
#include "defs.hpp"
#include "ui_setters.hpp"
#include "ui_events.h"
#include "ui.h"
#include "lv_util.h"

extern lv_obj_t *ui_SdCardStatus;
extern lv_obj_t *ui_BatteryStatus;
extern lv_obj_t *ui_BLEStatus;
extern lv_obj_t *ui_WifiStatus;

static lv_timer_t *na_timer;

lv_subject_t oat_tmp, oat_hum, env_tmp, env_hum, wifi_status, battery_status, sdcard_status, ble_status;

// ruuvi interval 60s

static transient_subject_t oat_temp_fmt = {&oat_tmp, "%.1f°", "n/a", 65 * 1000, &ui_outsideTemp, 0};
static transient_subject_t oat_hum_fmt = {&oat_hum, "%.1f%%", "n/a", 65 * 1000, &ui_outsideHum, 0};
static transient_subject_t env_temp_fmt = {&env_tmp, "%.1f°", "n/a", 65 * 1000, &ui_envTemp, 0};
static transient_subject_t env_hum_fmt = {&env_hum, "%.1f%%", "n/a", 65 * 1000, &ui_envHum, 0};

static void value_available_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    transient_subject_t *tf = (transient_subject_t *)observer->user_data;
    if (tf->user_data == NULL)
        return; // object not initialized (yet)
    lv_obj_t *target = *(lv_obj_t **)tf->user_data;

    tf->last_heard_ms = millis();
    bool valid = lv_subject_get_type(subject) == LV_SUBJECT_TYPE_INT;

    const char *fmt = valid ? tf->available : tf->unavailable;
    int32_t value = lv_subject_get_int(subject);
    // LV_LOG_USER(fmt, ITOD100(value));
    lv_label_set_text_fmt(target, fmt, ITOD100(value));
}

static void wifi_status_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    if (!subject)
        return;
    LV_LOG_USER("status %ld", lv_subject_get_int(subject));
}

static void battery_status_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    if (!subject)
        return;
    LV_LOG_USER("status %ld", lv_subject_get_int(subject));
}

static void sdcard_status_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    if (!subject)
        return;
    LV_LOG_USER("status %ld", lv_subject_get_int(subject));
}

static void ble_status_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    if (!subject)
        return;
    LV_LOG_USER("status %ld", lv_subject_get_int(subject));
    // lv_obj_clear_state(ui_BLEStatus, LV_STATE_CHECKED);
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
}

void init_value_expiry(void)
{
    lv_timer_create(expire_values, 1000, NULL);
}

void register_observers(void)
{
    LV_ASSERT_MEM_INTEGRITY();

    lv_subject_add_observer(&oat_tmp, value_available_cb, &oat_temp_fmt);
    LV_ASSERT_MEM_INTEGRITY();

    lv_subject_add_observer(&oat_hum, value_available_cb, &oat_hum_fmt);
    LV_ASSERT_MEM_INTEGRITY();

    lv_subject_add_observer(&env_tmp, value_available_cb, &env_temp_fmt);
    LV_ASSERT_MEM_INTEGRITY();

    lv_subject_add_observer(&env_hum, value_available_cb, &env_hum_fmt);
    LV_ASSERT_MEM_INTEGRITY();

    lv_subject_add_observer(&wifi_status, wifi_status_cb, ui_WifiStatus);
    LV_ASSERT_MEM_INTEGRITY();

    lv_subject_add_observer(&battery_status, battery_status_cb, ui_BatteryStatus);
    LV_ASSERT_MEM_INTEGRITY();

    lv_subject_add_observer(&sdcard_status, sdcard_status_cb, ui_SdCardStatus);
    LV_ASSERT_MEM_INTEGRITY();

    lv_subject_add_observer(&ble_status, ble_status_cb, ui_BLEStatus);
    LV_ASSERT_MEM_INTEGRITY();
}

void subjects_init(void)
{
    lv_subject_init_none(&oat_tmp);
    lv_subject_init_none(&oat_hum);
    lv_subject_init_none(&env_tmp);
    lv_subject_init_none(&env_hum);
    lv_subject_init_int(&wifi_status, STATUS_WIFI_UNCONFIGURED);
    lv_subject_init_int(&battery_status, STATUS_BATTERY_DISCONNECTED);
    lv_subject_init_int(&sdcard_status, STATUS_SDCARD_MISSING);
    lv_subject_init_int(&ble_status, 0);
}

void lv_observer_init(void)
{
    lvgl_acquire();
    subjects_init();

    register_observers();
    init_value_expiry();
    lvgl_release();
}

void lv_observer_register(void)
{
    lvgl_acquire();
    subjects_init();
    register_observers();
    init_value_expiry();
    lvgl_release();
}
