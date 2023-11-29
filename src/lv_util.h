#pragma once

#include <lvgl.h>
#include "lv_observer.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // Function to compare two lv_color_t values
    static inline int compare_colors(lv_color_t color1, lv_color_t color2)
    {
        return (color1.ch.red == color2.ch.red) &&
               (color1.ch.green == color2.ch.green) &&
               (color1.ch.blue == color2.ch.blue);
    }
    void lvgl_acquire(void);
    void lvgl_release(void);

#if LV_USE_USER_DATA
    static inline void lv_subject_set_user_data(lv_subject_t *subject, void *user_data)
    {
        subject->user_data = user_data;
    }

    static inline void *lv_subject_get_user_data(lv_subject_t *subject)
    {
        return subject->user_data;
    }
#endif
    void lv_subject_set_type(lv_subject_t *subject, lv_subject_type_t type);
    lv_subject_type_t lv_subject_get_type(lv_subject_t *subject);
    void lv_subject_init_none(lv_subject_t *subject);

#if LV_USE_USER_DATA
    /**
     * Bind an color subject to an object's text style color
     * @param obj       pointer to an object
     * @param subject   pointer to a subject
     * @param selector  style selector to be used when applying the color
     * @return          pointer to the created observer
     */
    lv_observer_t *lv_obj_bind_text_style_color(lv_obj_t *obj, lv_subject_t *subject,
                                          lv_style_selector_t selector);
#endif
#ifdef __cplusplus
} /*extern "C"*/
#endif
