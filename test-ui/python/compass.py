import display_driver
import lvgl as lv

parent = lv.obj()

compass_display = lv.meter(parent)
compass_display.remove_style(None, lv.PART.MAIN)
compass_display.remove_style(None, lv.PART.INDICATOR)
compass_display.set_size(190, 190)  
compass_display.center()

scale_compass = compass_display.add_scale()
scale_compass.tick_cnt = 73
scale_compass.tick_length = 12
scale_compass.tick_width = 1

scale_compass_maj = compass_display.add_scale()
scale_compass_maj.tick_cnt = 12
scale_compass_maj.tick_length = 15
scale_compass_maj.tick_width = 2

labelNcont = lv.obj(parent)
labelNcont.set_size(42, 42)  
labelNcont.set_style_pad_all(2,lv.PART.MAIN)
labelNcont.set_style_bg_color(lv.palette_main(lv.PALETTE.RED), lv.PART.MAIN)
labelNcont.align(lv.ALIGN.CENTER, 0, -48)

labelN = lv.label(labelNcont)
labelN.set_text("N")
labelN.align(lv.ALIGN.CENTER, 0, 0)
labelN.set_style_transform_pivot_x(42 // 2, 0)
labelN.set_style_transform_pivot_y(42 // 2 + 48, 0)

labelScont = lv.obj(parent)
labelScont.set_size(42, 42)  
labelScont.set_style_pad_all(2,lv.PART.MAIN)
labelScont.set_style_bg_color(lv.palette_main(lv.PALETTE.BLUE), lv.PART.MAIN)
labelScont.align(lv.ALIGN.CENTER, 0, -48)

labelS = lv.label(labelScont)
labelS.set_text("S")
labelS.align(lv.ALIGN.CENTER, 0, 0)
labelS.set_style_transform_pivot_x(42 // 2, 0)
labelS.set_style_transform_pivot_y(42 // 2 + 48, 0)

labelEcont = lv.obj(parent)
labelEcont.set_size(42, 42)  
labelEcont.set_style_pad_all(2,lv.PART.MAIN)
labelEcont.align(lv.ALIGN.CENTER, 0, -48)

labelE = lv.label(labelEcont)
labelE.set_text("E")
labelE.align(lv.ALIGN.CENTER, 0, 0)
labelE.set_style_transform_pivot_x(42 // 2, 0)
labelE.set_style_transform_pivot_y(42 // 2 + 48, 0)
#--
labelWcont = lv.obj(parent)
labelWcont.set_size(42, 42)  
labelWcont.set_style_pad_all(2,lv.PART.MAIN)
labelWcont.align(lv.ALIGN.CENTER, 0, -48)

labelW = lv.label(labelWcont)
labelW.set_text("W")
labelW.align(lv.ALIGN.CENTER, 0, 0)

labelW.set_style_transform_pivot_x(42 // 2, 0)
labelW.set_style_transform_pivot_y(42 // 2 + 48, 0)

mark = lv.label(parent)
mark.set_text(lv.SYMBOL.DOWN)
mark.align(lv.ALIGN.CENTER, 0, -100)

suncont = lv.obj(parent)
suncont.set_size(42, 42)  
suncont.set_style_pad_all(2,lv.PART.MAIN)
suncont.align(lv.ALIGN.CENTER, 0, -48)

sun = lv.label(suncont)
sun.set_text("o")
sun.set_style_text_color(lv.palette_main(lv.PALETTE.RED),lv.PART.MAIN)
# sun.set_style_bg_opa(lv.OPA.COVER,0)
sun.align(lv.ALIGN.CENTER, 0, 0)

scale_compass.min = 0
scale_compass.max = 72
scale_compass.angle_range = 360
scale_compass.rotation = 270

scale_compass_maj.min = 1
scale_compass_maj.max = 12
scale_compass_maj.angle_range = 330
scale_compass_maj.rotation = 300

labelScont.set_style_transform_angle(180 * 10, 0)
labelEcont.set_style_transform_angle(90 * 10, 0)
labelWcont.set_style_transform_angle(270 * 10, 0)

def rotate(h_deg, sunpos):
    rot = 360 - h_deg;

    scale_compass.angle_range = 360
    scale_compass.rotation = 270 + rot

    scale_compass_maj.angle_range = 330
    scale_compass_maj.rotation = 300 + rot

    labelNcont.set_style_transform_angle(rot * 10, 0)
    labelScont.set_style_transform_angle((180 + rot) * 10, 0)
    labelEcont.set_style_transform_angle((90 + rot) * 10, 0)
    labelWcont.set_style_transform_angle((270 + rot) * 10, 0)
    suncont.set_style_transform_angle(sunpos * 10, 0)

lv.scr_load(parent)
rotate(37, 0)
