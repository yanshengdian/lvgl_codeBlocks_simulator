/*
During this session, only read or modify these paths:
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\main.c
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\user_app\*

Do not read, search, or touch any other files in the project.
*/

#include "ui_settings.h"
#include "ui_manager.h"

void ui_settings_init(void)
{
    scr_settings = lv_obj_create(NULL);
}

void ui_settings_focused(void)
{
    lv_group_remove_all_objs(ui_encoder_group);
}
