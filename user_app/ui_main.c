/*
During this session, only read or modify these paths:
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\main.c
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\user_app\*

Do not read, search, or touch any other files in the project.
*/

#include "ui_main.h"
#include "ui_manager.h"
#include "lvgl/lvgl.h"

static lv_obj_t * settings_btn;
static lv_obj_t * curves_btn;

static void settings_button_cb(lv_event_t * e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        ui_navigate_to(UI_SCREEN_SETTINGS);
    }
}

static void curves_button_cb(lv_event_t * e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        ui_navigate_to(UI_SCREEN_CURVES);
    }
}

void ui_main_init(void)
{
    scr_main = lv_obj_create(NULL);

    lv_obj_t * title_label = lv_label_create(scr_main);
    lv_label_set_text(title_label, "Main Screen");
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 18);

    lv_obj_t * status_label = lv_label_create(scr_main);
    lv_label_set_text(status_label, "Ready");
    lv_obj_align(status_label, LV_ALIGN_CENTER, 0, 0);

    settings_btn = lv_btn_create(scr_main);
    lv_obj_set_size(settings_btn, 120, 40);
    lv_obj_align(settings_btn, LV_ALIGN_BOTTOM_LEFT, 24, -24);
    lv_obj_add_event_cb(settings_btn, settings_button_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * settings_label = lv_label_create(settings_btn);
    lv_label_set_text(settings_label, "Settings");
    lv_obj_center(settings_label);

    curves_btn = lv_btn_create(scr_main);
    lv_obj_set_size(curves_btn, 120, 40);
    lv_obj_align(curves_btn, LV_ALIGN_BOTTOM_RIGHT, -24, -24);
    lv_obj_add_event_cb(curves_btn, curves_button_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * curves_label = lv_label_create(curves_btn);
    lv_label_set_text(curves_label, "Curves");
    lv_obj_center(curves_label);
}

void ui_main_focused(void)
{
    if (ui_encoder_group == NULL) return;
    lv_group_remove_all_objs(ui_encoder_group);
    lv_group_add_obj(ui_encoder_group, settings_btn);
    lv_group_add_obj(ui_encoder_group, curves_btn);
}
