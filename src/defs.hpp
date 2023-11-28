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
#define NOT_AVAILABLE "n/a"
