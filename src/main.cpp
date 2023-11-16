#ifdef M5UNIFIED
#include <M5Unified.h>
#else
#include <Arduino.h>
#endif

void startBLEscan(void);
void test_spdlog(void);
void m5gfxdemo_setup(void);
void m5gfxdemo_loop(void);
void lvgl_setup(void);
void lvgl_loop(void);

void setup(void)
{
#ifdef M5UNIFIED

  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;
  cfg.led_brightness = 128;
  cfg.clear_display = true;
  M5.begin(cfg);
#else
  Serial.begin(115200);
#endif
  test_spdlog();
  startBLEscan();
  lvgl_setup();
  m5gfxdemo_setup();
}

void loop(void)
{
  lvgl_loop();
  m5gfxdemo_loop();
  delay(1);
}
