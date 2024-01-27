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
    ST_MQTT_SUBSCRIPTION,
    ST_MAX
} sensorType_t;

typedef enum {
    AT_NONE,
    AT_TEMPERATURE,
    AT_HUMIDITY,
    AT_FLOW,
    AT_BURNER_ON,
    AT_BURNER_OFF,
    AT_PRESSURE,
    AT_VOLTAGE,
    AT_CURRENT,
    AT_LONGITUDE,
    AT_LATITUDE,
    AT_ALTITUDE,
    AT_HSPEED,
    AT_VSPEED,
    AT_VACCEL,
    AT_MAX
} aspect_t;

// NB: keep in sync with unitText()
typedef enum {
    UT_NONE = 0,
    UT_TANK,
    UT_BURNER,
    UT_ENVELOPE,
    UT_BASKET,
    UT_AIRCRAFT,
    UT_VEHICLE,
    UT_MAX
} unit_t;

uint8_t volt2percent(const float volt);
const char *sensorTypeStr(const sensorType_t sensorType);
const char *unitTypeStr(const unit_t unitType);

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
int32_t getInt32LE(const uint8_t *data, int index);

class Unit;

class Sensor {
  private:
    sensorMode_t _mode;
    format_t _format;
    lv_subject_t *_subject;
    uint32_t _created; // timestamp
  protected:
    NimBLEAddress _macAddress;
    Unit *_unit;
    sensorType_t _type;

  public:
    Sensor(Unit *u) : _created(millis()), _unit(u) {
    };
    void setAddress(NimBLEAddress &mac) {
        _macAddress = mac;
    };
    void setAddress(const std::string &mac) {
        _macAddress = NimBLEAddress(mac);
    };
    uint32_t created(void) {
        return _created;
    };
    virtual uint32_t lastChange(void) = 0;
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
    virtual const  std::string name(void) = 0;
    virtual const  std::string fullName(void) = 0;
    virtual  const std::string id(void) = 0;
    const  std::string unitName(void);

};

typedef unordered_set<Sensor*> SensorSet;

class Unit {
  private:
    SensorSet _sensorset;
    std::string _id;
    unit_t _ut;
    uint32_t _created; // timestamp
  public:
    Unit( std::string id) : _id(id),_created(millis()) {};
    bool configure(JsonObject *conf);
    uint32_t created(void) {
        return _created;
    };
    void print(Print &p, format_t format = FMT_TEXT);
    void add(Sensor *s);
    Sensor *get(sensorType_t st);
    void setType(const unit_t ut) {
        _ut = ut;
    };
    unit_t getType(void) {
        return _ut;
    };
    const  std::string name(void) {
        return std::string(unitTypeStr(getType())) + ":" + _id;
    };
};

Unit *addUnit(JsonObject conf);
// Unit *setupUnit(const unit_t unit, const sensorType_t sensorType, const std::string &mac);

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
  private:
    ruuviAd_t _ruuvi_report;
  public:
    Ruuvi(Unit *u) : Sensor(u) {
        _type = ST_RUUVI;
    };
    void print(Print &p, format_t format = FMT_TEXT);
    void setOnUpdate(std::function<void(const char *value)> onUpdate, aspect_t what ) {}
    bool configure(JsonObject conf);
    bool bleAdvertisement(const bleAdvMsg_t  &msg);
    uint32_t lastChange(void) {
        return _ruuvi_report.lastchange;
    };

    const std::string id(void) {
        return std::string(NimBLEAddress(_macAddress));
    };
    const std::string name(void) {
        return sensorTypeStr(type());
    };
    const std::string fullName(void) {
        return name() + ":" + id();
    }
};

class Mopeka : public Sensor {
  private:
    mopekaAd_t _mopeka_report;
    uint16_t _min_mm = 100;
    uint16_t _max_mm = 857;
    bool _decode(const uint8_t *data,
                 const size_t len, mopekaAd_t &ma);
  public:
    Mopeka(Unit *u) : Sensor(u) {
        _type = ST_MOPEKA;
    };
    void print(Print &p, format_t format = FMT_TEXT);
    void setOnUpdate(std::function<void(const char *value)> onUpdate, aspect_t what ) {}
    bool configure(JsonObject conf);
    bool bleAdvertisement(const bleAdvMsg_t  &msg);
    uint32_t lastChange(void) {
        return _mopeka_report.lastchange;
    };

    const std::string id(void) {
        return std::string(NimBLEAddress(_macAddress));
    };
    const std::string name(void) {
        return sensorTypeStr(type());
    };
    const std::string fullName(void) {
        return name() + ":" + id();
    }
};

class TPMS : public  Sensor {
  private:
    tpmsAd_t _tpms_report;

  public:
    TPMS(Unit *u) : Sensor(u) {
        _type = ST_TPMS;
    };
    void print(Print &p, format_t format = FMT_TEXT);
    void setOnUpdate(std::function<void(const char *value)> onUpdate, aspect_t what ) {}
    bool configure(JsonObject conf);
    bool bleAdvertisement(const bleAdvMsg_t  &msg);
    uint32_t lastChange(void) {
        return _tpms_report.lastchange;
    };
    const std::string id(void) {
        return std::string(NimBLEAddress(_macAddress));
    };
    const std::string name(void) {
        return sensorTypeStr(type());
    };
    const std::string fullName(void) {
        return name() + ":" + id();
    }
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
