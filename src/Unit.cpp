#include "Sensor.hpp"
#include "blescan.hpp"

unordered_map<NimBLEAddress, Sensor *> ble_sensors;
const NimBLEAddress null_mac;

bool Unit::configure(JsonDocument *conf) {
    JsonArray sensors = (*conf)["sensors"].as<JsonArray>();

    for(JsonObject s: sensors) {
        Sensor *sp = NULL;
        switch (s["type"].as<int>()) {
            case ST_RUUVI:
                sp = new Ruuvi();
                break;
            case ST_MOPEKA:
                sp = new Mopeka();
                break;
            case ST_TPMS:
                sp = new TPMS();
                break;
            case ST_GPS:
                break;
            case ST_FLOWSENSOR:
                break;
            case ST_BARO:
                break;
            case ST_IMU:
                break;
            case ST_MAGNETOMETER:
                break;
        }
        serializeJson(s, Serial);
        if (sp && sp->configure(s)) {
            _sensorset.insert(sp);
            if (sp->mac() != null_mac) {
                // Serial.printf("INSERT %s\n", sp->mac().toString().c_str());
                ble_sensors[sp->mac()] = sp;
            }
            const char *mac = s["mac"].as<const char *>();
            Serial.print(mac);
        }
    }
    return true;
}

const String &Unit::name(void) {
    return "foo";
}

void Unit::print(Print &p, format_t format) {}

void Unit::add(Sensor *s) {
    _sensorset.insert(s);
};

bool bleDeliver(const bleAdvMsg_t &msg) {

    NimBLEAddress mac = NimBLEAddress(msg.mac64);
    Sensor *sp = ble_sensors[mac];
    log_e("deliver %s %p", mac.toString().c_str(), sp);
    if (sp) {
        return sp->bleAdvertisement(msg);
    }
    return false;
}

void convertFromJson(JsonVariantConst src, sensorType_t& dst) {
    dst = (sensorType_t) src.as<unsigned int>();
}

void convertFromJson(JsonVariantConst src, NimBLEAddress& dst) {
    // Serial.printf("convertFromJson mac=%s\n", dst.toString().c_str());
    dst = NimBLEAddress(src.as<std::string>());
}

void setEnvMacAddress(std::string &mac) {}
void setOATMacAddress(std::string &mac) {}