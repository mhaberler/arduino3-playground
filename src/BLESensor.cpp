#include "Sensor.hpp"

bool BLESensor::configure(JsonObject conf)    {
    _type = conf["st"];
    if (conf["mac"]) {
        _macAddress = NimBLEAddress(conf["mac"].as< std::string>());
    }
    if (conf["MAC"]) {
        _macAddress = NimBLEAddress(conf["MAC"].as< std::string>());
    }
    return (_type != AT_NONE);
}

const  std::string BLESensor::id(void) {
    return NimBLEAddress(_macAddress).toString();
}

const NimBLEAddress & BLESensor::mac() {
    return _macAddress;
}