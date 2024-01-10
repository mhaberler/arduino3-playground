#pragma once

#include "NimbleAddress.h"

#define BLE_ADV_QUEUELEN 10
#define BLE_ADV_SIZE 32 // increase to 255 for extended ads

// hash for NimBLEAddress containers
namespace std {
    template<>
    class hash<NimBLEAddress> {
    public:
        size_t operator()(NimBLEAddress const& c) const {
            uint64_t u64 = (uint64_t)c;
            size_t sz = (u64 >> 32) | u64;
            // Serial.printf("%s -> %lx %lu\n", c.toString().c_str(), sz, sz);
            return sz;
        }
    };
}

typedef struct  {
    uint64_t mac64;
    uint16_t mfid;
    int8_t rssi;
    uint8_t msg_size;
    uint8_t message[BLE_ADV_SIZE];
} bleAdvMsg_t;

extern  QueueHandle_t bleadv_queue;
