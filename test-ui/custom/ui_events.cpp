
#include "defs.hpp"
#include "Esp.h"
#include "ui.h"
#include "lv_setup.hpp"
#include "ui_compass.h"

extern "C"
{
    void emitEvent(lv_event_t *e)
    {
        Serial.printf("--->  emitEvent=%d target=%p userdata=%p\n", e->code, e->current_target, e->user_data);
        Serial.printf("free heap: %lu\n", ESP.getFreeHeap());
        Serial.printf("used psram: %lu\n", ESP.getPsramSize() - ESP.getFreePsram());
    }

static bool once = false;
    void mainScreenLoaded(lv_event_t *e)
    {
        LV_LOG_USER("xxx");
        if (!once) {
            // lv_init_compass_observers();
            once = true;
        }
    }
}