#include "Sensor.hpp"

constexpr float k0 = 273.15;

void TPMS::print(Print &p, format_t format) {
    JsonDocument doc;
    doc = _tpms_report;
    serializeJson(doc, p);
    p.write("\n");
}

bool TPMS::configure(JsonObject conf) {
    if (!Sensor::configure(conf))
        return false;
    // Serial.printf("CONFIGURE type=%u mac=%s\n", (unsigned)_type, _macAddress.toString().c_str());
    return (type() != ST_NONE);
};

const std::string& TPMS::id(void) {
    return std::string(NimBLEAddress(_macAddress));
}

bool  TPMS::bleAdvertisement(const bleAdvMsg_t  &msg) {

    const uint8_t *data = msg.message;
    const uint8_t len = msg.msg_size;

    _tpms_report.rssi = msg.rssi;
    if (msg.mfid == 0x0100) {
        if (len == 16) {
            _tpms_report.location = getUint8(data, 0) & 0x7f;
            _tpms_report.pressure = getInt32(data, 6);
            _tpms_report.temperature = k0 + getInt32(data, 10) / 100.0;
            _tpms_report.batpct = getUint8(data, 14);
            _tpms_report.status = getUint8(data, 15);
            log_e("tpms success");
            return true;
        }
    }
    if (msg.mfid == 172) { // hijacked TomTom
        if (len == 15) {
            _tpms_report.pressure = getInt32(data, 0);
            _tpms_report.temperature = k0 + getInt32(data, 4) / 100.0;
            _tpms_report.batpct = getUint8(data, 5);
            _tpms_report.location = getUint8(data, 6) & 0x7f;
            _tpms_report.status = 0;
            log_e("tpms success");
            return true;
        }
    }

    log_e("tpms FAIL");
    return false;
}
