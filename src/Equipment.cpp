#include "Sensor.hpp"
#include "fsVisitor.hpp"
#include <LittleFS.h>

typedef std::pair<std::string, Unit *> UnitsEntry;

struct {
    bool operator()(const UnitsEntry &a, const UnitsEntry &b) const {
        return a.second->created() < b.second->created();
    }
}
unitsLess;

static void _visit_unit(const UnitVisitor &unitVisitor,  Unit *u, const uint32_t flags) {
    if ((flags & UV_TANKS_ONLY) && (u->type() != UT_TANK)) {
        return;
    }
    unitVisitor(*u, flags);
}

void Equipment::walk(const UnitVisitor &unitVisitor, const uint32_t flags) {

    if (flags % UV_SORT_BY_TIMESTAMP) {
        std::vector<UnitsEntry> sorted_units(_units.begin(), _units.end());
        std::sort(sorted_units.begin(), sorted_units.end(), unitsLess);
        for (UnitsEntry ue: sorted_units) {
            _visit_unit(unitVisitor, ue.second, flags);
        }
    } else {
        for (auto u : _units) {
            _visit_unit(unitVisitor, u.second, flags);
        }
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

void Equipment::read(const char* dirname) {
    fsVisitor(LittleFS, Serial, dirname, VA_DEBUG | VA_LOAD_CONFIG );

}

bool Equipment::addUnit(JsonObject conf, bool save) {
    std::string id = conf["id"];
    if (id.empty()) {
        log_e("missing id");
        serializeJson(conf, Serial);
        return false;
    }
    Unit *u = new Unit(id);
    if (u->configure(*this, &conf)) {
        // delete previous unit
        if (_units[id]) {
            delete _units[id];
        }
        _units[id] = u;
        if (save) {
            // wrap in array
            JsonDocument doc;
            JsonArray array = doc.to<JsonArray>();
            array.add(conf);
            _saveUnit(id, array);
        }
    } else {
        log_e("configure failed: %s", id.c_str());
        delete u;
        return false;
    }
    return true;
}

bool Equipment::addUnit(const char *path) {
    File fd = LittleFS.open(path, "r");
    String fileContent = fd.readString();
    fd.close();

    JsonDocument unitconf;
    DeserializationError error = deserializeJson(unitconf, fileContent);

    if (error) {
        Serial.printf("parse errpr: %s : %s\n", path,error.c_str());
    } else {
        if (unitconf.is<JsonArray>()) {
            JsonArray ua = unitconf.as<JsonArray>();
            for (JsonVariant element : ua) {
                addUnit(element.as<JsonObject>(), false); // do NOT save
            }
        } else  {
            log_e("config not an array, skipping: %s ", path);
            serializeJsonPretty(unitconf, Serial);
        }
    }
    return true;
}

void Equipment::dump(Stream &s) {
    for(auto u: _units) {
        s.printf("unit '%s':  %s\n", u.first.c_str(), unitTypeStr(u.second->type()));
        u.second->dump(Serial);
    }
}

bool Equipment::_saveUnit(const std::string &id, const JsonArray &array) {
    String path = String(_topdir) + String("/") + sanitizeLittleFSPath(String(id.c_str())) + String(".json");

    log_e("saveUnit normalizePath '%s' -> %s", id.c_str(), path.c_str());
    LittleFS.remove(path.c_str());
    File fd = LittleFS.open(path.c_str(), "w");
    serializeJsonPretty(array, fd);
    fd.close();
    return true;
}