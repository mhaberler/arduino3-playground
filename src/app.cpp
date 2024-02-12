#ifdef M5UNIFIED
    #include <M5Unified.h>
#else
    #include <Arduino.h>
#endif
#include <Ticker.h>

void ble_setup(void);
void ble_loop(void);
void test_spdlog(void);
void gfxdemo_setup(void);
void gfxdemo_loop(void);
void lvgl_setup(void);
void lvgl_loop(void);
void webserver_setup(void);
void webserver_loop(void);
void nfc_setup(void);
void nfc_loop(void);
void init_sensors(void);
void sensor_loop(void);

bool psRAMavail;

static bool rfid_test;

static Ticker secondTicker;

void setup(void) {
    // disableLoopWDT();
    // disableCore0WDT();
    // disableCore1WDT();

    delay(3000);
    psRAMavail = ESP.getFreePsram() > 0;
#ifdef M5UNIFIED
    auto cfg = M5.config();
    cfg.serial_baudrate = 115200;
    cfg.led_brightness = 128;
    cfg.clear_display = true;
    M5.begin(cfg);
#else
    Serial.begin(115200);
#endif
    lvgl_setup();
    gfxdemo_setup();
    test_spdlog();
    ble_setup();
    webserver_setup();
    nfc_setup();
    init_sensors();
    secondTicker.attach_ms(300, []() {
        rfid_test = true;
    });
    Serial.printf("C++ version: %ld\n", __cplusplus);
}

void loop(void) {
    lvgl_loop();
    gfxdemo_loop();
    webserver_loop();
    if (rfid_test) {
        nfc_loop();
        rfid_test = false;
    }
    ble_loop();
    sensor_loop();
    delay(1);
    yield(); // Stop watchdog reset
}

size_t getArduinoLoopTaskStackSize(void) {
    return CUSTOM_ARDUINO_LOOP_STACK_SIZE;
}