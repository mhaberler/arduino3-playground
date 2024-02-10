#include "Sensor.hpp"


// sensorMode_t Sensor::mode() {
//     return _mode;
// }

sensorType_t Sensor::type() {
    return _type;
}

Unit *Sensor::unit(void) {
    return _unit;
}

format_t Sensor::format() {
    return _format;
}


void Sensor::dump(Stream &s) {
    s.printf("%s %s\n", fullName().c_str(), sensorTypeStr( type()));
}

const  std::string Sensor::unitName(void) {
    return _unit->id();
}

bool Sensor::configure(JsonObject conf)  {
    _type = conf["st"];
    if (conf["min"]) {
        setMin(conf["min"].as<float>());
    }
    if (conf["max"]) {
        setMax(conf["max"].as<float>());
    }
    return (_type != AT_NONE);
}

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

const char *sensorTypeStr(const sensorType_t st) {
    switch (st) {
        case AT_RUUVI:
            return "Ruuvi";
        case AT_MOPEKA:
            return "Mopeka";
        case AT_TPMS:
            return "TPMS";
        case AT_GPS:
            return "GPS";
        case AT_FLOWSENSOR:
            return "Fuel Flow";
        case AT_BARO:
            return "Barometer";
        case AT_IMU:
            return "IMU";
        case AT_MAGNETOMETER:
            return "Magnetometer";
        default:
            return "?";
    }
}
