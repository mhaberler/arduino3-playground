

#include <lvgl.h>
#include "subjects.h"
#include "defs.h"

static lv_timer_t *na_timer;

lv_subject_t oat_tmp, oat_hum, env_tmp, env_hum;

// ruuvi interval 60s

static transient_subject_t oat_temp_fmt = {&oat_tmp, "OAT temp: %.1f°", "OAT temp: n/a", 65 * 1000, "foobar"};
static transient_subject_t oat_hum_fmt = {&oat_hum, "OAT hum: %.1f%%", "OAT hum: n/a", 65 * 1000};
static transient_subject_t env_temp_fmt = {&env_tmp, "env temp: %.1f°", "env temp: n/a", 65 * 1000};
static transient_subject_t env_hum_fmt = {&env_hum, "env hum: %.1f%%", "env hum: n/a", 65 * 1000};

static void value_available_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    transient_subject_t *tf = (transient_subject_t *)observer->user_data;
    tf->last_heard_ms = millis();
    int32_t current_value = lv_subject_get_int(subject);
    int32_t previous_value = lv_subject_get_previous_int(subject);
    bool current_value_valid = VALID_INT_VALUE(current_value);
    bool previous_value_valid = VALID_INT_VALUE(previous_value);
    if (previous_value_valid ^ current_value_valid)
    {
        // availability changed
        if (current_value_valid)
        {
            LV_LOG_USER("value became available");
        }
        if (previous_value_valid)
        {
            LV_LOG_USER("value became unavailable");
        }
    }
    if (tf)
    {
        LV_LOG_USER(current_value_valid ? tf->available : tf->unavailable, ITOD100(current_value));
    }
    else
    {
        LV_LOG_USER("missing format");
    }
}

static void expire_fmt(uint32_t now, transient_subject_t *fmt)
{
    if (now > (fmt->last_heard_ms + fmt->ttl_ms))
    {
        // LV_LOG_USER("expire subject %p ", fmt->subject);
        lv_subject_set_int(fmt->subject, INT32_MAX);
    }
}

static void expire_values(lv_timer_t *timer)
{
    uint32_t now = millis();
    expire_fmt(now, &oat_temp_fmt);
    expire_fmt(now, &oat_hum_fmt);
    expire_fmt(now, &env_temp_fmt);
    expire_fmt(now, &env_hum_fmt);
}

void init_timer(void)
{
    lv_timer_create(expire_values, 1000, NULL);
}

void register_observers(void)
{
    lv_subject_add_observer(&oat_tmp, value_available_cb, &oat_temp_fmt);
    lv_subject_add_observer(&oat_hum, value_available_cb, &oat_hum_fmt);
    lv_subject_add_observer(&env_tmp, value_available_cb, &env_temp_fmt);
    lv_subject_add_observer(&env_hum, value_available_cb, &env_hum_fmt);
}

void subjects_init(void)
{
    // use INT32_MAX as "not available"
    lv_subject_init_int(&oat_tmp, INT32_MAX);
    oat_tmp.prev_value.num = INT32_MAX; // invalidate the previous value

    lv_subject_init_int(&oat_hum, INT32_MAX);
    oat_hum.prev_value.num = INT32_MAX;

    lv_subject_init_int(&env_tmp, INT32_MAX);
    env_tmp.prev_value.num = INT32_MAX;

    lv_subject_init_int(&env_hum, INT32_MAX);
    env_hum.prev_value.num = INT32_MAX;
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
extern "C"
{
    void ui_init(void)
    {
        subjects_init();

        lv_disp_t *dispp = lv_disp_get_default();
        lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
        lv_disp_set_theme(dispp, theme);
        lv_example_label_1();
        register_observers();
        init_timer();
    }
}
