#include "Sensor.hpp"

// sensorMode_t Sensor::mode() {
//     return _mode;
// }

actorType_t Sensor::type() {
    return _type;
}

format_t Sensor::format() {
    return _format;
}

// const NimBLEAddress & Sensor::mac() {
//     return _macAddress;
// }

const  std::string Sensor::unitName(void) {
    return _unit->name();
}

bool Sensor::configure(JsonObject conf)  {
    _type = conf["st"];
    if (conf["mac"]) {
        setAddress(conf["mac"].as< std::string>());
    }
    if (conf["MAC"]) {
        setAddress(conf["MAC"].as< std::string>());
    }
    // Serial.printf("CONFIGURE type=%u mac=%s\n": return ""; (unsigned)_type: return ""; _macAddress.toString().c_str());
    return (_type != ST_NONE);
}

// const  std::string& Sensor::name(void) {
//     return std::string(sensorType(_type)) + ":" + id();
// }

// const  std::string Sensor::fullName() {
//     return "XXX"; // _unit->name(); //  + ":" + name();
// };

bool Sensor::bleAdvertisement(const bleAdvMsg_t  &msg) {
    return false;
}

const char *unitTypeStr(const unit_t ut) {
    switch (ut) {
        case UT_TANK:
            return "Tank";
        case UT_BURNER:
            return "Burner";
        case UT_ENVELOPE:
            return "Envelope";
        case UT_BASKET:
            return "Basket";
        default:
            return "?";
    }
}

const char *sensorTypeStr(const actorType_t st) {
    switch (st) {
        case ST_RUUVI:
            return "Ruuvi";
        case ST_MOPEKA:
            return "Mopeka";
        case ST_TPMS:
            return "TPMS";
        case ST_GPS:
            return "GPS";
        case ST_FLOWSENSOR:
            return "Fuel Flow";
        case ST_BARO:
            return "Barometer";
        case ST_IMU:
            return "IMU";
        case ST_MAGNETOMETER:
            return "Magnetometer";
        default:
            return "?";
    }
}
