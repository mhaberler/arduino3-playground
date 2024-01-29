#pragma once

#include <FS.h>

typedef enum {
  VA_PRINT = (1u << 0),
  VA_DEBUG = (1u << 1),
  VA_RECURSIVE = (1u << 2),
  VA_ERASE_FILES = (1u << 4),
  VA_LOAD_CONFIG  = (1u << 8),
} visitorAction_t;

void fsVisitor(fs::FS &fs, Stream &fmt, const char *topdir = "/",
               uint32_t flags = 0, uint8_t levels = 10);

               
