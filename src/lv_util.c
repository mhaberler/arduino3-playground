
#include "lv_util.h"

void lv_subject_set_type(lv_subject_t *subject, lv_subject_type_t type)
{
    subject->type = type;
}

lv_subject_type_t lv_subject_get_type(lv_subject_t *subject)
{
    return subject->type;
}

void lv_subject_init_none(lv_subject_t *subject)
{
    lv_memzero(subject, sizeof(lv_subject_t));
    subject->type = LV_SUBJECT_TYPE_NONE;
    subject->value.num = 0;
    subject->prev_value.num = 0;
    _lv_ll_init(&(subject->subs_ll), sizeof(lv_observer_t));
}

void lvgl_assert_fail(void)
{
    LV_LOG_USER("-------> ASSERT FAIL");
}