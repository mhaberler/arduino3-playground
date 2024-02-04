#include "Sensor.hpp"
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
            Serial.printf("%s %s ", sp->unitName().c_str(), sp->fullName().c_str());
            sp->print(Serial);
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
            _saveSequence();
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
                    uint32_t idx = 0;
                    JsonArray seq = jdoc["seq"];
                    // set per-tank index
                    for(JsonVariant v : seq) {
                        Unit *u = _unit_by_id[v.as<std::string>()];
                        if (u && u->type() == UT_TANK) {
                            log_e("%s %u", u->id().c_str(), idx);
                            u->setIndex(idx);
                            idx += 1;
                        } else {
                            log_e("no such Unit: %s",  u->id().c_str());
                        }
                    }
                    // sort the _units vector by index, i.e creation order:
                    std::sort(_units.begin(),
                              _units.end(),
                    [](Unit* const& u1, Unit* const& u2) {
                        return (u1->index() < u2->index());
                    });
                    return 0;
                }
                break;
            default:
                ;
        }
    }
    return true;
}

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

bool Equipment::_saveSequence(void) {

    JsonDocument root;

    JsonObject obj = root.to<JsonObject>();
    obj["ut"] = UT_TANK_SEQUENCE;
    JsonArray seq = obj["seq"].to<JsonArray>();
    for (auto u: _units) {
        if (u->type() == UT_TANK) {
            seq.add(u->id());
        }
    }
    // serializeJsonPretty(root, Serial);
    // Serial.printf("\n");

    String path = String(SEQUENCE_DIR) + String("/") + sanitizeLittleFSPath(String("tank_sequence")) + String(".json");

    // log_e("_saveSequence normalizePath %s", path.c_str());
    LittleFS.remove(path.c_str());
    File fd = LittleFS.open(path.c_str(), "w");
    serializeJsonPretty(root, fd);
    fd.close();
    return true;
}
