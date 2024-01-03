#pragma once

#include <stdint.h>
#include "freertos/queue.h"

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

extern uint32_t 
    BW_EVENT_TAG_NO_MATCH,
    BW_EVENT_RUUVI,
    BW_EVENT_RUUVI_OAT,
    BW_EVENT_RUUVI_ENV,
    BW_EVENT_TANK,
    BW_EVENT_BURNER,
    BW_EVENT_FLOWSENSOR,
    BW_EVENT_PRESSURESENSOR,
    BW_EVENT_BAROSENSOR;

typedef struct {
    uint32_t key;
    void *user_data;
} nfcMessage_t;

extern QueueHandle_t nfc_queue;
constexpr int nfc_queue_len = 5;
