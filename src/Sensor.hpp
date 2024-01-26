#pragma once

#include "Arduino.h"
#include "ArduinoJson.h"
#include "ArduinoJsonCustom.h"
#include <list>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include "NimBLEAddress.h"
#include "blescan.hpp"
#include "esp_log.h"
#include "ruuvi.h"
#include "mopeka.h"
#include "tpms.h"
#include "lv_observer.h"

using namespace std;

#define round1(x) (round((x)*1.0e1) / 1.0e1)
#define round2(x) (round((x)*1.0e2) / 1.0e2)
#define round3(x) (round((x)*1.0e3) / 1.0e3)
#define round4(x) (round((x)*1.0e4) / 1.0e4)
#define round5(x) (round((x)*1.0e5) / 1.0e5)
#define round6(x) (round((x)*1.0e6) / 1.0e6)

typedef enum {
    FMT_NONE=0,
    FMT_TEXT,
    FMT_JSON,
    FMT_CSV,
    FMT_PROTOBUF,
    FMT_TELEPLOT,
    FMT_MSGPACK,
    FMT_MAX
} format_t;

typedef enum {
    SM_NONE=0,
    SM_EVENT_DRIVEN,
    SM_PERIODIC,
    SM_MAX
} sensorMode_t;

// NB: keep in sync with sensorTypeText() 

typedef enum {
    ST_NONE = 0,
    ST_RUUVI,
    ST_MOPEKA,
    ST_TPMS,
    ST_GPS,
    ST_FLOWSENSOR,
    ST_BARO,
    ST_IMU,
    ST_MAGNETOMETER,
    ST_MAX
} sensorType_t;

typedef enum {
    FT_NONE,
    FT_TEMPERATURE,
    FT_HUMIDITY,
    FT_PRESSURE,
    FT_VOLTAGE,
    FT_CURRENT,
    FT_LONGITUDE,
    FT_LATITUDE,
    FT_ALTITUDE,
    FT_SPEED,
    FT_VSPEED,
    FT_VACCEL,
    FT_MAX
} facette_t;

// NB: keep in sync with unitText() 
typedef enum {
    UT_NONE = 0,
    UT_TANK,
    UT_BURNER,
    UT_ENVELOPE,
    UT_OAT,
    UT_FLOW,
    UT_BLASTVALVE1,
    UT_BLASTVALVE2,
    UT_BLASTVALVE3,
    UT_BLASTVALVE4,
    UT_MAX
} unit_t;

uint8_t volt2percent(const float volt);
const char *sensorType(const sensorType_t sensorType);
const char *unitType(const unit_t unitType);

void convertFromJson(JsonVariantConst src, sensorType_t& dst);
void convertFromJson(JsonVariantConst src, NimBLEAddress& dst);

void convertToJson(const ruuviAd_t & src, JsonVariant dst);
void convertToJson(const mopekaAd_t & src, JsonVariant dst);
void convertToJson(const tpmsAd_t & src, JsonVariant dst);

int16_t getInt16(const uint8_t *data, int index);
uint16_t getUint16(const uint8_t *data, int index);
int32_t getInt32(const uint8_t *data, int index);
uint32_t getUint32(const uint8_t *data, int index);
uint8_t getUint8(const uint8_t *data, int index);
int8_t getInt8(const uint8_t *data, int index);

class Unit;

class Sensor {
  private:
    sensorMode_t _mode;
    sensorType_t _type;
    format_t _format;
    uint32_t _lastchange;
    NimBLEAddress _macAddress;
    lv_subject_t *_subject;
    Unit *_unit;

  public:
    // Sensor( const std::string &mac, const sensorType_t st) : _macAddress(NimBLEAddress(mac)), _type(st) {};
    // Sensor( NimBLEAddress &mac, const sensorType_t st) : _macAddress(NimBLEAddress(mac)), _type(st) {};
    void setAddress(NimBLEAddress &mac) {
        _macAddress = mac;
    };
    void setAddress(const std::string &mac) {
        _macAddress = NimBLEAddress(mac);
    };
    void setType(const sensorType_t st) {
        _type = st;
    };
    void setUnit(Unit *u) {
        _unit = u;
    };

    virtual void print(Print &p, format_t format = FMT_TEXT) = 0;
    sensorMode_t mode();
    sensorType_t type();
    format_t format();
    NimBLEAddress & mac();
    bool configure(JsonObject conf);
    virtual bool bleAdvertisement(const bleAdvMsg_t  &msg) = 0;
    void setSubject(lv_subject_t *subject) {
        _subject = subject;
    };

};

typedef unordered_set<Sensor*> SensorSet;

class Unit {
  private:
    SensorSet _sensorset;
    std::string _id;
  public:
    Unit( std::string id) : _id(id) {};
    bool configure(JsonObject *conf);
    const  std::string &name();
    void print(Print &p, format_t format = FMT_TEXT);
    void add(Sensor *s);
    Sensor *get(sensorType_t st);
};

Unit *addUnit(JsonObject conf);
Unit *setupUnit(const unit_t unit, const sensorType_t sensorType, const std::string &mac);

// typedef unordered_set<Unit*> UnitSet;
typedef unordered_map<std::string, Unit *> UnitMap;

// class Equipment {
//   private:
//     UnitMap _unitmap;
//   public:

//     // void add(Unit *u) {
//     //     _unitmap.insert(u);
//     // };
//     void render() {
//         for(auto u: _unitmap) {
//             Serial.printf("unit '%s': ");
//             u.second->print(Serial);
//         }
//     };
// };

class Ruuvi : public Sensor {
  public:
    Ruuvi( const std::string &mac) {
        setAddress(mac);
        setType(ST_RUUVI);
    };

    void print(Print &p, format_t format = FMT_TEXT);
    void setOnUpdate(std::function<void(const char *value)> onUpdate, facette_t what ) {}
    bool configure(JsonObject conf);
    bool bleAdvertisement(const bleAdvMsg_t  &msg);

  private:
    ruuviAd_t _ruuvi_report;
};

class Mopeka : public Sensor {
  private:
    mopekaAd_t _mopeka_report;
    uint16_t _min_mm = 100;
    uint16_t _max_mm = 857;
    bool decode(const uint8_t *data,
                const size_t len, mopekaAd_t &ma);

  public:
    void print(Print &p, format_t format = FMT_TEXT);
    void setOnUpdate(std::function<void(const char *value)> onUpdate, facette_t what ) {}
    bool configure(JsonObject conf);
    bool bleAdvertisement(const bleAdvMsg_t  &msg);
};

class TPMS : public  Sensor {
  private:
    tpmsAd_t _tpms_report;

  public:
    void print(Print &p, format_t format = FMT_TEXT);
    void setOnUpdate(std::function<void(const char *value)> onUpdate, facette_t what ) {}
    bool configure(JsonObject conf);
    bool bleAdvertisement(const bleAdvMsg_t  &msg);
};

class GPS : public Sensor {
  public:

};

class Barometer : public Sensor {
  public:

};

class IMU : public Sensor {
  public:

};

bool bleDeliver(const bleAdvMsg_t &msg);

const char *unitText(const int32_t ut);
const char *sensorTypeText(const int32_t st);
