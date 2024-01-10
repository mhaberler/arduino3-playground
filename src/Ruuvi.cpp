#include "Sensor.hpp"

int16_t getInt16(const uint8_t *data, int index) {
    return (int16_t)((data[index] << 8) + (data[index + 1]));
}

uint16_t getUint16(const uint8_t *data, int index) {
    return (uint16_t)((data[index] << 8) + (data[index + 1]));
}

uint8_t getUint8(const uint8_t *data, int index) {
    return (uint8_t)((data[index]));
}

void DecodeV5(const uint8_t *data, ruuviAd_t &ra) {
    ra.ruuvi_format = 5;
    ra.temperature = (float)getInt16(data, 3) * 0.005;
    ra.humidity = (float)getUint16(data, 5) * 0.0025;
    ra.pressure = (float)getUint16(data, 7) / 100 + 500;
    ra.accelX = getInt16(data, 9);
    ra.accelY = getInt16(data, 11);
    ra.accelZ = getInt16(data, 13);
    ra.voltage = (data[15] << 3 | data[16] >> 5) + 1600;
    ra.power = (data[16] & 0x1F) * 2 - 40;
    ra.moveCount = getUint8(data, 17);
    ra.sequence = getUint16(data, 18);
}

void DecodeV3(const uint8_t *data, ruuviAd_t &ra) {
    ra.ruuvi_format = 3;
    float t = (float)(getUint8(data, 4) & 0b01111111) +
              (float)getUint8(data, 5) / 100;
    ra.temperature = (getUint8(data, 4) & 0b10000000) == 128 ? t * -1 : t;
    ra.humidity = (float)getUint8(data, 3) * 0.5;
    ra.pressure = (float)getUint16(data, 6) / 100 + 500;
    ra.accelX = getInt16(data, 8);
    ra.accelY = getInt16(data, 10);
    ra.accelZ = getInt16(data, 12);
    ra.voltage = getUint16(data, 14);
}

void Ruuvi::print(Print &p, format_t format) {

}

bool Ruuvi::configure(JsonObject conf) {
    return Sensor::configure(conf);
};

bool  Ruuvi::bleAdvertisement(const bleAdvMsg_t  &msg) {

    const uint8_t *data = msg.message;
    const uint8_t len = msg.msg_size;
    if (msg.mfid != 0x0499) {
        log_e("ruuvi: bad mfid %x", msg.mfid );
        return false;
    }
    _ruuvi_report.rssi = msg.rssi;
    if (data[2] == 0x3 && len > 15) {
        DecodeV3(data, _ruuvi_report);
        goto RUUVI_DECODED;
    }

    if (data[2] == 0x5 && len > 19) {
        DecodeV5(data, _ruuvi_report);
        goto RUUVI_DECODED;
    }
    log_e("failed to decode ruuvi msg");
    return false;

RUUVI_DECODED:
    _ruuvi_report.rssi = msg.rssi;
    // log_e("ruuvi success");
    return true;
}
