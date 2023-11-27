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

lv_subject_t oat_tmp, oat_hum, env_tmp, env_hum, wifi_color, http_status, sdcard_status, ble_color;
lv_subject_t battery_all, battery_color, battery_label;

static transient_subject_t oat_temp_fmt = {&oat_tmp, "%.1f°", "n/a", RUUVI_PERIOD, 0};
static transient_subject_t oat_hum_fmt = {&oat_hum, "%.1f%%", "n/a", RUUVI_PERIOD, 0};
static transient_subject_t env_temp_fmt = {&env_tmp, "%.1f°", "n/a", RUUVI_PERIOD, 0};
static transient_subject_t env_hum_fmt = {&env_hum, "%.1f%%", "n/a", RUUVI_PERIOD, 0};

static void value_available_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    transient_subject_t *tf = (transient_subject_t *)observer->user_data;
    lv_obj_t *target = (lv_obj_t *)lv_observer_get_target(observer);

    tf->last_heard_ms = millis();
    bool valid = lv_subject_get_type(subject) == LV_SUBJECT_TYPE_INT;

    const char *fmt = valid ? tf->available : tf->unavailable;
    int32_t value = lv_subject_get_int(subject);
    // LV_LOG_USER(fmt, ITOD100(value));
    lv_label_set_text_fmt(target, fmt, ITOD100(value));
}

static void set_color_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    lv_color_t c = lv_subject_get_color(subject);
    lv_obj_t *target = (lv_obj_t *)lv_observer_get_target(observer);
    lv_obj_set_style_text_color(target, c, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void sdcard_status_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    int32_t v = lv_subject_get_int(subject);
    if (v == lv_subject_get_previous_int(subject))
        return;
    LV_LOG_USER("status %ld", lv_subject_get_int(subject));
}

static lv_subject_t *battery_list[] = {&battery_label, &battery_color};

static void battery_group_cb(lv_subject_t *subject, lv_observer_t *observer)
{
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
    // lv_subject_set_color(&ble_color, STATUS_BLE_IDLE);
    // lv_subject_set_color(&wifi_color, lv_subject_get_previous_color(&wifi_color));
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
    lv_subject_add_observer_with_target(&wifi_color, set_color_cb, ui_WifiStatus, NULL);

    lv_subject_add_observer_with_target(&sdcard_status, sdcard_status_cb, ui_SdCardStatus, NULL);
    lv_subject_add_observer_with_target(&ble_color, set_color_cb, ui_BLEStatus, NULL);

    lv_subject_add_observer_with_target(&battery_all, battery_group_cb, ui_BatteryStatus, NULL);
}

static void subjects_init(void)
{
    lv_subject_init_none(&oat_tmp);
    lv_subject_init_none(&oat_hum);
    lv_subject_init_none(&env_tmp);
    lv_subject_init_none(&env_hum);

    lv_subject_init_color(&wifi_color, STATUS_WIFI_UNCONFIGURED);
    lv_subject_init_color(&ble_color, STATUS_BLE_IDLE);
    lv_subject_init_int(&sdcard_status, STATUS_SDCARD_MISSING);

    lv_subject_init_color(&battery_color, lv_palette_main(LV_PALETTE_RED));
    lv_subject_init_pointer(&battery_label, (void *)LV_SYMBOL_BATTERY_EMPTY);
    lv_subject_init_group(&battery_all, battery_list, 2);
}

void lv_observer_init(void)
{
    lvgl_acquire();
    subjects_init();
    register_observers();
    init_value_expiry();
    lvgl_release();
}
