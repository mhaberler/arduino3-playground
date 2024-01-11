#include "Sensor.hpp"
#include "blescan.hpp"

unordered_map<NimBLEAddress, Sensor *> ble_sensors;
const NimBLEAddress null_mac;

bool Unit::configure(JsonObject *conf) {
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
                Serial.printf("INSERT %s\n", sp->mac().toString().c_str());
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
    if (sp) {
        // log_e("deliver %s", mac.toString().c_str());
        bool rc =  sp->bleAdvertisement(msg);
        if (rc) {
            sp->print(Serial);
        }
    }
    return false;
}

void convertFromJson(JsonVariantConst src, sensorType_t& dst) {
    dst = (sensorType_t) src.as<unsigned int>();
}

void convertFromJson(JsonVariantConst src, NimBLEAddress& dst) {
    dst = NimBLEAddress(src.as<std::string>());
}

void convertToJson(const ruuviAd_t & src, JsonVariant dst) {
    dst["temp"] =  round1(src.temperature);
    dst["hum"] =  round1(src.humidity);
    dst["press"] = round1(src.pressure);
    dst["rssi"] = src.rssi;
}

void convertToJson(const mopekaAd_t & src, JsonVariant dst) {
    dst["temp"] =  src.temperature;
    dst["level"] = src.level;
    dst["stars"] = src.qualityStars;
    dst["accX"] = src.acceloX;
    dst["accY"] = src.acceloY;
    dst["rssi"] = src.rssi;
    dst["batt"] = round2(src.battery);
    if (src.syncPressed)
        dst["sync"] = 1;

}

void convertToJson(const tpmsAd_t & src, JsonVariant dst) {
    dst["press"] = round1(src.pressure);
    dst["temp"] = round1(src.temperature);
    dst["bat"] = round1(src.batpct);
    dst["loc"] = src.location;
    dst["status"] = src.status;
    dst["rssi"] = src.rssi;
}

float round1(float value) {
    return (int)(value * 10 + 0.5) / 10.0;
}

float round2(float value) {
    return (int)(value * 100 + 0.5) / 100.0;
}

float round3(float value) {
    return (int)(value * 1000 + 0.5) / 1000.0;
}

float round4(float value) {
    return (int)(value * 10000 + 0.5) / 10000.0;
}

void setEnvMacAddress(std::string &mac) {}
void setOATMacAddress(std::string &mac) {}