

#include <lvgl.h>
// #include "lv_conf.h"
// #include "lv_setup.hpp"
// #include "esp_err.h"
#include "subjects.h"
#include "defs.h"

lv_subject_t oat_tmp, oat_hum, env_tmp, env_hum;

void subjects_init(void)
{
    // use INT32_MAX as "not available"
    lv_subject_init_int(&oat_tmp, INT32_MAX);
    lv_subject_init_int(&oat_hum, INT32_MAX);
    lv_subject_init_int(&env_tmp, INT32_MAX);
    lv_subject_init_int(&env_hum, INT32_MAX);
}

typedef struct
{
    const char *available;
    const char *unavailable;
} fmt_spec_t;

#define INVALID_INT_VALUE(x) ((x) == INT32_MAX)
#define SET_INVALID_INT(x) \
    {                      \
        x = INT32_MAX;     \
    }

static void timed_value_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    int32_t v = lv_subject_get_int(subject);
    fmt_spec_t *fmt_spec = observer->user_data;
    const char *fmt = INVALID_INT_VALUE(v) ? fmt_spec->unavailable : fmt_spec->available;
    LV_LOG_USER(fmt, ITOD100(v));
}

static fmt_spec_t oat_temp_fmt = {"OAT temp: %.1f°", "OAT temp: n/a"};
static fmt_spec_t env_temp_fmt = {"env temp: %.1f°", "env temp: n/a"};
static fmt_spec_t oat_hum_fmt = {"OAT hum: %.1f%%", "OAT hum: n/a"};
static fmt_spec_t env_hum_fmt = {"env hum: %.1f%%", "env hum: n/a"};

void register_observers(void)
{
    lv_subject_add_observer(&oat_tmp, timed_value_cb, &oat_temp_fmt);
    lv_subject_add_observer(&oat_hum, timed_value_cb, &oat_hum_fmt);
    lv_subject_add_observer(&env_tmp, timed_value_cb, &env_temp_fmt);
    lv_subject_add_observer(&env_hum, timed_value_cb, &env_hum_fmt);
}

void lv_example_label_1(void)
{
    lv_obj_t *scr = lv_screen_active();
    lv_obj_t *label1 = lv_label_create(scr);
    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP); /*Break the long lines*/
    lv_label_set_text(label1, "Recolor is not supported for v9 now.");
    lv_obj_set_width(label1, 150); /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t *label2 = lv_label_create(lv_screen_active());
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
    lv_obj_set_width(label2, 150);
    lv_label_set_text(label2, "It is a circularly scrolling text. ");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 40);
}

void ui_init(void)
{
    subjects_init();

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    lv_example_label_1();
    register_observers();
}
