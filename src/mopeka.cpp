#include "Sensor.hpp"

void Mopeka::print(Print &p, format_t format) {
    JsonDocument doc;
    doc = _mopeka_report;
    serializeJson(doc, p);
    p.write("\n");
}

bool Mopeka::configure(JsonObject conf) {
    if (!BLESensor::configure(conf))
        return false;

    return (type() != AT_NONE);
};

bool Mopeka::_decode(const uint8_t *data,
                     const size_t len, mopekaAd_t &ma) {

    if (len != 12) {
        log_e("Mopeka PRO: manufacturer data len (%u - expect 12)",
              len);
        return false;
    }
    // if (data[2] != 3) {
    //     log_e("Mopeka PRO: invalid HW id %u", data[2]);
    //     return false;
    // }

    ma.battery = (data[3] & 0x7f) / 32.0;
    ma.syncPressed = (data[4] & 0x80) > 0;
    ma.raw_temp = (data[4] & 0x7f);
    ma.temperature = ma.raw_temp - 40; // °C
    ma.qualityStars = (data[6] >> 6);

    ma.acceloX = data[10];
    ma.acceloY = data[11];

    ma.raw_level = ((int(data[6]) << 8) + data[5]) & 0x3fff;
    ma.level = ma.raw_level *
               (MOPEKA_TANK_LEVEL_COEFFICIENTS_PROPANE_0 +
                (MOPEKA_TANK_LEVEL_COEFFICIENTS_PROPANE_1 * ma.raw_temp) +
                (MOPEKA_TANK_LEVEL_COEFFICIENTS_PROPANE_2 * ma.raw_temp *
                 ma.raw_temp));

    // if (cp->params.size() == 3) {
    //     float capacity = cp->params[0];
    //     float full_mm = cp->params[1];
    //     float empty_mm = cp->params[2];
    //     float f;

    //     if (ma.level > full_mm) {
    //         f = 1.0;
    //         ma.liters = capacity;
    //         ma.percent = 100.0;
    //         sp->data_valid = true;
    //     } else if (ma.level > empty_mm) {
    //         f = (ma.level - empty_mm) / (full_mm - empty_mm);
    //         ma.liters = f * capacity;
    //         ma.percent = round(f * 100.0);
    //         sp->data_valid = true;
    //     } else {
    //         ma.percent = NAN;
    //         ma.liters = NAN;
    //         sp->data_valid = false;
    //     }
    // } else {
    //     ma.percent = NAN;
    //     ma.liters = NAN;
    //     sp->data_valid = false;
    // }
    return true;
}

bool  Mopeka::bleAdvertisement(const bleAdvMsg_t  &msg) {

    const uint8_t *data = msg.message;
    const uint8_t len = msg.msg_size;
    if (msg.mfid != 0x0059) {
        log_e("mopeka: bad mfid %x", msg.mfid );
        return false;
    }
    _mopeka_report.rssi = msg.rssi;

    if (!_decode(data, len, _mopeka_report)) {
        log_e("failed to decode mopeka msg");
        return false;
    }
    _mopeka_report.lastchange = millis();
    return true;
}
