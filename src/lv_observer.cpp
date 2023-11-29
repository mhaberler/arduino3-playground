#include <lvgl.h>
#include "hal/lv_hal_tick.h"
#include "lv_setup.hpp"
#include "lv_subjects.hpp"
#include "ui_events.h"
#include "ui.h"
#include "lv_util.h"
#include "ruuvi.h"

extern lv_obj_t *ui_SdCardStatus;
extern lv_obj_t *ui_BatteryStatus;
extern lv_obj_t *ui_BLEStatus;
extern lv_obj_t *ui_WifiStatus;

static lv_timer_t *na_timer;

static void ruuvi_report_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    const char *fmt = (const char *)observer->user_data;
    uint32_t last_heard = (uint32_t)subject->user_data;
    int32_t value = lv_subject_get_int(subject);

    if ((last_heard == 0) || (lv_tick_elaps(last_heard) > RUUVI_PERIOD))
    {
        fmt = NOT_AVAILABLE;
    }
    else
    {
        if (lv_subject_get_previous_int(subject) == value) // no change
            return;
    }
    lv_obj_t *target = (lv_obj_t *)lv_observer_get_target(observer);
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

static void expire_values(lv_timer_t *timer)
{
    lv_subject_notify(&oat_temp);
    lv_subject_notify(&oat_hum);
    lv_subject_notify(&env_temp);
    lv_subject_notify(&env_hum);
}

static void init_value_expiry(void)
{
    lv_timer_create(expire_values, 1000, NULL);
}

static void register_observers(void)
{
    lv_subject_add_observer_with_target(&oat_temp, ruuvi_report_cb, ui_outsideTemp, "%.1f°");
    lv_subject_add_observer_with_target(&oat_hum, ruuvi_report_cb, ui_outsideHum, "%.1f%%");
    lv_subject_add_observer_with_target(&env_temp, ruuvi_report_cb, ui_envTemp, "%.1f°");
    lv_subject_add_observer_with_target(&env_hum, ruuvi_report_cb, ui_envHum, "%.1f%%");

    lv_subject_add_observer_with_target(&wifi_color, set_color_cb, ui_WifiStatus, NULL);

    lv_subject_add_observer_with_target(&sdcard_status, sdcard_status_cb, ui_SdCardStatus, NULL);
    lv_subject_add_observer_with_target(&ble_color, set_color_cb, ui_BLEStatus, NULL);

    lv_subject_add_observer_with_target(&battery_all, battery_group_cb, ui_BatteryStatus, NULL);
}

void lv_observer_init(void)
{
    lvgl_acquire();
    register_observers();
    init_value_expiry();
    lvgl_release();
}
