#include "Sensor.hpp"

#if 0
bool Consumer::configure(JsonObject *conf) {
    log_e("Unit::configure");

    serializeJson((*conf),Serial);
    Serial.printf("\n");

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
        }

        if (sp && sp->configure(s)) {
            _actorset.insert(sp);
            if (sp->mac() != null_mac) {
                Serial.printf("add BLE %s:%s %s:%s\n",
                              unitTypeStr(ut),
                              id.c_str(),
                              sensorTypeStr(st),
                              sp->mac().toString().c_str());
                ble_sensors[sp->mac()] = sp;
            }
        }
    }
    return true;
}
#endif

