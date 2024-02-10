#include "Sensor.hpp"
#include "ui_message.hpp"
#include "lv_subjects.hpp"

int16_t getInt16(const uint8_t *data, int index) {
    return (int16_t)((data[index] << 8) + (data[index + 1]));
}

uint16_t getUint16(const uint8_t *data, int index) {
    return (uint16_t)((data[index] << 8) + (data[index + 1]));
}

int32_t getInt32(const uint8_t *data, int index) {
    return (int32_t)(
               (data[index] << 24) |
               (data[index+1] << 16) |
               (data[index+2] << 8) |
               (data[index+3]));
}

int32_t getInt32LE(const uint8_t *data, int index) {
    return (int32_t)(
               (data[index]) |
               (data[index+1] << 8) |
               (data[index+2] << 16) |
               (data[index+3] << 24));
}

uint32_t getUint32(const uint8_t *data, int index) {
    return (uint32_t)(
               (data[index] << 24) |
               (data[index+1] << 16) |
               (data[index+2] << 8) |
               (data[index+3]));
}

uint8_t getUint8(const uint8_t *data, int index) {
    return (uint8_t)((data[index]));
}

int8_t getInt8(const uint8_t *data, int index) {
    return (int8_t)((data[index]));
}

// see https://github.com/ruuvi/ruuvi-sensor-protocols/blob/master/dataformat_05.md
static void DecodeV5(const uint8_t *data, ruuviAd_t &ra) {
    int16_t i16;
    uint16_t u16;
    uint8_t u8;

    ra.availability = 0;
    ra.ruuvi_format = 5;

    i16 = getInt16(data, 3);
    if (i16 !=0x8000) ra.availability |= RUUVI_TEMPERATURE_AVAILABLE;
    ra.temperature = (float) i16 * 0.005;

    u16 = getUint16(data, 5);
    if (u16 != 65535) ra.availability |= RUUVI_HUMIIDTY_AVAILABLE;
    ra.humidity = (float)u16 * 0.0025;

    u16 = getUint16(data, 7);
    if (u16 != 65535) ra.availability |= RUUVI_PRESSURE_AVAILABLE;
    ra.pressure = ((float)u16 + 50000.0) / 100.0;

    ra.accelX = getInt16(data, 9);
    if (ra.accelX != 0x8000) ra.availability |= RUUVI_ACCELX_AVAILABLE;
    ra.accelY = getInt16(data, 11);
    if (ra.accelY != 0x8000) ra.availability |= RUUVI_ACCELY_AVAILABLE;
    ra.accelZ = getInt16(data, 13);
    if (ra.accelZ != 0x8000) ra.availability |= RUUVI_ACCELZ_AVAILABLE;

    u16 = data[15] << 3 | data[16];
    if (u16 != 2047) ra.availability |= RUUVI_BATTERY_AVAILABLE;
    ra.voltage = u16 + 1600;

    u8 = data[16] & 0x1F;
    if (u8 != 31) ra.availability |= RUUVI_TXPOWER_AVAILABLE;
    ra.power = (u8) * 2 - 40;

    u8 = getUint8(data, 17);
    if (u8 != 255) ra.availability |= RUUVI_MOVEMENT_AVAILABLE;
    ra.moveCount = u8;

    u16 = getUint16(data, 18);
    if (u16 != 65535) ra.availability |= RUUVI_SEQUENCE_AVAILABLE;
    ra.sequence = u16;
}

static void DecodeV3(const uint8_t *data, ruuviAd_t &ra) {
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
    JsonDocument doc;
    doc = _ruuvi_report;
    doc["st"] = AT_RUUVI;
    serializeJson(doc, p);
    p.write("\n");
}

bool Ruuvi::configure(JsonObject conf) {
    return BLESensor::configure(conf);
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
    _ruuvi_report.lastchange = millis();
    _ruuvi_report.rssi = msg.rssi;
    return true;
}

void Ruuvi::report(void) {
    JsonDocument doc;
    doc = _ruuvi_report;
    doc["st"] = AT_RUUVI;
    switch (unit()->type()) {
        case UT_BASKET:
            doc["um"] = (int)UM_OAT;
            break;
        case UT_ENVELOPE:
            doc["um"] = (int)UM_ENV;
            break;
        default:
            break;
    }
    sendUiMessage(doc);
}