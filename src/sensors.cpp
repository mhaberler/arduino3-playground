#include "Sensor.hpp"
#include "ArduinoJson.h"
#include "ArduinoJsonCustom.h"
#include <LittleFS.h>
#include "blescan.hpp"
#include "TreeWalker.hpp"
#include "fsVisitor.hpp"
#include "logmacros.hpp"
#include <filesystem>

Equipment equipment;
static bool reconfigure_tanks;

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
    LittleFS.mkdir(CONFIG_DIR);
    LittleFS.mkdir(UNITS_DIR); // just in case
    LittleFS.mkdir(SEQUENCE_DIR);
    // LittleFS strangeness deleting files
    // fsVisitor(LittleFS, Serial, dirname, VA_DEBUG | VA_ERASE_FILES );
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
        equipment.bleDeliver(msg);
    }
}

void read_config(void) {
    LittleFS.begin(true);
    LittleFS.mkdir(CONFIG_DIR);
    LittleFS.mkdir(UNITS_DIR);
    LittleFS.mkdir(SEQUENCE_DIR);
    equipment.read(UNITS_DIR, VA_LOAD_UNITS);
    equipment.read(SEQUENCE_DIR, VA_LOAD_SEQUENCE);
    equipment.emitTankSequence();
}

void init_sensors(void) {
    read_config();
}

void sensor_loop(void) {
    process_ble();
    if (reconfigure_tanks) {
        equipment.emitTankSequence();
        reconfigure_tanks = false;
    }
}

void schedule_UI_reconfigure() {
    reconfigure_tanks = true;
}