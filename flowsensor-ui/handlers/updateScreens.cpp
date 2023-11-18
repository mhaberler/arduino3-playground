#include "M5Unified.h"
#include "ui.h"
#include "Esp.h"

void update_screen(void)
{
    char buf[128];

    if (lv_scr_act() == ui_Main)
    {
        Serial.printf("--->  update ui_Main\n");

        lv_bar_set_value(ui_Battery, M5.Power.getBatteryLevel(), LV_ANIM_OFF);

        switch (M5.Power.isCharging())
        {
        case m5::Power_Class::is_discharging:
            lv_obj_set_style_bg_color(ui_Battery, lv_color_hex(0x12EEEC),
                                      LV_PART_INDICATOR);
            break;
        case m5::Power_Class::is_charging:
            lv_obj_set_style_bg_color(ui_Battery, lv_color_hex(0x09CE46),
                                      LV_PART_INDICATOR);
            break;
        case m5::Power_Class::charge_unknown:
            lv_obj_set_style_bg_color(ui_Battery, lv_color_hex(0xFADD18),
                                      LV_PART_INDICATOR);
            break;
        }
    }
    if (lv_scr_act() == ui_second)
    {
        Serial.printf("---> update ui_second \n");
        sprintf(buf, "free heap: %lu", ESP.getFreeHeap());
        lv_label_set_text(ui_freeHeap, buf);

        sprintf(buf, "free psram: %lu", ESP.getFreePsram());
        lv_label_set_text(ui_psRam, buf);

        sprintf(buf, "used psram: %lu", ESP.getPsramSize() - ESP.getFreePsram());
        lv_label_set_text(ui_usedPSRam, buf);
    }
}
