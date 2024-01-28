#include "Sensor.hpp"
#include "ArduinoJson.h"
#include "ArduinoJsonCustom.h"
#include <LittleFS.h>
#include "blescan.hpp"
#include "TreeWalker.hpp"
#include "fsVisitor.hpp"
#include "logmacros.hpp"
#include <filesystem>

Equipment equipment(TOPDIR);

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
