#include <Arduino.h>

#include <lvgl.h>
#include "lv_conf.h"
#include "lv_setup.hpp"
#include "esp_err.h"

#define LV_TICK_PERIOD_MS 1
#define TAG __FILE__


/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
static SemaphoreHandle_t xGuiSemaphore = NULL;
static TaskHandle_t g_lvgl_task_handle;

static void gui_task(void *args);
static void lv_tick_task(void *arg);
const char *boardName(void);
static void dummy_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p);

uint8_t *last_flushed_buf;
lv_indev_t *lv_test_mouse_indev;

#define HOR_RES 128 // SCREEN_WIDTH
#define VER_RES 128 // SCREEN_HEIGHT

static int32_t x_act;
static int32_t y_act;
static uint32_t key_act;
static int32_t diff_act;
static bool mouse_pressed;

void lv_test_mouse_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
  LV_UNUSED(indev);
  data->point.x = x_act;
  data->point.y = y_act;
  data->state = mouse_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

static void hal_init(void)
{

  static lv_color32_t *test_fb = (lv_color32_t *)ps_calloc((HOR_RES + LV_DRAW_BUF_STRIDE_ALIGN - 1) * VER_RES + LV_DRAW_BUF_ALIGN,  1);
  assert(test_fb);

  lv_display_t *disp = lv_display_create(HOR_RES, VER_RES);
  lv_display_set_draw_buffers(disp, lv_draw_buf_align(test_fb, LV_COLOR_FORMAT_ARGB8888), NULL, HOR_RES * VER_RES,
                              LV_DISPLAY_RENDER_MODE_DIRECT);
  lv_display_set_flush_cb(disp, dummy_flush_cb);

  lv_test_mouse_indev = lv_indev_create();
  lv_indev_set_type(lv_test_mouse_indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(lv_test_mouse_indev, lv_test_mouse_read_cb);
}

static void dummy_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p)
{
  LV_UNUSED(area);
  LV_UNUSED(color_p);
  last_flushed_buf = color_p;
  lv_display_flush_ready(disp);
}

void my_log_cb(lv_log_level_t level, const char *msg)
{
  Serial.print(msg);
}

/* Setup lvgl with display and touch pad */
void lv_begin()
{
  lv_init(); // call this before any other lvgl function
  lv_log_register_print_cb(my_log_cb);

  hal_init();

  /* Create and start a periodic timer interrupt to call lv_tick_inc */
  const esp_timer_create_args_t lv_periodic_timer_args = {
      .callback = &lv_tick_task,
      .arg = NULL,
      .dispatch_method = ESP_TIMER_TASK,
      .name = "periodic_gui",
      .skip_unhandled_events = true};
  esp_timer_handle_t lv_periodic_timer;
  ESP_ERROR_CHECK(esp_timer_create(&lv_periodic_timer_args, &lv_periodic_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(lv_periodic_timer, LV_TICK_PERIOD_MS * 1000));

  xGuiSemaphore = xSemaphoreCreateMutex();
  if (!xGuiSemaphore)
  {
    ESP_LOGE(TAG, "Create mutex for LVGL failed");
    if (lv_periodic_timer)
      esp_timer_delete(lv_periodic_timer);
    //   return ESP_FAIL;
  }
#if CONFIG_FREERTOS_UNICORE == 0
  int err = xTaskCreatePinnedToCore(gui_task, "lv gui", 1024 * 8, NULL, 3, &g_lvgl_task_handle, 1);
#else
  int err = xTaskCreatePinnedToCore(gui_task, "lv gui", 1024 * 8, NULL, 3, &g_lvgl_task_handle, 0);
#endif
  if (!err)
  {
    ESP_LOGE(TAG, "Create task for LVGL failed");
    if (lv_periodic_timer)
      esp_timer_delete(lv_periodic_timer);
    // return ESP_FAIL;
  }

  // return ESP_OK;
  Serial.printf("LVGL v%d.%d.%d initialized, board=%s\n", lv_version_major(),
                lv_version_minor(), lv_version_patch(), boardName());
}

/* Setting up tick task for lvgl */
static void lv_tick_task(void *arg)
{
  (void)arg;
  lv_tick_inc(LV_TICK_PERIOD_MS);
}

static void gui_task(void *args)
{
  ESP_LOGI(TAG, "Start to run LVGL");
  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(10));

    /* Try to take the semaphore, call lvgl related function on success */
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {
      lv_task_handler();
      // lv_timer_handler_run_in_period(5); /* run lv_timer_handler() every 5ms */
      xSemaphoreGive(xGuiSemaphore);
    }
  }
}

void lvgl_acquire(void)
{
  TaskHandle_t task = xTaskGetCurrentTaskHandle();
  if (g_lvgl_task_handle != task)
  {
    xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);
  }
}

void lvgl_release(void)
{
  TaskHandle_t task = xTaskGetCurrentTaskHandle();
  if (g_lvgl_task_handle != task)
  {
    xSemaphoreGive(xGuiSemaphore);
  }
}

#if !defined(LOVYANGFX) && !defined(M5UNIFIED)
void lv_begin(void) {}
#endif