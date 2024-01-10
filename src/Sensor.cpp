#include "Sensor.hpp"

sensorMode_t Sensor::mode() {
    return _mode;
}

sensorType_t Sensor::type() {
    return _type;
}

format_t Sensor::format() {
    return _format;
}

NimBLEAddress & Sensor::mac() {
    return _macAddress;
}

bool Sensor::configure(JsonObject conf)  {
    _type = conf["type"];
    if (conf["mac"]) {
        _macAddress = conf["mac"];
    }
    if (conf["MAC"]) {
        _macAddress = conf["MAC"];
    }
    // Serial.printf("CONFIGURE type=%u mac=%s\n", (unsigned)_type, _macAddress.toString().c_str());
    return (_type != ST_NONE);
}

bool Sensor::bleAdvertisement(const bleAdvMsg_t  &msg) {
    return false;
}
