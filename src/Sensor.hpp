#pragma once

#include "Arduino.h"
#include "ArduinoJson.h"
#include "ArduinoJsonCustom.h"
#include <FS.h>
#include <list>
#include <set>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "NimBLEAddress.h"
#include "blescan.hpp"
#include "esp_log.h"
#include "ruuvi.h"
#include "mopeka.h"
#include "tpms.h"
#include "lv_observer.h"
#include <Functor.h>

#define EQUIPMENT_DIR "/equipment"
#define BINDING_DIR "/binding"

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
    AT_GPS,
    AT_FLOWSENSOR,
    AT_BARO,
    AT_IMU,
    AT_MAGNETOMETER,
    AT_MQTT_SUBSCRIPTION,
    AT_UI,
    AT_LOG,
    AT_MAX
} actorType_t;

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

const char *sensorTypeStr(const actorType_t sensorType);
const char *unitTypeStr(const unit_t unitType);

void convertFromJson(JsonVariantConst src, actorType_t& dst);
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

typedef unordered_set<const std::string> StringTable_t;

class Unit;
class Equipment;
class Consumer;
class Producer;

class Binding {
  private:
  public:
    bool link(Producer &p, Consumer &c);
    bool link(Unit &src, Consumer &c);
    bool link(Unit &src, Unit &dst);
    bool link(actorType_t &src, Unit &);
    bool link(actorType_t &src, actorType_t &to);
};

class Actor { // abstract base class of Sensor, Actuator
  private:
  protected:
    actorType_t _type;
    Unit *_unit;
    uint32_t _created; // timestamp

  public:
    virtual bool configure(JsonObject conf) = 0;
    virtual void dump(Stream &s)  = 0;
    virtual const std::string id(void) = 0;
    virtual const NimBLEAddress & mac() {
        return null_mac;
    }
};

class Producer : public Actor {

};

class Consumer : public Actor {

};

// class Consumer {
//   private:
//     std::string _id;
//     unit_t _ut;
//     lv_subject_t *_subject;
//     fs::File _f;
//     std::string _uri;

//   public:
//     Consumer(const std::string& id, lv_subject_t *subject) : _id(id),_ut(UT_OBSERVER),_subject(subject) {};
//     Consumer(const std::string& id, fs::File &f) : _id(id),_ut(UT_WRITER),_f(f) {};
//     Consumer(const std::string& id, const std::string& uri) : _id(id),_ut(UT_URI) {};

//     bool configure(JsonObject *conf);

// };

class Sensor : public Producer {
  private:
    // sensorMode_t _mode;
    format_t _format;

  public:
    Sensor(Unit *u)  {
        _created = millis();
        _unit = u;
    };

    uint32_t created(void) {
        return _created;
    };
    virtual uint32_t lastChange(void) = 0;
    virtual void print(Print &p, format_t format = FMT_TEXT) = 0;
    // sensorMode_t mode();
    actorType_t type();
    format_t format();
    void dump(Stream &s) override;

    bool configure(JsonObject conf);
    virtual bool bleAdvertisement(const bleAdvMsg_t  &msg) = 0;
    virtual const  std::string name(void) = 0;
    virtual const  std::string fullName(void) = 0;
    virtual  const std::string id(void) = 0;
    const  std::string unitName(void);
};

class BLESensor : public Sensor {
  protected:
    NimBLEAddress _macAddress;
  public:
    BLESensor(Unit *u) : Sensor(u) {};
    bool configure(JsonObject conf);
    const std::string id(void);
    const NimBLEAddress & mac();
};

class PollingSensor : public Sensor {  // I2C etc

};

class ActiveSensor : public Sensor {  // GPS, URI etc - no polling needed

};

class Unit {
  private:
    std::string _id;
    unit_t _ut;
    uint32_t _created; // timestamp
    uint8_t _index;
    std::unordered_map< std::string, Actor *> _actor_map;

  public:
    Unit( std::string id) : _id(id),_created(millis()) {};

    // dtor: delete sensors!!
    //
    bool configure(Equipment &eq, JsonObject *conf);
    uint32_t created(void) {
        return _created;
    };
    void print(Print &p, format_t format = FMT_TEXT);
    void dump(Stream &s);
    void setType(const unit_t ut) {
        _ut = ut;
    };
    unit_t type(void) {
        return _ut;
    };
    const  std::string id(void) {
        return std::string(unitTypeStr(_ut)) + ":" + _id;
    };
};

#define UV_TANKS_ONLY BIT(0)

typedef Functor3wRet<Unit &, uint32_t, void *, bool> UnitVisitor;

// class Consumer : public Unit {
//   private:
//     lv_subject_t *_subject;
//     fs::File _f;
//     std::string _uri;
// };

struct cmp_unit_age {
    bool operator() (Unit *a, Unit *b) const {
        return a->created() < b->created();
    }
};

class Equipment {
  private:
    unordered_map<std::string, Unit *> _units;
    unordered_map<NimBLEAddress, Sensor *> _ble_sensors;
    std::set<Unit *, cmp_unit_age> _units_by_age;
    const char *_topdir;
    bool _saveUnit(const std::string &id, const JsonArray &array);

  public:
    // Equipment(const char *topdir) : _topdir(topdir) {};
    void read(const char* dirname);
    bool addUnit(const char *path);
    bool addUnit(JsonObject conf, bool save = true);
    void dump(Stream &s);
    bool bleDeliver(const bleAdvMsg_t &msg);
    bool bleRegister(const NimBLEAddress &mac, Sensor *sp);
    // bool addConsumer(const std::string &id, lv_subject_t *subject);

    void walk(const UnitVisitor &unitVisitor, const uint32_t flags, void *user_data);
};

class Ruuvi : public BLESensor {
  private:
    ruuviAd_t _ruuvi_report;
  public:
    Ruuvi(Unit *u) : BLESensor(u)  {
        _type = AT_RUUVI;
    };
    void print(Print &p, format_t format = FMT_TEXT);
    void setOnUpdate(std::function<void(const char *value)> onUpdate, aspect_t what ) {}
    bool configure(JsonObject conf) override;
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

class Mopeka : public BLESensor {
  private:
    mopekaAd_t _mopeka_report;
    uint16_t _min_mm = 100;
    uint16_t _max_mm = 857;
    bool _decode(const uint8_t *data,
                 const size_t len, mopekaAd_t &ma);
  public:
    Mopeka(Unit *u) : BLESensor(u) {
        _type = AT_MOPEKA;
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

class TPMS : public  BLESensor {
  private:
    tpmsAd_t _tpms_report;

  public:
    TPMS(Unit *u) : BLESensor(u) {
        _type = AT_TPMS;
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
