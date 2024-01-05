#pragma once

#include "ArduinoJson.h"

extern bool psRAMavail;

struct SpiRamAllocator {
    void *allocate(size_t size) {
        if (psRAMavail)
            return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
        else
            return heap_caps_malloc(size, MALLOC_CAP_DEFAULT);
    }
    void deallocate(void *pointer) {
        heap_caps_free(pointer);
    }
};
using SpiRamJsonDocument = BasicJsonDocument<SpiRamAllocator>;
