#pragma once
#if defined(LVGL_DEMO)

#include "lv_conf.h"
#include <lvgl.h>
#include "lv_observer.h"

#endif

#ifdef M5UNIFIED
#include <M5Unified.h>
#else
#include <Arduino.h>
#endif

#define F2I100(x) ((int)(((double)x) * 100.0))
#define ITOD100(x) ((double)(x) / 100.0)

typedef struct
{
    lv_subject_t *subject;
    const char *available;
    const char *unavailable;
    uint32_t ttl_ms;
    void *user_data;
    uint32_t last_heard_ms;
} transient_subject_t;
