#ifdef M5GFX_DEMO
#include <M5Unified.h>

M5GFX display;

static constexpr size_t BAR_COUNT = 64;
static int max_y[BAR_COUNT];
static int prev_y[BAR_COUNT];
static uint32_t colors[BAR_COUNT];
static size_t bar_count = BAR_COUNT;


void m5gfxdemo_setup(void)
{
    display.init();
    display.startWrite();
    display.fillScreen(TFT_BLACK);
    if (display.width() == 128) {
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
void m5gfxdemo_loop(void)
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
}
#else
void m5gfxdemo_setup(void) {}
void m5gfxdemo_loop(void) {}
#endif
