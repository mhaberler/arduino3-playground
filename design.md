Data Model Overview
===================

The Equipment is a bag of all items of interest, called Units.

Example Units are:

- a tank
- the envelope
- the basket
- the burner
- a GPS
- a barometer
- a magnetometer/compass
- an inertial sensor

A Unit is uniquely identified by a Unit id and has a particular type (envelope, basket, tank, burner). Units may have multiple instances (like for instance tanks). A typical Unit identifier could be the manufacturer serial number, or the aircraft callsign in case of the envelope.

A Unit contains zero or several Sensors. For example, the basket may have an Outside Air sensor for temperature and humidity, the burner might have one or more pressure, flow and blastvalve activation sensors.

The purpose of a Sensor is to report one or several aspects of a particular unit. For instance, the MopekaPro level sensor can report the fuel level in mm, and the temperature at the tank bottom.

A Sensor is of a particular type and model (for instance RuuviTag, MopekaPro, RotarexDimas, TPMS, UbloxGPS, DPS368 barometer sensor etc), and has an identifier unique within its type. 

In case of BLE-based sensors, the MAC address acts as the unique identifier (for instance "D8:C6:11:CA:12:55") . 

Sensors connected physically are identified by a bus coordinate (tuple of Interface Id and Device Address), for example "Serial:1" or "I2C2:0x78".

Sensor values can also be retrieved via an MQTT subscription. For this sensor, the id value is the MQTT URI, for example: "mqtt://mybroker.com/vehicle/tracker" .

Both a Unit, and a Sensor, are represented as a JSON object.
A Unit contains an array of zero or more Sensors.

An example of a Unit with no sensors might be the UT_AIRCRAFT Unit, which might just serve to represent fixed attributes like callsign and ICAO identifier.

Configuration Items
-------------------

The smallest configurable item is one or several Units.
Units are represented as Configuration items.


Configuration items may contain several Units represented as an array of JSON Objects. 

The trivial case of a single Unit is represented as an array of a single JSON object.

The trivial case of a singleton Unit (for instance the envelope temperature sensor) is represented as a Unit containing a single Sensor.

Unit, Sensor and Value Aspect Types
-----------------------------------
````c
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
    AT_MAX
} actorType_t;

// Sensor reporting aspect
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
````

Tank: Unit and Representation example
-------------------------------------
````json
[
    {
        "id": "5020/16",				// a Unit's unique ID, type string
        "ut": 1,						// Unit type; UT_TANK, see unit_t
        "dsc": "Kubicek King Size",	// descriptive text for UI porposes
        "cap": 82.5,					// capacity in liters
        "tar": 24.5,					// tara 
        "full": 65,					// gross weight, full
        "hgt": 1137,					// unit height
        "col": "#DD6D1D",				// tag color for easy Unit identification and UI
        "rem": "oranges Tag",			// tag text
        "sensors": [					// list of sensors observing this unit
            {
                "st": 2,				// AT_MOPEKA, see actorType_t
                "mac": "D8:C6:11:CA:12:55",	// unique id is the MAC address of this sensor
                "min": 100,			// fuel level for empty tank, to compute usage in %
                "max": 857				// fuel level for full tank
            },
            {
                "st": 3,				// AT_TPMS, fuel pressure 
                "mac": "80:EA:CA:11:79:6F"  // unique id is the MAC address of this sensor
            }
        ]
    }
]
````

Aircraft: Unit and Representation example
-----------------------------------------
NB: this unit lacks a sensor list since it is only a static descriptor.
````json
[
    {
        "id": "OE-SOX",				// callsign 
        "ut": 6,						// UT_AIRCRAFT 
        "dsc": "Haberler's Hot Air Balloon",	// descriptive text for UI porposes
        "icao": "4409C8",				// ICAO Mode-S identifier
    }
]
````


Tracker: Unit and MQTT Sensor example
-----------------------------------------
````json
[
    {
        "id": "OE-SOX Tracker",		// id 
        "ut": 6,						// UT_VEHICLE
        "dsc": "Haberler's Landcruiser",	// descriptive text for UI porposes
        "sensors": [					// list of sensors observing this unit
            {
                "st": 9,				// AT_MQTT_SUBSCRIPTION, see actorType_t
                "id": "mqtt://mybroker.com/vehicle/tracker", // unique id is the MQTT subscription
            }
        ]
    }
]
````

Storing Units in a File System
------------------------------
Once a tag or sensor is read successfully, the user has the option to Save (really Save and Apply), or Cancel the operation.

The 'id' unique key of a Unit can be a fairly arbitrary string which does not necessarily result in a valid basename for a particular filesystem.

Therefore, on Save the 'id' unique key is sanitized into a legit basename by the following code fragment:

````c
    // Replace invalid characters with underscores
    for (char &c : sanitizedPath) {
        if (c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|') {
            c = '_';
        }
    }

    // Trim leading and trailing spaces
    sanitizedPath.trim();
````
The pathname shall be `"/equipment/<basename>.json"`, for example:

````
'5020/16' -> /equipment/5020_16.json
'5020/24' -> /equipment/5020_24.json
'env' -> /equipment/env.json
'oair' -> /equipment/oair.json

````

Since the 'id' unique key is stored in the Unit object, the basename has no meaning except for being unique.

Restoring Units from a File System
----------------------------------

On startup, all files matching `"/equipment/*.json"` are read, parsed and added as Units.

Clearing the Configuration
--------------------------

This is done by removing all files from `"/equipment"`, possibly followed by a restart.

Relation to NFC usage
---------------------

Configuration items can be stored in, and read from a filesystem at startup.

Since writing Configuration items to a flash-based or SD-card based filesystem is tedious, an alternative is to store  Configuration items on NFC tags, or use the NFC capability of certain sensors like RuuviTags for NFC-based configuration.

For this purpose, the Flight Display has an RFID reader which can be used to read NFC tags or NFC-capable sensors.

Since some sensors are mounted in hard-to-access locations or not be NFC-capable like most sensors, a Proxy Tag can be attached in an easily accessible location. The Proxy Tag contains a Configuration item representing one or several units.

To completely reconfigure a Flight Display, the interaction flow would be as follows:
- erase any stored configuration.
- scan a Proxy tag conveniently located in the basket. This might contain Aircraft, Envelope and Outside Air Unit configurations.
- scan a Proxy tag fixed on each tank near the top. This would configure the set of tanks, ordered in sequence of scanning. 

Proxy Tags may be colored, and have color and text information stored. This can be used in the User Interface to make an easy connection between say a tank gauge and a particular tank.

While RuuviTags can be NFC-scanned as well, scanning a Proxy Tag in a more conveniently accessible location might be preferrable.

Link Unit to Manufacturer Site
------------------------------

Since NFC tags can store (among others) a HTTP URI, an obvious add-on is to store a link to the manufacturer's site, passing the serial number as a paramter. 
This could be used to link a device to the manufacturer's release history, recalls, field change orders etc.

All that is needed is holding a mobile close to the tag, and confirm opening the link.