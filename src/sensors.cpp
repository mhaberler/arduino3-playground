#include "Sensor.hpp"
#include "ArduinoJson.h"
#include "ArduinoJsonCustom.h"
#include <LittleFS.h>
#include "blescan.hpp"

extern SpiRamAllocator spiram_allocator;

SpiRamJsonDocument equipment(10240);

UnitSet units;

bool readEquipment(const char* dirname) {
    Serial.println("Listing files in directory: " + String(dirname));

    File root = LittleFS.open(dirname);
    if (!root) {
        Serial.println("Failed to open directory");
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

            DynamicJsonDocument unitconf(1024);
            DeserializationError error = deserializeJson(unitconf, fileContent);

            if (error) {
                Serial.println("Failed to parse JSON file: " + filePath);
            } else {
                Unit *u = new Unit();
                if (u->configure(&unitconf)) {
                    units.insert(u);
                } else {
                    delete u;
                }
            }
        }
        file = root.openNextFile();
    }
    return true;
}

void process_ble() {
    bleAdvMsg_t msg;
    if (xQueueReceive(bleadv_queue, (void *)&msg, 0) == pdTRUE) {
        bleDeliver(msg);
    }
}

void read_config(void) {
    if (!LittleFS.begin(false))
        return;
    readEquipment("/config");
}

void init_sensors(void) {
    read_config();
}

void sensor_loop(void) {
    process_ble();
}
