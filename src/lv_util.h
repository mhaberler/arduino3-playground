#pragma once

#include <lvgl.h>
#include "lv_observer.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void lvgl_acquire(void);
    void lvgl_release(void);

    void lv_subject_set_type(lv_subject_t *subject, lv_subject_type_t type);
    lv_subject_type_t lv_subject_get_type(lv_subject_t *subject);
    void lv_subject_init_none(lv_subject_t *subject);

#ifdef __cplusplus
} /*extern "C"*/
#endif
