#ifndef _LVGL_SETUP_H
#define _LVGL_SETUP_H

#include <stdint.h>

void lv_begin(void);


#ifdef __cplusplus
extern "C"
{
#endif
    void lv_observer_register(void);
#ifdef __cplusplus
}
#endif
#endif