#pragma once

#include <stdint.h>
#include "freertos/queue.h"

typedef struct {
    uint32_t key;
    void *user_data;
} nfcMessage_t;

extern QueueHandle_t nfc_queue;
constexpr int nfc_queue_len = 5;
