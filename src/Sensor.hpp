#pragma once

#include "Arduino.h"
#include "ArduinoJson.h"
#include "ArduinoJsonCustom.h"
#include <FS.h>
#include <list>

#include <vector>
#include <unordered_map>
#include <algorithm>

#include "NimBLEAddress.h"
#include "blescan.hpp"
#include "esp_log.h"
#include "ruuvi.h"
#include "mopeka.h"
#include "tpms.h"
#include "lv_observer.h"
#include <Functor.h>

#define CONFIG_DIR "/config"
#define UNITS_DIR "/config/units"
#define SEQUENCE_DIR "/config/sequence"

#define RUUVI_TIMEOUT 62 // Ruuvi: 60sec
#define MOPEKA_TIMEOUT 120
#define TPMS_TIMEOUT 120

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
    AT_NONE = 0,
    AT_RUUVI,
    AT_MOPEKA,
    AT_TPMS,
    AT_ROTAREX,
    AT_FLOWSENSOR,
    AT_GPS,
    AT_BARO,
    AT_IMU,
    AT_MAGNETOMETER,
    AT_MQTT_SUBSCRIPTION,
    AT_UI,
    AT_LOG,
    AT_MAX
} sensorType_t;

typedef enum {
    ASPT_NONE,
    ASPT_TEMPERATURE,
    ASPT_SENSOR_TEMPERATURE,
    ASPT_SENSOR_BATTERY_STATUS,
    ASPT_HUMIDITY,
    ASPT_FLOW,
    ASPT_BURNER_ON,
    ASPT_BURNER_OFF,
    ASPT_BURNER_DURATION,
    ASPT_PRESSURE,
    ASPT_VOLTAGE,
    ASPT_CURRENT,
    ASPT_LONGITUDE,
    ASPT_LATITUDE,
    ASPT_ALTITUDE,
    ASPT_HSPEED,
    ASPT_VSPEED,
    ASPT_VACCEL,
    ASPT_MAX
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
    UT_TANK_SEQUENCE,
    UT_OBSERVER,  // 7
    UT_WRITER,
    UT_URI,
    UT_BINDING, // 10
    UT_MAX
} unit_t;

extern  const NimBLEAddress null_mac;

String sanitizeLittleFSPath(const String &path);
bool wipeLittleFS(void);
uint8_t volt2percent(const float volt);
bool bleDeliver(const bleAdvMsg_t &msg);
float percentBetween(float min, float max, float value);
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
class Equipment;

class Sensor  {
  private:

    format_t _format;
    float _min = 0.0;
    float _max = 100.0;
  protected:
    sensorType_t _type;
    Unit *_unit;
  public:
    Sensor(Unit *u)  {
        _unit = u;
    };
    virtual const NimBLEAddress & mac() {
        return null_mac;
    }
    virtual const void *pod(void) = 0;
    virtual uint32_t lastChange(void) = 0;
    virtual void print(Print &p, format_t format = FMT_TEXT) = 0;

    sensorType_t type();
    Unit *unit(void);

    format_t format();
    void dump(Stream &s);

    void setMin(float m) {
        _min = m;
    }
    void setMax(float m) {
        _max = m;
    }
    const float min(void) {
        return _min;
    }
    const float max(void) {
        return _max;
    }
    virtual bool configure(JsonObject conf) = 0;
    virtual void report(void) = 0;
    virtual bool bleAdvertisement(const bleAdvMsg_t  &msg) = 0;
    virtual const  std::string name(void) = 0;
    virtual const  std::string fullName(void) = 0;
    virtual  const std::string id(void) = 0;
    const  std::string unitName(void);
};

class BLESensor : public Sensor {
  protected:
    NimBLEAddress _macAddress = {};
  public:
    BLESensor(Unit *u) : Sensor(u) {};
    bool configure(JsonObject conf);
    const std::string id(void);
    const NimBLEAddress & mac();
};

class Unit {
  private:
    std::string _id;
    unit_t _ut;
    int8_t _index;
    std::unordered_map< std::string, Sensor *> _sensor_map;
    uint32_t _timestamp;
    std::string _tagcolor;
    std::string _description;
    float _cap = 40.0; // liter

  public:
    Unit( std::string id) : _id(id), _index(-1) {};
    ~Unit() {
        Serial.printf("Unit.dtor %s\n", _id.c_str());
        // dtor: delete sensors!!
        //
    };
    void setTagColor(  std::string color) {
        _tagcolor = color;
    }
    const  std::string & tagColor(void) {
        return _tagcolor;
    }
    void setCap(float cap) {
        _cap = cap;
    }
    const float cap(void) {
        return _cap;
    }
    uint32_t timestamp(void) {
        return _timestamp;
    };
    void setTimestamp(uint32_t t) {
        _timestamp = t;
    };
    int8_t index(void) {
        return _index;
    };
    void setIndex(int8_t t) {
        _index = t;
    };
    bool configure(Equipment &eq, JsonObject *conf);
    void print(Print &p, format_t format = FMT_TEXT);
    void dump(Stream &s);
    void setType(const unit_t ut) {
        _ut = ut;
    };
    unit_t type(void) {
        return _ut;
    };
    const  std::string fullName(void) {
        return std::string(unitTypeStr(_ut)) + ":" + _id;
    };
    const  std::string &id(void) {
        return  _id;
    };
    void reportSensors(void);
};

#define UV_TANKS_ONLY BIT(0)

typedef Functor3wRet<Unit &, uint32_t, void *, bool> UnitVisitor;

struct cmp_unit_age {
    bool operator() (Unit *a, Unit *b) const {
        return a->timestamp() < b->timestamp();
    }
};

typedef enum {
    SRC_FLASHFS,
    SRC_NFC,
    SRC_SETUP
} source_t;

class Equipment {
  private:
    vector<Unit *> _units;
    vector<Unit *> _tanks;
    unordered_map<std::string, Unit *> _unit_by_id;
    unordered_map<NimBLEAddress, Sensor *> _ble_sensors;
    bool _saveUnit(const std::string &id, const JsonDocument &doc);
    bool _updateSequence(void);
    uint32_t _seq;

  public:
    void read(const char* dirname, uint32_t flags);
    bool addUnit(const char *path);
    bool addUnit(JsonObject conf, source_t source);
    void delUnit(const std::string &id);
    void reportSensors(void);
    bool restoreSequence(const char *path);
    void dump(Stream &s);
    bool bleDeliver(const bleAdvMsg_t &msg);
    bool bleRegister(const NimBLEAddress &mac, Sensor *sp);
    void walk(const UnitVisitor &unitVisitor, const uint32_t flags, void *user_data);
};

class Ruuvi : public BLESensor {
  private:
    ruuviAd_t _ruuvi_report = {};

  public:
    Ruuvi(Unit *u) : BLESensor(u)  {
        _type = AT_RUUVI;
    };
    void print(Print &p, format_t format = FMT_TEXT);
    void setOnUpdate(std::function<void(const char *value)> onUpdate, aspect_t what ) {}
    bool configure(JsonObject conf);
    void report(void);
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
    const void *pod(void) {
        return &_ruuvi_report;
    }
    uint32_t lastHeard(void) {
        return _ruuvi_report.lastchange;
    }

};

class Mopeka : public BLESensor {
  private:
    mopekaAd_t _mopeka_report = {};
    bool _decode(const uint8_t *data,
                 const size_t len, mopekaAd_t &ma);
  public:
    Mopeka(Unit *u) : BLESensor(u) {
        _type = AT_MOPEKA;
    };
    void print(Print &p, format_t format = FMT_TEXT);
    void setOnUpdate(std::function<void(const char *value)> onUpdate, aspect_t what ) {}
    bool configure(JsonObject conf);
    void report(void);
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
    const void *pod(void) {
        return &_mopeka_report;
    }
    uint32_t lastHeard(void) {
        return _mopeka_report.lastchange;
    }
};

class TPMS : public  BLESensor {
  private:
    tpmsAd_t _tpms_report = {};

  public:
    TPMS(Unit *u) : BLESensor(u) {
        _type = AT_TPMS;
    };
    void print(Print &p, format_t format = FMT_TEXT);
    void setOnUpdate(std::function<void(const char *value)> onUpdate, aspect_t what ) {}
    bool configure(JsonObject conf);
    void report(void);
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
    const void *pod(void) {
        return &_tpms_report;
    }
    uint32_t lastHeard(void) {
        return _tpms_report.lastchange;
    }
};

// class GPS : public Sensor {
//   public:

// };

// class Barometer : public Sensor {
//   public:

// };

// class IMU : public Sensor {
//   public:

// };

// class UIElement : public Consumer {

// };

// class Actuator : public Consumer {

// };

extern Equipment equipment;
