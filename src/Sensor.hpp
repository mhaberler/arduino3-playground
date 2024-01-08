#pragma once

#include "Arduino.h"
#include <list>
#include <set>
#include <unordered_set>
#include "ruuvi.h"
#include "esp_log.h"

using namespace std;

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


typedef enum {
  UT_NONE = 0,
  UT_TANK,
  UT_BURNER,
  UT_ENVELOPE,
  UT_BASKET,
  UT_FLOW,
  UT_BLASTVALVE1,
  UT_BLASTVALVE2,
  UT_BLASTVALVE3,
  UT_BLASTVALVE4,
  UT_MAX
} unit_t;

class Sensor {
  private:
    sensorMode_t _mode;
    format_t _format;
    sensorType_t _type;
    uint32_t _lastchange;

  public:
    virtual void print(Print &p, format_t format = FMT_TEXT) = 0;
    sensorMode_t mode() {
        return _mode;
    };
    format_t format() {
        return _format;
    };
    sensorType_t type() {
        return _type;
    };

};
typedef unordered_set<Sensor*> SensorSet;

class Unit {
  private:
    SensorSet _sensorset;
  public:
    const String &name() {};
    void print(Print &p, format_t format = FMT_TEXT) {};
    void add(Sensor *s) {
        _sensorset.insert(s);
    };
};

typedef unordered_set<Unit*> UnitSet;

class Equipment {
  private:
    UnitSet _unitset;
  public:

    void add(Unit *u) {
        _unitset.insert(u);
    };
    void render() {
        for(auto u: _unitset) {
            u->print(Serial);
        }
    };
};


class Ruuvi : public Sensor {
  public:
    void print(Print &p, format_t format = FMT_TEXT) {};
    void setOnUpdate(std::function<void(const char *value)> onUpdate, facette_t what ) {}

  private:
    ruuviAd_t value;
};

class Mopeka : public Sensor {
  public:
    void print(Print &p, format_t format = FMT_TEXT) {};

};

class TPMS : public  Sensor {
  public:
    void print(Print &p, format_t format = FMT_TEXT) {};

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
