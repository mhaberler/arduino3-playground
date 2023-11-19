#ifdef GFXDEMO

#ifdef M5UNIFIED
#include <M5Unified.h>

#if defined(LVGL_DEMO)
#include <lvgl.h>
#endif

M5GFX display;
#else
#include <Arduino.h>
#endif

#ifdef LOVYANGFX

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
#define ARDUINO_M5STACK_CORE2
#define LGFX_AUTODETECT
#include <LGFX_AUTODETECT.hpp>
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#endif

#ifdef ATOMS3
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define LGFX_M5ATOMS3
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>
#endif
LGFX display;
#endif

static constexpr size_t BAR_COUNT = 64;
static int max_y[BAR_COUNT];
static int prev_y[BAR_COUNT];
static uint32_t colors[BAR_COUNT];
static size_t bar_count = BAR_COUNT;

void gfxdemo_setup(void)
{

    display.init();
    display.setBrightness(255);

    display.startWrite();
    display.fillScreen(TFT_BLACK);
    Serial.printf("width %d height %d\n", display.width(), display.height());

    if (display.width() == 128) // AtomS3
    {
        bar_count = 8;
    }
    if (display.isEPD())
    {
        display.setEpdMode(epd_mode_t::epd_fastest);
    }
    if (display.width() < display.height())
    {
        display.setRotation(display.getRotation() ^ 1);
    }

    for (int x = 0; x < bar_count; ++x)
    {
        prev_y[x] = display.height();
        max_y[x] = display.height();
        int r = 0, g = 0, b = 0;
        switch (x >> 4)
        {
        case 0:
            b = 255;
            g = x * 0x11;
            break;
        case 1:
            b = 255 - (x & 15) * 0x11;
            g = 255;
            break;
        case 2:
            g = 255;
            r = (x & 15) * 0x11;
            break;
        case 3:
            r = 255;
            g = 255 - (x & 15) * 0x11;
            break;
        }
        colors[x] = display.color888(r, g, b);
    }
}
void gfxdemo_loop(void)
{
    int h = display.height();

    static int i;
    ++i;
    display.waitDisplay();
    for (int x = 0; x < bar_count; ++x)
    {
        int y = (h >> 1) - (sinf((float)((x - 24) * i) / 3210.0f) +
                            sinf((float)((x - 40) * i) / 1234.0f)) *
                               (h >> 2);

        int xpos = x * display.width() / bar_count;
        int w = ((x + 1) * display.width() / bar_count) - xpos - 1;
        if (max_y[x] + 1 >= y)
        {
            max_y[x] = y - 1;
        }
        else
        {
            if ((i & 3) == 0)
            {
                display.fillRect(xpos, max_y[x] - 3, w, 1, TFT_BLACK);
                max_y[x]++;
            }
        }
        display.fillRect(xpos, max_y[x] - 3, w, 3, TFT_WHITE);
        if (prev_y[x] < y)
        {
            display.fillRect(xpos, prev_y[x], w, y - prev_y[x], TFT_BLACK);
        }
        else
        {
            display.fillRect(xpos, y, w, prev_y[x] - y, colors[x]);
        }
        prev_y[x] = y;
    }
    display.display();

    uint16_t touchX, touchY;
#ifdef M5UNIFIED
    bool touched = M5.Display.getTouch(&touchX, &touchY);
#else
    bool touched = display.getTouchRaw(&touchX, &touchY);
#endif
    if (touched)
    {
        Serial.printf("touch %d %d\n", touchX, touchY);
    }
}
#else
void gfxdemo_setup(void) {}
void gfxdemo_loop(void) {}
#endif
