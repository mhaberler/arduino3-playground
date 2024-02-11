#include "lv_setup.hpp"

#define LV_TICK_PERIOD_MS 1
#define TAG __FILE__

#undef LVGL_DOUBLE_BUFFER

#ifdef M5UNIFIED
    #include <M5Unified.h>
    #include <lvgl.h>

    M5GFX display;
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

    #ifdef LILYGO_S3_PRO
        #define LGFX_AUTODETECT
        #include <LGFX_AUTODETECT.hpp>
        #define SCREEN_WIDTH 480
        #define SCREEN_HEIGHT 222
    #endif

    LGFX display;
#endif

#ifdef LILYGO_T4S3
#include <LilyGo_AMOLED.h>
#include <lvgl.h>
#include "LilyGo_Display.h"

        #define SCREEN_WIDTH 600
        #define SCREEN_HEIGHT 450

static lv_disp_drv_t disp_drv;
static lv_indev_drv_t  indev_drv;

LilyGo_AMOLED display;

static void lv_rounder_cb(lv_disp_drv_t *disp_drv, lv_area_t *area)
{
    // make sure all coordinates are even
    if (area->x1 & 1)
        area->x1--;
    if (!(area->x2 & 1))
        area->x2++;
    if (area->y1 & 1)
        area->y1--;
    if (!(area->y2 & 1))
        area->y2++;
}
#endif

static lv_disp_draw_buf_t draw_buf;

/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
static SemaphoreHandle_t xGuiSemaphore = NULL;
static TaskHandle_t g_lvgl_task_handle;

static void gui_task(void *args);
static void lv_tick_task(void *arg);

const char *boardName(void);

static void lvgl_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                       lv_color_t *color_p) {
    int w = (area->x2 - area->x1 + 1);
    int h = (area->y2 - area->y1 + 1);

#ifdef M5UNIFIED
    M5.Display.startWrite();
    M5.Display.setAddrWindow(area->x1, area->y1, w, h);
#if defined(LVGL_USE_DMA)
    M5.Display.pushPixelsDMA((uint16_t *)&color_p->full, w * h, true);
#else
    M5.Display.pushPixels((uint16_t *)&color_p->full, w * h, true);
#endif
    M5.Display.endWrite();
#endif

#ifdef LOVYANGFX
    if (display.getStartCount() == 0) {
        // Processing if not yet started
        display.startWrite();
    }
    display.setAddrWindow(area->x1, area->y1, w, h);
#if defined(LVGL_USE_DMA)
    display.pushPixelsDMA((uint16_t *)&color_p->full, w * h, true);
#else
    display.pushPixels((uint16_t *)&color_p->full, w * h, true);
#endif
#endif
#ifdef LILYGO_T4S3
    static_cast<LilyGo_Display *>(disp->user_data)->pushColors(area->x1, area->y1, w, h, (uint16_t *)color_p);
#endif
    lv_disp_flush_ready(disp);
}

static void lvgl_read(lv_indev_drv_t *indev_driver,
                      lv_indev_data_t *data) {
    data->state = LV_INDEV_STATE_REL;
#if defined(M5UNIFIED)
    lgfx::touch_point_t tp[1];

    M5.update();

    M5.Display.getTouchRaw(&touchX, &touchY);

    int nums = M5.Display.getTouchRaw(tp, 1);
    if (nums) {
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
    uint16_t touchX, touchY;
    if (display.getTouch(&touchX, &touchY)) {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
#ifdef TRACE_TOUCH
        Serial.printf("xy %d %d\n", data->point.x, data->point.y);
#endif
    }
#endif
#ifdef LILYGO_T4S3
    static int16_t x, y;
    uint8_t touched =   static_cast<LilyGo_Display *>(indev_driver->user_data)->getPoint(&x, &y, 1);
    if ( touched ) {
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PR;
        return;
    }
#endif

#ifdef LILYGO_S3CAP
    lgfx::v1::touch_point_t tp;

    if (display.getTouchRaw(&tp, 0)) {
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

static void my_log_cb(const char *msg) {
    Serial.print(msg);
}

/* Setup lvgl with display and touch pad */
void lv_begin() {
    lv_init(); // call this before any other lvgl function
    lv_group_set_default(lv_group_create());
    lv_log_register_print_cb(my_log_cb);

#ifdef LILYGO_T4S3
    display.beginAMOLED_241();
#else
    display.init();
#endif
#if defined(INITIAL_ROTATION)
    display.setRotation(INITIAL_ROTATION);
#endif
    const size_t buf_size = display.width() * 10;

#if defined(LVGL_DOUBLE_BUFFER)
    lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_DMA);
    lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_DMA);

    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, buf_size);
#else
    lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_DMA);
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, buf_size);
#endif

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = display.width();
    disp_drv.ver_res = display.height();
    disp_drv.flush_cb = lvgl_flush;
    disp_drv.draw_buf = &draw_buf;

#ifdef LILYGO_T4S3
    bool full_refresh = display.needFullRefresh();
    disp_drv.full_refresh = full_refresh;
    disp_drv.user_data = &display;
    if (!full_refresh) {
        disp_drv.rounder_cb = lv_rounder_cb;
    }
#endif
    lv_disp_drv_register(&disp_drv);

    /*Initialize the input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = lvgl_read;
    lv_indev_t *touch_indev = lv_indev_drv_register(&indev_drv);
    lv_indev_set_group(touch_indev, lv_group_get_default());

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t lv_periodic_timer_args = {
        .callback = &lv_tick_task,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "periodic_gui",
        .skip_unhandled_events = true
    };
    esp_timer_handle_t lv_periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&lv_periodic_timer_args, &lv_periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lv_periodic_timer, LV_TICK_PERIOD_MS * 1000));

    xGuiSemaphore = xSemaphoreCreateMutex();
    if (!xGuiSemaphore) {
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
    if (!err) {
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
static void lv_tick_task(void *arg) {
    (void)arg;
    lv_tick_inc(LV_TICK_PERIOD_MS);
}

static void gui_task(void *args) {
    ESP_LOGI(TAG, "Start to run LVGL");
    uint32_t time_till_next = 10;
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(time_till_next));
        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) { // block indefinitely
            time_till_next = lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
    }
}

extern "C"
{
    void lvgl_acquire(void) {
        TaskHandle_t task = xTaskGetCurrentTaskHandle();
        if (g_lvgl_task_handle != task) {
            xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);
        }
    }

    void lvgl_release(void) {
        TaskHandle_t task = xTaskGetCurrentTaskHandle();
        if (g_lvgl_task_handle != task) {
            xSemaphoreGive(xGuiSemaphore);
        }
    }
}
#ifdef LILYGO_T4S3
const char *boardName(void) {
    return "LilyGo T4-S3";
}
#endif

#ifdef M5UNIFIED
const char *boardName(void) {

    switch (M5.getBoard()) {
#if defined(CONFIG_IDF_TARGET_ESP32S3)
        case m5::board_t::board_M5StackCoreS3:
            return "StackCoreS3";
            break;
        case m5::board_t::board_M5StampS3:
            return "StampS3";
            break;
        case m5::board_t::board_M5AtomS3U:
            return "ATOMS3U";
            break;
        case m5::board_t::board_M5AtomS3Lite:
            return "ATOMS3Lite";
            break;
        case m5::board_t::board_M5AtomS3:
            return "ATOMS3";
            break;
        case m5::board_t::board_M5Dial:
            return "Dial";
            break;
        case m5::board_t::board_M5DinMeter:
            return "DinMeter";
            break;
        case m5::board_t::board_M5Capsule:
            return "Capsule";
            break;
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
        case m5::board_t::board_M5StampC3:
            return "StampC3";
            break;
        case m5::board_t::board_M5StampC3U:
            return "StampC3U";
            break;
#else
        case m5::board_t::board_M5Stack:
            return "Stack";
            break;
        case m5::board_t::board_M5StackCore2:
            return "StackCore2";
            break;
        case m5::board_t::board_M5StickC:
            return "StickC";
            break;
        case m5::board_t::board_M5StickCPlus:
            return "StickCPlus";
            break;
        case m5::board_t::board_M5StickCPlus2:
            return "StickCPlus";
            break;
        case m5::board_t::board_M5StackCoreInk:
            return "CoreInk";
            break;
        case m5::board_t::board_M5Paper:
            return "Paper";
            break;
        case m5::board_t::board_M5Tough:
            return "Tough";
            break;
        case m5::board_t::board_M5Station:
            return "Station";
            break;
        case m5::board_t::board_M5Atom:
            return "ATOM";
            break;
        case m5::board_t::board_M5AtomPsram:
            return "ATOM PSRAM";
            break;
        case m5::board_t::board_M5AtomU:
            return "ATOM U";
            break;
        case m5::board_t::board_M5TimerCam:
            return "TimerCamera";
            break;
        case m5::board_t::board_M5StampPico:
            return "StampPico";
            break;
#endif
        default:
            return "Who am I ?";
            break;
    }
}
#endif

#ifdef LOVYANGFX
const char *boardName(void) {
    static char name[20];
    snprintf(name, sizeof(name), "lovyan %lu", (uint32_t)display.getBoard());
    return name;
}
#endif

#if !defined(LOVYANGFX) && !defined(M5UNIFIED)  && !defined(LILYGO_T4S3)
void lv_begin(void) {}
#endif