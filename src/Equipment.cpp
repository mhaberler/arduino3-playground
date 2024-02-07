#include "Sensor.hpp"
#include "lv_subjects.hpp"
#include "ui_message.hpp"
#include "fsVisitor.hpp"
#include <LittleFS.h>

typedef std::pair<std::string, Unit *> UnitsEntry;

struct {
    bool operator()(const UnitsEntry &a, const UnitsEntry &b) const {
        return a.second->timestamp() < b.second->timestamp();
    }
}
unitsLess;

static void _visit_unit(const UnitVisitor &unitVisitor,  Unit *u, const uint32_t flags, void *user_data) {
    if ((flags & UV_TANKS_ONLY) && (u->type() != UT_TANK)) {
        return;
    }
    unitVisitor(*u, flags, user_data);
}

void Equipment::walk(const UnitVisitor &unitVisitor, const uint32_t flags, void *user_data) {
    for (auto u : _units) {
        _visit_unit(unitVisitor, u, flags, user_data);
    }
}

void Equipment::sensorToUI(Sensor *sp) {

    JsonDocument doc;
    const void *decoded = sp->pod();
    actorType_t at = sp->type();
    unit_t ut = sp->unit()->type();

    switch (at) {
        case AT_RUUVI: {
                const ruuviAd_t *p = (ruuviAd_t*) decoded;
                doc = *p;
                doc["st"] = AT_RUUVI;
            }
            break;
        case AT_MOPEKA: {
                const mopekaAd_t *p = (mopekaAd_t*) decoded;
                doc = *p;
                doc["st"] = AT_MOPEKA;
                doc["pct"] = (int) percentBetween(sp->min(), sp->max(), p->level);
            }
            break;
        case AT_TPMS: {
                const tpmsAd_t *p = (tpmsAd_t*) decoded;
                doc = *p;
                doc["st"] = AT_TPMS;
            }
            break;
        default:
            ;
    }
    doc["ut"] = (int)ut;
    switch (ut) {
        case UT_BASKET:
            doc["um"] = (int)UM_SENSOR_OAT;
            break;
        case UT_ENVELOPE:
            doc["um"] = (int)UM_SENSOR_ENVELOPE;
            break;
        case UT_TANK:
            doc["um"] = (int)UM_SENSOR_TANK;
            doc["ix"] = sp->unit()->index();
            doc["col"] = sp->unit()->tagColor();
            break;
        default:
            break;
    }
    sendUiMessage(doc);
}

bool Equipment::bleRegister(const NimBLEAddress &mac, Sensor *sp) {

    Sensor *old =  _ble_sensors[mac];
    _ble_sensors[mac] = sp;

    if (old != NULL) {
        delete old;
    }
    return true;
}

bool Equipment::bleDeliver(const bleAdvMsg_t &msg) {
    NimBLEAddress mac = NimBLEAddress(msg.mac64);
    Sensor *sp = _ble_sensors[mac];
    if (sp) {
        // log_e("deliver %s", mac.toString().c_str());
        bool rc =  sp->bleAdvertisement(msg);
        if (rc) {
            // Serial.printf("%s %s ", sp->unitName().c_str(), sp->fullName().c_str());
            // sp->print(Serial);
            sensorToUI(sp);
        }
    }
    return false;
}

void Equipment::read(const char* dirname, uint32_t flags) {
    fsVisitor(LittleFS, Serial, dirname, flags );
}

void Equipment::delUnit(const std::string &id)      {
    Unit *u = NULL;
    _units.erase(std::remove_if(_units.begin(), _units.end(), [&](Unit *x) {
        if (x->id() == id) {
            u = x;
            return true;
        }
        return false;
    }), _units.end());
    if (u)
        delete u;
}

bool Equipment::addUnit(JsonObject conf, source_t source) {
    std::string id = conf["id"];
    if (id.empty()) {
        log_e("missing id");
        serializeJson(conf, Serial);
        return false;
    }
    Unit *u = new Unit(id);
    if (u->configure(*this, &conf)) {
        if (source == SRC_NFC) {
            u->setTimestamp(millis());
        }
        delUnit(id);
        _units.push_back(u);
        _unit_by_id[id] = u;

        if (source == SRC_NFC) {
            // wrap in array and save
            JsonDocument doc;
            doc.add(conf);
            _saveUnit(id, doc);
            _updateSequence();
        }
        return true;
    } else {
        log_e("configure failed: %s", id.c_str());
        _unit_by_id.erase(id);
        delete u;
        return false;
    }
}

bool Equipment::addUnit(const char *path) {
    File fd = LittleFS.open(path, "r");
    String fileContent = fd.readString();
    fd.close();

    JsonDocument unitconf;
    DeserializationError error = deserializeJson(unitconf, fileContent);

    if (error) {
        Serial.printf("parse errpr: %s : %s\n", path, error.c_str());
    } else {
        if (unitconf.is<JsonArray>()) {
            JsonArray ua = unitconf.as<JsonArray>();
            for (JsonVariant element : ua) {
                addUnit(element.as<JsonObject>(), SRC_FLASHFS); // do NOT save
            }
        } else  {
            log_e("config not an array, skipping: %s ", path);
            serializeJsonPretty(unitconf, Serial);
        }
    }
    return true;
}

bool Equipment::restoreSequence(const char *path) {
    File fd = LittleFS.open(path, "r");
    String fileContent = fd.readString();
    fd.close();
    JsonDocument jdoc;
    DeserializationError error = deserializeJson(jdoc, fileContent);
    if (error) {
        Serial.printf("parse errpr: %s : %s\n", path, error.c_str());
    } else {
        unit_t t = jdoc["ut"] | UT_NONE;
        switch (t) {
            case UT_TANK_SEQUENCE: {
                    _tanks.clear();
                    int8_t idx = 0;
                    JsonArray seq = jdoc["seq"];
                    // set per-tank index
                    for(JsonVariant v : seq) {
                        Unit *u = _unit_by_id[v.as<std::string>()];
                        if (u && u->type() == UT_TANK) {
                            log_e("%s %u", u->id().c_str(), idx);
                            u->setTimestamp(idx);
                            u->setIndex(idx);
                            _tanks.push_back(u);
                            idx += 1;
                        } else {
                            log_e("no such Unit: %s",  u->id().c_str());
                        }
                    }
                    // // sort the _units vector by index, i.e creation order:
                    // std::sort(_units.begin(),
                    //           _units.end(),
                    // [](Unit* const& u1, Unit* const& u2) {
                    //     return (u1->timestamp() < u2->timestamp());
                    // });
                    return 0;
                }
                break;
            default:
                ;
        }
    }
    return true;
}

// send sensors available to UI?

// send tank layout to UI
// UI needs:
// id, color, sensors available (bitmap) %full (if available), bar (if available)
// [
//     {"id": "5020/16", "color" : 21321312, "sa" : 3, "f1": 73, "press" : 9.2},
//     ...
// ]

void Equipment::dump(Stream &s) {
    for(auto u: _units) {
        s.printf("unit '%s':  %s\n", u->id().c_str(), unitTypeStr(u->type()));
        u->dump(Serial);
    }
}

bool Equipment::_saveUnit(const std::string &id, const JsonDocument &doc) {
    String path = String(UNITS_DIR) + String("/") + sanitizeLittleFSPath(String(id.c_str())) + String(".json");

    // log_e("saveUnit normalizePath '%s' -> %s", id.c_str(), path.c_str());
    LittleFS.remove(path.c_str());
    File fd = LittleFS.open(path.c_str(), "w");
    serializeJsonPretty(doc, fd);
    fd.close();
    return true;
}

bool Equipment::_updateSequence(void) {
    _tanks.clear();
    for (auto u: _units) {
        if (u->type() == UT_TANK) {
            _tanks.push_back(u);
        }
    }
    // sort the _tanks vector by index, i.e creation order:
    std::sort(_tanks.begin(),
              _tanks.end(),
    [](Unit* const& u1, Unit* const& u2) {
        return (u1->timestamp() < u2->timestamp());
    });

    JsonDocument root;
    JsonObject obj = root.to<JsonObject>();

    obj["ut"] = UT_TANK_SEQUENCE;
    JsonArray seq = obj["seq"].to<JsonArray>();
    int8_t idx = 0;
    for (auto u: _tanks) {
        u->setIndex(idx);
        idx += 1;
        seq.add(u->id());
    }
    serializeJsonPretty(root, Serial);
    Serial.printf("\n");

    String path = String(SEQUENCE_DIR) + String("/") + sanitizeLittleFSPath(String("tank_sequence")) + String(".json");

    // log_e("_updateSequence normalizePath %s", path.c_str());
    LittleFS.remove(path.c_str());
    File fd = LittleFS.open(path.c_str(), "w");
    serializeJsonPretty(root, fd);
    fd.close();
    return true;
}
