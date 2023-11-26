#pragma once

#ifdef M5UNIFIED
#include <M5Unified.h>

#if defined(LVGL_DEMO)
#include <lvgl.h>
#endif
#else
#include <Arduino.h>
#endif

#define F2I100(x) ((int)((x) * 100.0))
#define ITOD100(x) ((double)(x) / 100.0)

#define VALID_INT_VALUE(x) ((x) != INT32_MAX)


typedef struct
{
    const char *available;
    const char *unavailable;
    void  *user_data;
} fmt_spec_t;