/* Minimal hardware controls header for simulator shims */

#ifndef HARDWARE_CONTROLS_H
#define HARDWARE_CONTROLS_H

#include "lvgl/lvgl.h"

/* Turn a switch object to ON/OFF. The animator argument is ignored by the shim.
 * These mirror LVGL helper names used in the original codebase. */
void lv_switch_on(lv_obj_t * sw, lv_anim_enable_t anim);
void lv_switch_off(lv_obj_t * sw, lv_anim_enable_t anim);

#endif /* HARDWARE_CONTROLS_H */
