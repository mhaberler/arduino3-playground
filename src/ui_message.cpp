
#include <lvgl.h>
#include "lv_observer.h"
#include "lv_util.h"
#include "ArduinoJson.h"

extern lv_subject_t uiMessage;

bool sendUiMessage(const JsonDocument &jdoc) {
    if (jdoc.isNull())
        return false;

    size_t ss = lv_subject_get_string_size(&uiMessage);
    size_t n = serializeJson(jdoc, lv_subject_get_string_buf(&uiMessage), ss);
    lv_subject_notify(&uiMessage);
    return n < ss;
}

void sendUiMessage(const char * fmt, ...) {
    LV_ASSERT_NULL(fmt);

    va_list args;
    va_start (args, fmt);
    vsprintf (lv_subject_get_string_buf(&uiMessage), fmt, args);
    va_end (args);
    lv_subject_notify(&uiMessage);
}
