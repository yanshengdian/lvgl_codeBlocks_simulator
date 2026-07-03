/*
During this session, only read or modify these paths:
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\main.c
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\user_app\*

Do not read, search, or touch any other files in the project.
*/

#include "ui_curves.h"
#include "ui_manager.h"

void ui_curves_init(void)
{
    scr_curves = lv_obj_create(NULL);
}

void ui_curves_focused(void)
{
    lv_group_remove_all_objs(ui_encoder_group);
}
