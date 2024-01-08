#include "Sensor.hpp"
#include "ArduinoJson.h"
#include "ArduinoJsonCustom.h"
#include <LittleFS.h>

extern SpiRamAllocator spiram_allocator;
JsonDocument equipment(&spiram_allocator);


void readEquipment(const char* dirname, JsonArray &ua) {
    Serial.println("Listing files in directory: " + String(dirname));

    File root = LittleFS.open(dirname);
    if (!root) {
        Serial.println("Failed to open directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            readEquipment(file.name(),ua);
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
            String filePath = String(dirname) + "/" + file.name();
            File fd = LittleFS.open(filePath, "r");

            String fileContent = fd.readString();
            fd.close();

            // Parse the file content into a JSON document
            DynamicJsonDocument unit(1024); // adjust the size based on your data

            // Deserialize the JSON
            DeserializationError error = deserializeJson(unit, fileContent);

            if (error) {
                Serial.println("Failed to parse JSON file: " + filePath);
            } else {
                ua.add(unit);

            }
        }
        file = root.openNextFile();
    }
}

void read_config(void) {
    if (!LittleFS.begin())
        return;
    JsonArray units = equipment.createNestedArray("units");

    readEquipment("/config", units);
    serializeJsonPretty(equipment, Serial);
    Serial.println();
}

void init_sensors(void) {
    // TPMS *tpms = new TPMS();
    // Mopeka *mopeka = new Mopeka();
    // Unit *u = new Unit();
    // u->add(tpms);
    // u->add(mopeka);

    // e.add(u);
    read_config();
}

void sensor_loop(void) {
    // e.render();
}
