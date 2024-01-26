#include "Sensor.hpp"
#include "blescan.hpp"

unordered_map<NimBLEAddress, Sensor *> ble_sensors;
const NimBLEAddress null_mac;

bool Unit::configure(JsonObject *conf) {
    log_e("Unit::configure");

    serializeJson((*conf),Serial);
    Serial.printf("\n");

    int ut = (*conf)["ut"].as<int>();
    setType((unit_t)ut);
    String dsc = (*conf)["dsc"];
    String id = (*conf)["id"];
    JsonArray sensors = (*conf)["sensors"].as<JsonArray>();

    for(JsonObject s: sensors) {
        Sensor *sp = NULL;
        int32_t st = s["st"].as<int>();
        switch (st) {
            case ST_RUUVI:
                sp = new Ruuvi(s["mac"].as<std::string>());
                sp->setUnit(this);
                break;
            case ST_MOPEKA:
                sp = new Mopeka();
                sp->setUnit(this);
                break;
            case ST_TPMS:
                sp = new TPMS();
                sp->setUnit(this);
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

        if (sp && sp->configure(s)) {
            _sensorset.insert(sp);
            if (sp->mac() != null_mac) {
                Serial.printf("INSERT %s:%s %s:%s\n",
                              unitType((unit_t)ut),
                              id.c_str(),
                              sensorType((sensorType_t)st),
                              sp->mac().toString().c_str());
                ble_sensors[sp->mac()] = sp;
            }
        }
    }
    return true;
}



void Unit::print(Print &p, format_t format) {}

void Unit::add(Sensor *s) {
    _sensorset.insert(s);
};

Unit *setupUnit(const unit_t unit, const sensorType_t sensorType, const std::string &mac) {
    JsonDocument config;
    switch (unit) {
        case UT_ENVELOPE:
            config["id"] = "envelope";
            break;
        case UT_OAT:
            config["id"] = "OAT";
            break;
        default:
            return NULL;
    }
    config["ut"] = (int) unit;
    JsonArray sensors = config["sensors"].to<JsonArray>();
    JsonObject sensor = sensors.add<JsonObject>();
    sensor["st"] = (int)sensorType;
    sensor["mac"] = mac;
    return addUnit(config.as<JsonObject>());

}

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

uint8_t volt2percent(const float v) {
    // convert voltage and scale for CR2032
    float percent = (v - 2.2f) / 0.65f * 100.0f;
    if (percent < 0.0f) {
        return 0;
    }
    if (percent > 100.0f) {
        return 100;
    }
    return (uint8_t) percent;
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
    dst["batt"] = volt2percent((float)src.voltage/1000.0);
    dst["rssi"] = src.rssi;
}

void convertToJson(const mopekaAd_t & src, JsonVariant dst) {
    dst["temp"] =  src.temperature;
    dst["level"] = src.level;
    dst["stars"] = src.qualityStars;
    dst["accX"] = src.acceloX;
    dst["accY"] = src.acceloY;
    if (src.syncPressed)
        dst["sync"] = 1;
    dst["rssi"] = src.rssi;
    dst["batt"] = volt2percent(src.battery);
}

void convertToJson(const tpmsAd_t & src, JsonVariant dst) {
    dst["press"] = round1(src.pressure);
    dst["temp"] = round1(src.temperature);
    dst["loc"] = src.location;
    dst["status"] = src.status;
    dst["batt"] = src.batpct;
    dst["rssi"] = src.rssi;
}
