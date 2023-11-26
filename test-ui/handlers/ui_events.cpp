
#include "defs.hpp"
#include "Esp.h"
#include "ui.h"

extern "C"
{
    void emitEvent(lv_event_t *e)
    {
        Serial.printf("--->  emitEvent=%d target=%p userdata=0x%x\n", e->code, e->current_target, e->user_data);
        Serial.printf("free heap: %lu\n", ESP.getFreeHeap());
        Serial.printf("used psram: %lu\n", ESP.getPsramSize() - ESP.getFreePsram());
    }
}