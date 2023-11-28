
#include "defs.hpp"
#include "Esp.h"
#include "ui.h"
#include "lv_setup.hpp"
#include "ui_compass.h"

extern "C"
{
    void showMemory(lv_event_t *e)
    {
        LV_LOG_USER("--->  emitEvent\n");
        LV_LOG_USER("free heap: %lu\n", ESP.getFreeHeap());
        LV_LOG_USER("used psram: %lu\n", ESP.getPsramSize() - ESP.getFreePsram());
    }

    void mainScreenLoaded(lv_event_t *e)
    {
        LV_LOG_USER("xxx");
    }
}