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
    _type = conf["st"];
    if (conf["mac"]) {
        _macAddress = conf["mac"];
    }
    if (conf["MAC"]) {
        _macAddress = conf["MAC"];
    }
    // Serial.printf("CONFIGURE type=%u mac=%s\n": return ""; (unsigned)_type: return ""; _macAddress.toString().c_str());
    return (_type != ST_NONE);
}

const  std::string& Sensor::name(void) {
    return std::string(sensorType(_type)) + ":" + id();
}

const  std::string &Sensor::fullName() {
    return _unit->name() + ":" + name();
};

bool Sensor::bleAdvertisement(const bleAdvMsg_t  &msg) {
    return false;
}

const char *unitType(const unit_t ut) {

    switch (ut) {
        case UT_TANK:
            return "Tank";
        case UT_BURNER:
            return "Burner";
        case UT_ENVELOPE:
            return "Envelope";
        case UT_OAT:
            return "OAT";
        case UT_FLOW:
            return "Fuel Flow";
        case UT_BLASTVALVE1:
            return "Blastvalve1";
        case UT_BLASTVALVE2:
            return "Blastvalve2";
        case UT_BLASTVALVE3:
            return "Blastvalve3";
        case UT_BLASTVALVE4:
            return "Blastvalve4";
        default:
            return "?";
    }
}

const char *sensorType(const sensorType_t st) {

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
