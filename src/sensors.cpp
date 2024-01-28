#include "Sensor.hpp"
#include "ArduinoJson.h"
#include "ArduinoJsonCustom.h"
#include <LittleFS.h>
#include "blescan.hpp"
#include "TreeWalker.hpp"
#include "fsVisitor.hpp"
#include "logmacros.hpp"
#include <filesystem>

#define TOPDIR "/equipment"

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

    log_e("normalizePath '%s' -> %s", id.c_str(), path.c_str());
    LittleFS.remove(path.c_str());
    File fd = LittleFS.open(path.c_str(), "w");
    serializeJsonPretty(array, fd);

    fd.close();
    return true;
}

Unit *addUnit(JsonObject conf) {
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
        // wrap in array
        JsonDocument doc;
        JsonArray array = doc.to<JsonArray>();
        array.add(conf);
        saveUnit(id, array);
    } else {
        log_e("configure failed: %s", id.c_str());
        delete u;
        return NULL;
    }
    return u;
}

static bool _fs_visit(fs::FS &fs, Stream &out, fs::File &f, uint32_t flags) {
    bool result = false;
    time_t t = f.getLastWrite();
    struct tm *tm = localtime(&t);
    bool isDir = f.isDirectory();
    if (flags & VA_PRINT) {
        log_e("%s %u %s", isDir ? "d" : "f", f.size(),
              f.path());
        // LOGD("{} {:>7} {:<50} {:%Y-%m-%d %H:%M:%S}", isDir ? "d" : "f", f.size(),
        //      f.path(), *tm);
    }
    if (isDir) {
        return true;
    }
    //   if (flags & VA_CACHE) {
    //     result = psramCache.insert(fs, f.path());
    //     if (!result && (flags & VA_DEBUG)) {
    //       LOGD("toPsram() failed, stopping");
    //     }
    //   }
    return result;
}

bool emptyDir(const char* dirname) {

    log_e("listing LittleFS  directory:  %s", dirname);
    fsVisitor(LittleFS, Serial, dirname, VA_PRINT | VA_DEBUG );

    return false;
}

bool readEquipment(const char* dirname) {
    Serial.println("Listing files in directory: " + String(dirname));

    File root = LittleFS.open(dirname);
    if (!root) {
        log_e("Failed to open directory");
        return false;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            readEquipment(file.name());
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
            String filePath = String(dirname) + "/" + file.name();
            File fd = LittleFS.open(filePath, "r");

            String fileContent = fd.readString();
            fd.close();

            JsonDocument unitconf;
            DeserializationError error = deserializeJson(unitconf, fileContent);

            if (error) {
                Serial.printf("parse errpr: %s : %s\n", filePath.c_str(),error.c_str());
            } else {
                if (unitconf.is<JsonArray>()) {
                    JsonArray ua = unitconf.as<JsonArray>();
                    for (JsonVariant element : ua) {
                        addUnit(element.as<JsonObject>());//FIXME retcode
                    }
                } else  {
                    log_e("config not an array, skipping: %s ", filePath.c_str());
                    serializeJsonPretty(unitconf, Serial);

                }
            }
        }
        file = root.openNextFile();
    }
    return true;
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
    emptyDir("/");
    LittleFS.mkdir(TOPDIR); // just in case
    readEquipment(TOPDIR);
}

void init_sensors(void) {
    read_config();
}

void sensor_loop(void) {
    process_ble();
}
