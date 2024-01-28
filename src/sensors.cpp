#include "Sensor.hpp"
#include "ArduinoJson.h"
#include "ArduinoJsonCustom.h"
#include <LittleFS.h>
#include "blescan.hpp"
#include "TreeWalker.hpp"
#include "fsVisitor.hpp"
#include "logmacros.hpp"
#include <filesystem>

extern SpiRamAllocator spiram_allocator;

JsonDocument equipment(&spiram_allocator);

UnitMap units;

String sanitizeLittleFSPath(const String &path) {
    // Maximum length of LittleFS file/directory names
    const size_t maxNameLength = 255;

    String sanitizedPath = path;

    // Replace invalid characters with underscores
    for (char &c : sanitizedPath) {
        if (c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|') {
            c = '_';
        }
    }

    // Trim leading and trailing spaces
    sanitizedPath.trim();

    // Limit length of file/directory names
    if (sanitizedPath.length() > maxNameLength) {
        sanitizedPath = sanitizedPath.substring(0, maxNameLength);
    }

    return sanitizedPath;
}

bool saveUnit(const std::string &id, const JsonArray &array) {
    String path = String(TOPDIR) + String("/") + sanitizeLittleFSPath(String(id.c_str())) + String(".json");

    log_e("saveUnit normalizePath '%s' -> %s", id.c_str(), path.c_str());
    LittleFS.remove(path.c_str());
    File fd = LittleFS.open(path.c_str(), "w");
    serializeJsonPretty(array, fd);
    fd.close();
    return true;
}

Unit *addUnit(JsonObject conf, bool save) {
    std::string id = conf["id"];
    if (id.empty()) {
        log_e("missing id");
        serializeJson(conf, Serial);
        return NULL;
    }
    Unit *u = new Unit(id);
    if (u->configure(&conf)) {
        // delete previous unit
        if (units[id]) {
            delete units[id];
        }
        units[id] = u;
        if (save) {
            // wrap in array
            JsonDocument doc;
            JsonArray array = doc.to<JsonArray>();
            array.add(conf);
            saveUnit(id, array);
        }
    } else {
        log_e("configure failed: %s", id.c_str());
        delete u;
        return NULL;
    }
    return u;
}

bool wipeLittleFS(void) {
    LittleFS.begin(false);
    LittleFS.format();
    return LittleFS.mkdir(TOPDIR); 
    // LittleFS strangeness deleting files
    // fsVisitor(LittleFS, Serial, dirname, VA_DEBUG | VA_ERASE_FILES );
}

void readEquipment(const char* dirname) {
    fsVisitor(LittleFS, Serial, dirname, VA_DEBUG | VA_LOAD_CONFIG );
}

bool wipe_lfs(void) {
    if (LittleFS.begin(false)) {
        return LittleFS.format();
    }
    return false;
}

void process_ble() {
    bleAdvMsg_t msg;
    if (xQueueReceive(bleadv_queue, (void *)&msg, 0) == pdTRUE) {
        bleDeliver(msg);
    }
}

void read_config(void) {
    if (!LittleFS.begin(false)) {
        log_e("LittleFS mount failed");
        return;
    }
    LittleFS.mkdir(TOPDIR); // just in case
    readEquipment(TOPDIR);
}

void init_sensors(void) {
    read_config();
}

void sensor_loop(void) {
    process_ble();
}

bool loadUnitFile(const char* path) {
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
