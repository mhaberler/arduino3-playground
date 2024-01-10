#include "Sensor.hpp"



int getInt16(const uint8_t *data, int index) {
    return (short)((data[index] << 8) + (data[index + 1]));
}

int getInt16one(const uint8_t *data, int index) {
    return (int16_t)((data[index]));
}

unsigned int getUShort(const uint8_t *data, int index) {
    return (uint16_t)((data[index] << 8) + (data[index + 1]));
}

unsigned int getUShortone(const uint8_t *data, int index) {
    return (unsigned int16_t)((data[index]));
}

void DecodeV5(const uint8_t *data, ruuviAd_t *ra) {
    ra->ruuvi_format = 5;
    ra->temperature = (float)getInt16(data, 3) * 0.005;
    ra->humidity = (float)getUShort(data, 5) * 0.0025;
    ra->pressure = (float)getUShort(data, 7) / 100 + 500;
    ra->accelX = getInt16(data, 9);
    ra->accelY = getInt16(data, 11);
    ra->accelZ = getInt16(data, 13);
    ra->voltage = (data[15] << 3 | data[16] >> 5) + 1600;
    ra->power = (data[16] & 0x1F) * 2 - 40;
    ra->moveCount = getUShortone(data, 17);
    ra->sequence = getUShort(data, 18);
}

void DecodeV3(const uint8_t *data, ruuviAd_t *ra) {
    ra->ruuvi_format = 3;
    float t = (float)(getUShortone(data, 4) & 0b01111111) +
              (float)getUShortone(data, 5) / 100;
    ra->temperature = (getUShortone(data, 4) & 0b10000000) == 128 ? t * -1 : t;
    ra->humidity = (float)getUShortone(data, 3) * 0.5;
    ra->pressure = (float)getUShort(data, 6) / 100 + 500;
    ra->accelX = getInt16(data, 8);
    ra->accelY = getInt16(data, 10);
    ra->accelZ = getInt16(data, 12);
    ra->voltage = getUShort(data, 14);
}


void Ruuvi::print(Print &p, format_t format) {

}

bool Ruuvi::configure(JsonObject conf) {
    return Sensor::configure(conf);
};

bool  Ruuvi::bleAdvertisement(const bleAdvMsg_t  &msg) {
    return false;
};
