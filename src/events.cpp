
#if defined(LVGL_DEMO)

#include "defs.hpp"
#include <lvgl.h>

#include "events.hpp"
#include "ui.h"
#include "esp_log.h"

static void lv_update_battery(uint batval);
const char *TAG = __FILE__;

// // UI changed. cause action in main thread
// static void view_change_cb(void *s, lv_msg_t *m)
// {
    // unsigned int msg_id = lv_msg_get_id(m);
    // const char *msg_payload = (const char *)lv_msg_get_payload(m);
    // const char *msg_data = (const char *)lv_msg_get_user_data(m);

    // Serial.printf("view_change_cb %u\n", msg_id);

    // switch (msg_id)
    // {
    // case MSG_PAGE_HOME:
    //     break;
    // case MSG_PAGE_PARAMS:
    //     break;
    // case MSG_PAGE_STATUS:
    //     break;
    // case MSG_COUNT_RESET:
    //     break;
    // default:;
    // }
// }

// static void model_change_cb(void *s, lv_msg_t *m)
// {
//     // unsigned int msg_id = lv_msg_get_id(m);
//     // lv_obj_t *target = (lv_obj_t *)lv_msg_get_user_data(m);

//     // Serial.printf("model_change_cb %u\n", msg_id);

//     switch (msg_id)
//     {
//     case MSG_BATTERY_STATUS:
//     {
//         // int battery_val = *(int *)lv_msg_get_payload(m);
//         // lv_update_battery(battery_val);
//         // lv_obj_clear_state(ui_BLEStatus, LV_STATE_CHECKED);  // reset BLE "LED"
//     }
//     break;
//     case MSG_TIME_CHANGED:
//         break;
//     case MSG_SCARD_STATUS:
//         break;
//     case MSG_GPS_FIX:
//         break;
//     case MSG_POSITION_UPDATE:
//         break;
//     case MSG_ENV_TEMP_UPDATE:
//     case MSG_OAT_TEMP_UPDATE:
//     {
//         // const double v = *(const double *)lv_msg_get_payload(m);
//         // lv_label_set_text_fmt(target, "%.1f°", v);
//         // lv_obj_add_state(ui_BLEStatus, LV_STATE_CHECKED); // set BLE "LED"
//     }
//     break;
//     case MSG_ENV_HUM_UPDATE:
//     case MSG_OAT_HUM_UPDATE:
//     {
//         // const double v = *(const double *)lv_msg_get_payload(m);
//         // lv_label_set_text_fmt(target, "%.1f %%", v);
//         // lv_obj_add_state(ui_BLEStatus, LV_STATE_CHECKED); // set BLE "LED"
//     }
//     break;
//     case MSG_ALTITUDE_UPDATE:
//         break;
//     case MSG_PRESSURE_UPDATE:
//         break;
//     case MSG_HEADING_UPDATE:
//         break;
//     case MSG_SPEED_UPDATE:
//         break;
//     case MSG_WIFI_UNCONFIGURED:
//         // lv_obj_set_style_text_color(ui_WifiStatus, lv_palette_main(LV_PALETTE_AMBER), LV_PART_MAIN | LV_STATE_DEFAULT);
//         break;
//     case MSG_WIFI_SCAN_COMPLETE:
//         // lv_obj_set_style_text_color(ui_WifiStatus, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
//         break;
//     case MSG_WIFI_CONNECTED:
//         // lv_obj_set_style_text_color(ui_WifiStatus, lv_palette_main(LV_PALETTE_CYAN), LV_PART_MAIN | LV_STATE_DEFAULT);
//         break;
//     case MSG_WIFI_DISCONNECTED:
//         // lv_obj_set_style_text_color(ui_WifiStatus, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_MAIN | LV_STATE_DEFAULT);
//         break;
//     case MSG_COUNT_CHANGED:
//         break;
//     default:;
//     }
// }


#endif