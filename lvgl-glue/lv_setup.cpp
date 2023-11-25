
#ifdef M5UNIFIED
#include <M5Unified.h>
#include <lvgl.h>

M5GFX display;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;
#endif

#ifdef LOVYANGFX
#include <lvgl.h>

#ifdef LILYGO_S3CAP
#include "lgfx_lilygo_t_display_s3_cap.h"
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 170
#endif

#ifdef SUNTON7IN
#include "lgfx_user/LGFX_Sunton_ESP32-8048S070.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#endif

#ifdef ELECROW_DLC35020S
#include "elecrow_DLC35020S.h"
#endif

#ifdef CORES3
#define LGFX_AUTODETECT
#include <LGFX_AUTODETECT.hpp>
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#endif

#ifdef CORE2
#define LGFX_USE_V1
#define LGFX_AUTODETECT
#include <M5Core2.h>
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp> // クラス"LGFX"を準備
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#endif

#ifdef ATOMS3
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#endif
LGFX display;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[2][SCREEN_WIDTH * 10];
#endif

const char *boardName(void);

static void lvgl_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                             lv_color_t *color_p)
{

#ifdef M5UNIFIED
  int w = (area->x2 - area->x1 + 1);
  int h = (area->y2 - area->y1 + 1);

  M5.Display.startWrite();
  M5.Display.setAddrWindow(area->x1, area->y1, w, h);
  M5.Display.pushPixels((uint16_t *)&color_p->full, w * h, true);
  M5.Display.endWrite();
#endif
#ifdef LOVYANGFX
  if (display.getStartCount() == 0)
  { // Processing if not yet started
    display.startWrite();
  }
  int w = (area->x2 - area->x1 + 1);
  int h = (area->y2 - area->y1 + 1);
  display.setAddrWindow(area->x1, area->y1, w, h);
#if defined(CORES3) || defined(LILYGO_S3CAP)
  display.pushPixelsDMA((uint16_t *)&color_p->full, w * h, true);
#else
  display.pushPixels((uint16_t *)&color_p->full, w * h, true);
#endif
#endif

  lv_disp_flush_ready(disp);
}

static void lvgl_read(lv_indev_drv_t *indev_driver,
                            lv_indev_data_t *data)
{
  uint16_t touchX, touchY;
  data->state = LV_INDEV_STATE_REL;

#if defined(M5UNIFIED)

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
#endif
#if defined(LOVYANGFX)
#if defined(CORES3) || defined(CORE2) || defined(SUNTON7IN) || defined(ELECROW_DLC35020S)
  if (display.getTouch(&touchX, &touchY))
  {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = touchX;
    data->point.y = touchY;
#ifdef TRACE_TOUCH
    Serial.printf("xy %d %d\n", data->point.x, data->point.y);
#endif
  }
#endif

#ifdef LILYGO_S3CAP
  lgfx::v1::touch_point_t tp;

  if (display.getTouchRaw(&tp, 0))
  {
    data->state = LV_INDEV_STATE_PR;
    data->point.y = tp.x;
    data->point.x = SCREEN_WIDTH - tp.y;
#ifdef TRACE_TOUCH
    Serial.printf("xy %d %d\n", data->point.x, data->point.y);
#endif
  }
#endif
#endif
}

/* Setup lvgl with display and touch pad */
void lv_begin()
{
  lv_init(); // call this before any other lvgl function

  display.init();
#if defined(LILYGO_S3CAP) || defined(ELECROW_DLC35020S)
  display.setRotation(3);
#endif

#ifdef M5UNIFIED
  size_t buf_size = display.width() * display.height() / 10;
  buf = (lv_color_t *)malloc(buf_size);
  lv_disp_draw_buf_init(&draw_buf, buf, NULL,
                        buf_size); // initialize the display buffer
#endif
#ifdef LOVYANGFX
  lv_disp_draw_buf_init(&draw_buf, buf[0], buf[1], SCREEN_WIDTH * 10);
#endif
  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res = display.width();
  disp_drv.ver_res = display.height();
  disp_drv.flush_cb = lvgl_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = lvgl_read;
  lv_indev_drv_register(&indev_drv);

  Serial.printf("LVGL v%d.%d.%d initialized, board=%s\n", lv_version_major(),
                lv_version_minor(), lv_version_patch(), boardName());
}

/* Handles updating the display and touch events */
void lv_handler()
{
  static uint32_t previousUpdate = 0;
  static uint32_t interval = 50;

  if (millis() - previousUpdate > interval)
  {
    previousUpdate = millis();
    interval = lv_timer_handler(); // Update the UI
  }
}

#if !defined(LOVYANGFX) && !defined(M5UNIFIED)
void lv_begin(void) {}
void lv_handler(void) {}
#endif