#pragma once

#include <stdint.h>

#define NFC_MAX_MSG_SIZE 1024

typedef enum {
    BWTAG_NO_MATCH,
    BWTAG_RUUVI,
    BWTAG_RUUVI_OAT,
    BWTAG_RUUVI_ENV,
    BWTAG_TANK,
    BWTAG_BURNER,
    BWTAG_FLOWSENSOR,
    BWTAG_PRESSURESENSOR,
    BWTAG_BAROSENSOR,
} bwTagType_t;


