#pragma once

#include <stdint.h>

// ruuvi reporting interval is 60s
#define RUUVI_PERIOD ( 65 * 1000)

typedef struct
{
    float temperature;
    float humidity;
    float pressure;
    int16_t accelX;
    int16_t accelY;
    int16_t accelZ;
    uint16_t voltage;
    uint8_t power;
    uint16_t sequence;
    uint8_t moveCount;
    uint8_t ruuvi_format;
    int16_t rssi;
        char name[30];
        char address[20];
    uint32_t last_sequence;
} ruuviAd_t;
