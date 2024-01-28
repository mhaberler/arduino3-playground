#include "Sensor.hpp"
#include "ArduinoJson.h"
#include "ArduinoJsonCustom.h"
#include <LittleFS.h>
#include "blescan.hpp"
#include "TreeWalker.hpp"
#include "fsVisitor.hpp"
#include "logmacros.hpp"

extern SpiRamAllocator spiram_allocator;

JsonDocument equipment(&spiram_allocator);

UnitMap units;

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
        // save unit as <id>.json

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
    fsVisitor(LittleFS, Serial, dirname , VA_PRINT | VA_DEBUG );

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

    readEquipment("/equipment");
}

void init_sensors(void) {
    read_config();
}

void sensor_loop(void) {
    process_ble();
}
