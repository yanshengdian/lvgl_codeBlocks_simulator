#ifndef UI_POWER_UP_H
#define UI_POWER_UP_H

#include "lvgl.h"

// Tell the compiler that the logo descriptor exists in another file (logo.c)
extern const lv_image_dsc_t logo; 

// Your existing power-up function declarations
void ui_power_up_init(void);

#endif // UI_POWER_UP_H
