
#if defined(LVGL_DEMO)

#include "defs.hpp"
#include <lvgl.h>

#include "events.hpp"
#include "ui.h"
#include "esp_log.h"

static void lv_update_battery(uint batval);
const char *TAG = __FILE__;

// UI changed. cause action in main thread
static void view_change_cb(void *s, lv_msg_t *m)
{
    unsigned int msg_id = lv_msg_get_id(m);
    const char *msg_payload = (const char *)lv_msg_get_payload(m);
    const char *msg_data = (const char *)lv_msg_get_user_data(m);

    Serial.printf("view_change_cb %u\n", msg_id);

    switch (msg_id)
    {
    case MSG_PAGE_HOME:
        break;
    case MSG_PAGE_PARAMS:
        break;
    case MSG_PAGE_STATUS:
        break;
    case MSG_COUNT_RESET:
        break;
    default:;
    }
}

static void model_change_cb(void *s, lv_msg_t *m)
{
    unsigned int msg_id = lv_msg_get_id(m);
    const char *msg_payload = (const char *)lv_msg_get_payload(m);
    const char *msg_data = (const char *)lv_msg_get_user_data(m);

    Serial.printf("model_change_cb %u\n", msg_id);

    switch (msg_id)
    {
    case MSG_BATTERY_STATUS:
    {
        int battery_val = *(int *)lv_msg_get_payload(m);
        lv_update_battery(battery_val);
    }
    break;
    case MSG_TIME_CHANGED:
        break;
    case MSG_SCARD_STATUS:
        break;
    case MSG_GPS_FIX:
        break;
    case MSG_POSITION_UPDATE:
        break;
    case MSG_ENV_UPDATE:
        break;
    case MSG_OAT_UPDATE:
        break;
    case MSG_ALTITUDE_UPDATE:
        break;
    case MSG_PRESSURE_UPDATE:
        break;
    case MSG_HEADING_UPDATE:
        break;
    case MSG_SPEED_UPDATE:
        break;
    case MSG_WIFI_UNCONFIGURED:
        lv_obj_set_style_text_color(ui_WifiStatus, lv_palette_main(LV_PALETTE_AMBER), LV_PART_MAIN | LV_STATE_DEFAULT);
        break;
    case MSG_WIFI_SCAN_COMPLETE:
        lv_obj_set_style_text_color(ui_WifiStatus, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
        break;
    case MSG_WIFI_CONNECTED:
        lv_obj_set_style_text_color(ui_WifiStatus, lv_palette_main(LV_PALETTE_CYAN), LV_PART_MAIN | LV_STATE_DEFAULT);
        break;
    case MSG_WIFI_DISCONNECTED:
        lv_obj_set_style_text_color(ui_WifiStatus, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_MAIN | LV_STATE_DEFAULT);
        break;
    case MSG_COUNT_CHANGED:
        break;
    default:;
    }
}

// simulated model updates
static int battery_value = 0;
static lv_style_t style_battery;
static lv_timer_t *timer_battery;

static void timer_battery_callback(lv_timer_t *timer)
{
    // Battery icon animation
    if (battery_value > 100)
        battery_value = 0;
    battery_value += 10;
    // from a non-lvgl thread, use lvgl_msg_send_prot()
    lv_msg_send(MSG_BATTERY_STATUS, &battery_value);
}

void lv_updates_init(void)
{
    // BATTERY
    lv_label_set_text(ui_BatteryStatus, LV_SYMBOL_BATTERY_EMPTY);
    timer_battery = lv_timer_create(timer_battery_callback, 1000, NULL);
}

void lv_events_init(void)
{

    lv_msg_subsribe(MSG_PAGE_HOME, view_change_cb, NULL);
    lv_msg_subsribe(MSG_PAGE_PARAMS, view_change_cb, NULL);
    lv_msg_subsribe(MSG_PAGE_STATUS, view_change_cb, NULL);
    lv_msg_subsribe(MSG_COUNT_RESET, view_change_cb, NULL);

    lv_msg_subsribe(MSG_BATTERY_STATUS, model_change_cb, NULL);
    lv_msg_subsribe(MSG_TIME_CHANGED, model_change_cb, NULL);
    lv_msg_subsribe(MSG_SCARD_STATUS, model_change_cb, NULL);

    lv_msg_subsribe(MSG_GPS_FIX, model_change_cb, NULL);
    lv_msg_subsribe(MSG_POSITION_UPDATE, model_change_cb, NULL);
    lv_msg_subsribe(MSG_ENV_UPDATE, model_change_cb, NULL);
    lv_msg_subsribe(MSG_OAT_UPDATE, model_change_cb, NULL);
    lv_msg_subsribe(MSG_ALTITUDE_UPDATE, model_change_cb, NULL);
    lv_msg_subsribe(MSG_PRESSURE_UPDATE, model_change_cb, NULL);
    lv_msg_subsribe(MSG_HEADING_UPDATE, model_change_cb, NULL);
    lv_msg_subsribe(MSG_SPEED_UPDATE, model_change_cb, NULL);

    lv_msg_subsribe(MSG_WIFI_UNCONFIGURED, model_change_cb, NULL);
    lv_msg_subsribe(MSG_WIFI_STARTED, model_change_cb, NULL);
    lv_msg_subsribe(MSG_WIFI_SCAN_COMPLETE, model_change_cb, NULL);
    lv_msg_subsribe(MSG_WIFI_CONNECTED, model_change_cb, NULL);
    lv_msg_subsribe(MSG_WIFI_DISCONNECTED, model_change_cb, NULL);

    lv_msg_subsribe(MSG_COUNT_CHANGED, model_change_cb, NULL);
}

static void lv_update_battery(uint batval)
{
    if (batval < 20)
    {
        lv_obj_set_style_text_color(ui_BatteryStatus, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(ui_BatteryStatus, LV_SYMBOL_BATTERY_EMPTY);
    }
    else if (batval < 50)
    {
        lv_obj_set_style_text_color(ui_BatteryStatus, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(ui_BatteryStatus, LV_SYMBOL_BATTERY_1);
    }
    else if (batval < 70)
    {
        lv_obj_set_style_text_color(ui_BatteryStatus, lv_palette_main(LV_PALETTE_DEEP_ORANGE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(ui_BatteryStatus, LV_SYMBOL_BATTERY_2);
    }
    else if (batval < 90)
    {
        lv_obj_set_style_text_color(ui_BatteryStatus, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(ui_BatteryStatus, LV_SYMBOL_BATTERY_3);
    }
    else
    {
        lv_obj_set_style_text_color(ui_BatteryStatus, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(ui_BatteryStatus, LV_SYMBOL_BATTERY_FULL);
    }
}

#endif