#pragma once

#include "ArduinoJson.h"

// // https://github.com/bblanchon/ArduinoJson/issues/1904#issuecomment-1498614172
// JsonVariant resolveJsonPath(JsonVariant variant, const char* path) {
//   for (size_t n = 0; path[n]; n++) {
//     if (path[n] == '.') {
//       variant = variant[JsonString(path, n)];
//       path += n + 1;
//       n = 0;
//     }
//   }
//   return variant[path];
// }

#if ARDUINOJSON_VERSION_MAJOR > 6
struct SpiRamAllocator : ArduinoJson::Allocator {
    void* allocate(size_t size) override {
        return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
    }

    void deallocate(void* pointer) override {
        heap_caps_free(pointer);
    }

    void* reallocate(void* ptr, size_t new_size) override {
        return heap_caps_realloc(ptr, new_size, MALLOC_CAP_SPIRAM);
    }
};
#else
struct SpiRamAllocator {
  void* allocate(size_t size) {
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
  }

  void deallocate(void* pointer) {
    heap_caps_free(pointer);
  }

  void* reallocate(void* ptr, size_t new_size) {
    return heap_caps_realloc(ptr, new_size, MALLOC_CAP_SPIRAM);
  }
};

using SpiRamJsonDocument = BasicJsonDocument<SpiRamAllocator>;

#endif
