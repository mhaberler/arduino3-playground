#include "Sensor.hpp"

constexpr float k0 = 273.15;

void TPMS::print(Print &p, format_t format) {
    JsonDocument doc;
    doc = _tpms_report;
    serializeJson(doc, p);
    p.write("\n");
}

bool TPMS::configure(JsonObject conf) {
    return Sensor::configure(conf);
};

bool  TPMS::bleAdvertisement(const bleAdvMsg_t  &msg) {

    const uint8_t *data = msg.message;
    const uint8_t len = msg.msg_size;

    _tpms_report.rssi = msg.rssi;
    if (msg.mfid == 0x0100) {
        if (len == 18) {
            _tpms_report.location = getUint8(data, 2) & 0x7f;
            _tpms_report.pressure = (float)getInt32LE(data, 8) / 100000.0;
            _tpms_report.temperature = (float)getInt32LE(data, 12) / 100.0;
            _tpms_report.batpct = getUint8(data, 16);
            _tpms_report.status = getUint8(data, 17);
            _tpms_report.rssi = msg.rssi;
            _tpms_report.lastchange = millis();

            return true;
        }
    }
    if (msg.mfid == 0x00AC) {
        if (len == 15) {
            _tpms_report.pressure = getInt32LE(data, 0);
            _tpms_report.temperature = k0 + getInt32LE(data, 4) / 100.0;
            _tpms_report.batpct = getUint8(data, 5);
            _tpms_report.location = getUint8(data, 6) & 0x7f;
            _tpms_report.status = 0;
            _tpms_report.rssi = msg.rssi;
            _tpms_report.lastchange = millis();

            return true;
        }
    }

    log_e("tpms FAIL");
    return false;
}
