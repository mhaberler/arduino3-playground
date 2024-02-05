#include "Sensor.hpp"
#include "blescan.hpp"

const NimBLEAddress null_mac;

void Unit::dump(Stream &s) {
    for(auto a: _actor_map) {
        s.printf("\tactor '%s': ", a.first.c_str());
        a.second->dump(Serial);
    }
}

bool Unit::configure(Equipment &eq, JsonObject *conf) {
    unit_t ut = (*conf)["ut"].as<unit_t>();
    setType(ut);
    String dsc = (*conf)["dsc"];
    String id = (*conf)["id"];
    JsonArray sensors = (*conf)["sensors"].as<JsonArray>();

    for(JsonObject s: sensors) {
        Sensor *sp = NULL;
        actorType_t st = s["st"].as<actorType_t>();
        switch (st) {
            case AT_RUUVI:
                sp = new Ruuvi(this);
                break;
            case AT_MOPEKA:
                sp = new Mopeka(this);
                break;
            case AT_TPMS:
                sp = new TPMS(this);
                break;
            case AT_GPS:
                break;
            case AT_FLOWSENSOR:
                break;
            case AT_BARO:
                break;
            case AT_IMU:
                break;
            case AT_MAGNETOMETER:
                break;
            case AT_MQTT_SUBSCRIPTION:
                break;
            case AT_UI: {
                    // Binding *bp = new Binding();
                }
                break;
            case AT_LOG:
                break;
        }

        if (sp && sp->configure(s)) {
            _actor_map[sp->id()] = sp;
            if (sp->mac() != null_mac) {
                // Serial.printf("add BLE %s:%s %s:%s\n",
                //               unitTypeStr(ut),
                //               id.c_str(),
                //               sensorTypeStr(st),
                //               sp->mac().toString().c_str());
                eq.bleRegister(sp->mac(), sp);
            }
        }
    }
    return true;
}

void Unit::print(Print &p, format_t format) {}

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

void convertFromJson(JsonVariantConst src, actorType_t& dst) {
    dst = (actorType_t) src.as<unsigned int>();
}

void convertFromJson(JsonVariantConst src, NimBLEAddress& dst) {
    dst = NimBLEAddress(src.as<std::string>());
}

void convertToJson(const ruuviAd_t & src, JsonVariant dst) {
    if (src.availability & RUUVI_TEMPERATURE_AVAILABLE)
        dst["temp"] =  round1(src.temperature);
    if (src.availability & RUUVI_HUMIIDTY_AVAILABLE)
        dst["hum"] =  round1(src.humidity);
    if (src.availability & RUUVI_PRESSURE_AVAILABLE)
        dst["press"] = round1(src.pressure);
    if (src.availability & RUUVI_BATTERY_AVAILABLE)
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
