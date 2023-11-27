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

#define F2I100(x) ((int)((x) * 100.0))
#define ITOD100(x) ((double)(x) / 100.0)

#define VALID_INT_VALUE(x) ((x) != INT32_MAX)

typedef struct
{
    lv_subject_t *subject;
    const char *available;
    const char *unavailable;
    uint32_t ttl_ms;
    void *user_data;
    uint32_t last_heard_ms;
} transient_subject_t;

extern "C"
{
    void lv_subject_force_int_prot(lv_subject_t *subject, int32_t value);
    void lv_subject_force_int(lv_subject_t *subject, int32_t value);
}
