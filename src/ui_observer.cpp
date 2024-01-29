#include "ui_observer.hpp"
#include "lv_util.h"
#include "lv_observer.h"
#include "lv_subjects.hpp"

bool sendUiMessage(const JsonDocument &jdoc) {
    if (jdoc.isNull())
        return false;
    lvgl_acquire();
    size_t ss = lv_subject_get_string_size(&uiMessage);
    size_t n = serializeJson(jdoc, lv_subject_get_string_buf(&uiMessage), ss);
    lv_subject_notify(&uiMessage);
    lvgl_release();
    return n < ss;
}

void sendUiMessage(const char *msg) {
    lvgl_acquire();
    lv_subject_copy_string(&uiMessage, msg);
    lv_subject_notify(&uiMessage);
    lvgl_release();
}