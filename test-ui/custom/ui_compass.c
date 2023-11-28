
#include "ui_compass.h"

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

lv_subject_t compass_all, heading_mag, heading_true, course_over_ground_true, mag_var, sun_pos;

static lv_obj_t *compass_display;
static lv_obj_t *compass_l;
static lv_obj_t *compass_hdt_l;
static lv_obj_t *compass_cogt_l;
static lv_obj_t *compass_mag_var_l;

static lv_obj_t *labelScont;
static lv_obj_t *labelNcont;
static lv_obj_t *labelEcont;
static lv_obj_t *labelWcont;

static lv_meter_scale_t *scale_compass;
static lv_meter_scale_t *scale_compass_maj;

static lv_subject_t *compass_list[] = {&heading_mag, &heading_true, &course_over_ground_true, &mag_var, &sun_pos};

static void compass_group_cb(lv_subject_t *subject, lv_observer_t *observer);

/**
 * A compass display
 */
void lv_compass_display(lv_obj_t *parent)
{
    compass_display = lv_meter_create(parent);

    lv_obj_remove_style(compass_display, NULL, LV_PART_MAIN);
    lv_obj_remove_style(compass_display, NULL, LV_PART_INDICATOR);

    lv_obj_set_size(compass_display, 190, 190);
    lv_obj_center(compass_display);

    scale_compass = lv_meter_add_scale(compass_display);
    lv_meter_set_scale_ticks(compass_display, scale_compass, 73, 1, 12, lv_palette_main(LV_PALETTE_GREY));

    scale_compass_maj = lv_meter_add_scale(compass_display);
    lv_meter_set_scale_ticks(compass_display, scale_compass_maj, 12, 2, 15, lv_palette_main(LV_PALETTE_GREY)); /*12 ticks*/

    labelNcont = lv_obj_create(parent);
    lv_obj_set_size(labelNcont, 42, 42);
    lv_obj_set_style_pad_all(labelNcont, 2, LV_PART_MAIN);
    lv_obj_set_style_bg_color(labelNcont, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
    lv_obj_align(labelNcont, LV_ALIGN_CENTER, 0, -48);

    lv_obj_t *labelN = lv_label_create(labelNcont);
    lv_label_set_text_static(labelN, "N");
    lv_obj_align(labelN, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_transform_pivot_x(labelNcont, 42 / 2, 0);
    lv_obj_set_style_transform_pivot_y(labelNcont, 42 / 2 + 48, 0);

    labelScont = lv_obj_create(parent);
    lv_obj_set_size(labelScont, 42, 42);
    lv_obj_set_style_pad_all(labelScont, 2, LV_PART_MAIN);
    lv_obj_set_style_bg_color(labelScont, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
    lv_obj_align(labelScont, LV_ALIGN_CENTER, 0, -48);

    lv_obj_t *labelS = lv_label_create(labelScont);
    lv_label_set_text_static(labelS, "S");
    lv_obj_align(labelS, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_transform_pivot_x(labelScont, 42 / 2, 0);
    lv_obj_set_style_transform_pivot_y(labelScont, 42 / 2 + 48, 0);

    labelEcont = lv_obj_create(parent);
    lv_obj_set_size(labelEcont, 42, 42);
    lv_obj_set_style_pad_all(labelEcont, 2, LV_PART_MAIN);
    lv_obj_align(labelEcont, LV_ALIGN_CENTER, 0, -48);

    lv_obj_t *labelE = lv_label_create(labelEcont);
    lv_label_set_text_static(labelE, "E");
    lv_obj_align(labelE, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_transform_pivot_x(labelEcont, 42 / 2, 0);
    lv_obj_set_style_transform_pivot_y(labelEcont, 42 / 2 + 48, 0);

    labelWcont = lv_obj_create(parent);
    lv_obj_set_size(labelWcont, 42, 42);
    lv_obj_set_style_pad_all(labelWcont, 2, LV_PART_MAIN);
    lv_obj_align(labelWcont, LV_ALIGN_CENTER, 0, -48);

    lv_obj_t *labelW = lv_label_create(labelWcont);
    lv_label_set_text_static(labelW, "W");
    lv_obj_align(labelW, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_transform_pivot_x(labelWcont, 42 / 2, 0);
    lv_obj_set_style_transform_pivot_y(labelWcont, 42 / 2 + 48, 0);

    lv_obj_t *compass_mark_l = lv_label_create(parent);
    lv_label_set_text_static(compass_mark_l, LV_SYMBOL_DOWN);
    lv_obj_align(compass_mark_l, LV_ALIGN_CENTER, 0, -100);

    compass_l = lv_label_create(parent);
    lv_label_set_text_static(compass_l, "--" LV_SYMBOL_DEGREES);
    lv_obj_align(compass_l, LV_ALIGN_CENTER, 0, 0);
#if LV_FONT_MONTSERRAT_14
    lv_obj_set_style_text_font(compass_l, &lv_font_montserrat_14, 0);
#endif

    lv_meter_set_scale_range(compass_display, scale_compass, 0, 72, 360, 270);
    lv_meter_set_scale_range(compass_display, scale_compass_maj, 1, 12, 330, 300);
    lv_obj_set_style_transform_angle(labelScont, 180 * 10, 0);
    lv_obj_set_style_transform_angle(labelEcont, 90 * 10, 0);
    lv_obj_set_style_transform_angle(labelWcont, 270 * 10, 0);

#if COMPASS_HEADING_TRUE
    compass_hdt_l = lv_label_create(parent);
    lv_label_set_text_static(compass_hdt_l, "HDT:  --" LV_SYMBOL_DEGREES);
    lv_obj_align(compass_hdt_l, LV_ALIGN_TOP_LEFT, 2, 2);
#if LV_FONT_MONTSERRAT_14
    lv_obj_set_style_text_font(compass_hdt_l, &lv_font_montserrat_14, 0);
#endif
#endif

#if COMPASS_COG_TRUE
    compass_cogt_l = lv_label_create(parent);
    lv_label_set_text_static(compass_cogt_l, "COGT:   --" LV_SYMBOL_DEGREES);
    lv_obj_align(compass_cogt_l, LV_ALIGN_TOP_RIGHT, -2, 2);
#if LV_FONT_MONTSERRAT_14
    lv_obj_set_style_text_font(compass_cogt_l, &lv_font_montserrat_14, 0);
#endif
#endif

#if COMPASS_MAG_VAR
    compass_mag_var_l = lv_label_create(parent);
    lv_label_set_text_static(compass_mag_var_l, "Var:\n--" LV_SYMBOL_DEGREES);
    lv_obj_align(compass_mag_var_l, LV_ALIGN_BOTTOM_LEFT, 2, -2);
#if LV_FONT_MONTSERRAT_14
    lv_obj_set_style_text_font(compass_mag_var_l, &lv_font_montserrat_14, 0);
#endif
#endif
    lv_subject_init_int(&heading_mag, 0);
    lv_subject_init_int(&heading_true, 0);
    lv_subject_init_int(&course_over_ground_true, 0);
    lv_subject_init_int(&mag_var, 0);
    lv_subject_init_int(&sun_pos, 0);

    lv_subject_init_group(&compass_all, compass_list, ARRAY_LENGTH(compass_list));
    lv_subject_add_observer_with_target(&compass_all, compass_group_cb, compass_display, NULL);
}


static int16_t last_heading = 0;
static unsigned long last_compass_upd = 0;

static void compass_group_cb(lv_subject_t *subject, lv_observer_t *observer)
{
    LV_LOG_USER("");

    lv_obj_t *target = (lv_obj_t *)lv_observer_get_target(observer);

    lv_subject_t *subject_heading_mag = lv_subject_get_group_element(subject, 0);
    lv_subject_t *subject_heading_true = lv_subject_get_group_element(subject, 1);
    lv_subject_t *subject_course_over_ground_true = lv_subject_get_group_element(subject, 2);
    lv_subject_t *subject_mag_var = lv_subject_get_group_element(subject, 3);
    lv_subject_t *subject_sun_pos = lv_subject_get_group_element(subject, 4);

    if (millis() - last_compass_upd > 500)
    { // reduce expensive rotations
        int32_t h_deg = lv_subject_get_int(subject_heading_mag);

        if (last_heading != h_deg)
        {
            int rot = 360 - h_deg;
            lv_meter_set_scale_range(compass_display, scale_compass, 0, 72, 360, 270 + rot);
            lv_meter_set_scale_range(compass_display, scale_compass_maj, 1, 12, 330, 300 + rot);
            lv_obj_set_style_transform_angle(labelNcont, rot * 10, 0);
            lv_obj_set_style_transform_angle(labelScont, (180 + rot) * 10, 0);
            lv_obj_set_style_transform_angle(labelEcont, (90 + rot) * 10, 0);
            lv_obj_set_style_transform_angle(labelWcont, (270 + rot) * 10, 0);

#if COMPASS_HEADING_MAG
            lv_label_set_text_fmt(compass_l, subject_heading_mag->user_data ? "%ld" LV_SYMBOL_DEGREES : "--", lv_subject_get_int(subject_heading_mag));
#endif
#if COMPASS_MAG_VAR
            lv_label_set_text_fmt(compass_mag_var_l, subject_mag_var->user_data ? "Var:\n%ld" LV_SYMBOL_DEGREES : "Var:\n--" LV_SYMBOL_DEGREES, lv_subject_get_int(subject_mag_var));
#endif
#if COMPASS_HEADING_TRUE
            lv_label_set_text_fmt(compass_hdt_l, subject_heading_true->user_data ? "HDT:  %ld" LV_SYMBOL_DEGREES : "--", lv_subject_get_int(subject_heading_true));
#endif
#if COMPASS_COG_TRUE
            lv_label_set_text_fmt(compass_cogt_l, subject_course_over_ground_true->user_data ? "COGT:  %ld" LV_SYMBOL_DEGREES : "--", lv_subject_get_int(subject_course_over_ground_true));
#endif
        }
        last_compass_upd = millis();
    }
}