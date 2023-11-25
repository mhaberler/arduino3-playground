#pragma once

#include <stdint.h>

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
    int ruuvi_format;
    uint8_t rssi;
    char name[30];
    char address[20];
    uint32_t last_sequence;
} ruuviAd_t;
