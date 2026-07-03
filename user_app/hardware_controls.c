/*
 * Hardware control shims for missing switch helpers.
 * These provide minimal implementations used by the simulator
 * to set/clear the `LV_STATE_CHECKED` state on a switch object.
 */

#include "lvgl/lvgl.h"

void lv_switch_on(lv_obj_t * sw, lv_anim_enable_t anim)
{
    (void)anim;
    if(sw) lv_obj_add_state(sw, LV_STATE_CHECKED);
}

void lv_switch_off(lv_obj_t * sw, lv_anim_enable_t anim)
{
    (void)anim;
    if(sw) lv_obj_clear_state(sw, LV_STATE_CHECKED);
}
