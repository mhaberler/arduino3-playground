
#ifdef LVGL_M5UNIFIED
#include <M5Unified.h>

#include <lvgl.h>

M5GFX display;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;
const char *boardName(void);

static void m5gfx_lvgl_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                             lv_color_t *color_p)
{
  int w = (area->x2 - area->x1 + 1);
  int h = (area->y2 - area->y1 + 1);

  M5.Display.startWrite();
  M5.Display.setAddrWindow(area->x1, area->y1, w, h);
  M5.Display.pushPixels((uint16_t *)&color_p->full, w * h, true);
  M5.Display.endWrite();
  lv_disp_flush_ready(disp);
}

static void m5gfx_lvgl_read(lv_indev_drv_t *indev_driver,
                            lv_indev_data_t *data)
{
  uint16_t touchX, touchY;
  lgfx::touch_point_t tp[1];

  M5.update();

  M5.Display.getTouchRaw(&touchX, &touchY);

  int nums = M5.Display.getTouchRaw(tp, 1);
  if (nums)
  {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = tp[0].x;
    data->point.y = tp[0].y;
#ifdef TRACE_TOUCH
    Serial.printf("x: %d y: %d\n", data->point.x, data->point.y);
#endif
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

/* Setup lvgl with display and touch pad */
void lv_begin()
{
  lv_init(); // call this before any other lvgl function

  display.init();
  size_t buf_size = display.width() * display.height() / 10;
  buf = (lv_color_t *)malloc(buf_size);
  lv_disp_draw_buf_init(&draw_buf, buf, NULL,
                        buf_size); // initialize the display buffer

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res = display.width();
  disp_drv.ver_res = display.height();
  disp_drv.flush_cb = m5gfx_lvgl_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = m5gfx_lvgl_read;
  lv_indev_drv_register(&indev_drv);

  Serial.printf("LVGL v%d.%d.%d initialized, board=%s\n", lv_version_major(),
                lv_version_minor(), lv_version_patch(), boardName());
}

/* Handles updating the display and touch events */
void lv_handler()
{
  static uint32_t previousUpdate = 0;
  static uint32_t interval = 0;

  if (millis() - previousUpdate > interval)
  {
    previousUpdate = millis();
    interval = lv_timer_handler(); // Update the UI
  }
}
#else
void lv_begin(void) {}
void lv_handler(void) {}
#endif