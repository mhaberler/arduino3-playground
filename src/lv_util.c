#include <lvgl.h>
#include "lv_observer.h"

void lv_subject_set_invalid(lv_subject_t *subject, int32_t value)
{
    subject->type = LV_SUBJECT_TYPE_NONE;
    lv_subject_notify(subject);
}

void lv_subject_force_int(lv_subject_t *subject, int32_t value)
{
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->prev_value.num = subject->value.num;
    subject->value.num = value;
    lv_subject_notify(subject);
}

void lv_subject_force_int_prot(lv_subject_t *subject, int32_t value)
{
    lvgl_acquire();
    lv_subject_force_int(subject, value);
    lvgl_release();
}
