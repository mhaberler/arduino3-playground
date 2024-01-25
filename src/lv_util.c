
#include "lv_util.h"
#include <stdlib.h>

void lv_subject_set_type(lv_subject_t *subject, lv_subject_type_t type) {
    subject->type = type;
}

lv_subject_type_t lv_subject_get_type(lv_subject_t *subject) {
    return subject->type;
}

void lv_subject_init_none(lv_subject_t *subject) {
    lv_memzero(subject, sizeof(lv_subject_t));
    subject->type = LV_SUBJECT_TYPE_NONE;
    subject->value.num = 0;
    subject->prev_value.num = 0;
    _lv_ll_init(&(subject->subs_ll), sizeof(lv_observer_t));
}

#if LV_USE_USER_DATA
static void text_style_color_observer_cb(lv_subject_t *subject, lv_observer_t *observer) {
    lv_style_selector_t selector = (lv_style_selector_t)subject->user_data;
    lv_color_t c = lv_subject_get_color(subject);
    lv_obj_t *target = (lv_obj_t *)lv_observer_get_target(observer);
    lv_obj_set_style_text_color(target, c, selector);
}

static void text_style_color_changed_cb(lv_event_t *e) {
    lv_obj_t *obj = lv_event_get_current_target(e);
    lv_subject_t *subject = lv_event_get_user_data(e);
    lv_style_selector_t selector = (lv_style_selector_t)subject->user_data;
    lv_color_t c = lv_obj_get_style_text_color(obj, selector);
    lv_subject_set_color(subject, c);
}

lv_observer_t *lv_obj_bind_text_style_color(lv_obj_t *obj, lv_subject_t *subject,
        lv_style_selector_t selector) {
    if (subject->type != LV_SUBJECT_TYPE_COLOR) {
        LV_LOG_WARN("Incompatible subject type: %d", subject->type);
        return NULL;
    }
    lv_obj_add_event_cb(obj, text_style_color_changed_cb, LV_EVENT_VALUE_CHANGED, subject);
    lv_observer_t *observer = lv_subject_add_observer_obj(subject, text_style_color_observer_cb, obj, NULL);
    return observer;
}
#endif

void lvgl_assert_fail(void) {
    LV_LOG_USER("-------> ASSERT FAIL");
}

lv_color_t lv_color_from_sharpRGB(const char *c) {
    if (c == NULL) goto FAIL;
    if (*c != '#') goto FAIL;

    uint32_t ul = strtoul (c+1, NULL, 16);
    return lv_color_hex(ul);
FAIL:
    return lv_palette_main(LV_PALETTE_RED);
}
