/*
During this session, only read or modify these paths:
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\main.c
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\user_app\*

Do not read, search, or touch any other files in the project.
*/


/*
don't remove this blok of comment.
it's the screen's layout.

+-------------------------------------------------------------------+ Y=0

|  SETTINGS MANAGER                                                 | Y=24
+-------------------------------------------------------------------+

| ▲ scroll up                                                       |
| 🔊 [1. Buzzer Toggle  ]               [ ON       ]                | Y=60
| 💡 [2. Backlight      ]               [ ██████░░ ] 75%            | Y=90
| 🌡️ [3. Temp Unit      ]               [ °C       ]                | Y=120 (Window View)
| ⏱️ [4. Auto-Lock Timer]               [ 30 Sec   ]                | Y=150
| ▼ scroll down (5. Calibration, 6. Sleep Time, 7. Sleep Temp... )  |
+-------------------------------------------------------------------+

|    +-------------------------+     +-------------------------+    |
|    |        ❌ CANCEL        |     |          ✅ OK          |    | Y=230
|    +-------------------------+     +-------------------------+    |
+-------------------------------------------------------------------+ Y=240
X=0                       X=160   X=170                         X=320


*/
#include "ui_settings.h"
#include "ui_manager.h"
#include "lvgl/lvgl.h"
#include "hardware_controls.h"
#include <stdio.h>

static lv_obj_t * buzzer_sw;
static lv_obj_t * backlight_slider;
static lv_obj_t * backlight_value_lbl;
static lv_obj_t * temp_dd;
static lv_obj_t * autolock_spin;
static lv_obj_t * btn_cancel;
static lv_obj_t * btn_ok;
static lv_obj_t * calibration_spin;
static lv_obj_t * sleep_time_spin;
static lv_obj_t * sleep_temp_spin;
static lv_obj_t * keylock_sw;

static void buzzer_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    bool on = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
    ui_data.buzzer_enabled = on;
}

static void backlight_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    lv_obj_t * s = lv_event_get_target(e);
    int v = lv_slider_get_value(s);
    ui_data.backlight_brightness = (uint32_t)v;
    if(backlight_value_lbl) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d%%", v);
        lv_label_set_text(backlight_value_lbl, buf);
    }
}

static void temp_dd_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    lv_obj_t * dd = lv_event_get_target(e);
    uint16_t sel = lv_dropdown_get_selected(dd);
    ui_data.temp_unit_is_fahrenheit = (sel == 1);
}

static void autolock_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    lv_obj_t * sp = lv_event_get_target(e);
    int v = lv_spinbox_get_value(sp);
    ui_data.auto_lock_timer_sec = (uint32_t)v;
}

static void calibration_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    ui_data.calibration_offset = (float)lv_spinbox_get_value(lv_event_get_target(e));
}

static void sleep_time_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    ui_data.sleep_time_min = (uint32_t)lv_spinbox_get_value(lv_event_get_target(e));
}

static void sleep_temp_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    ui_data.sleep_temp = (float)lv_spinbox_get_value(lv_event_get_target(e));
}

static void keylock_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    bool on = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
    ui_data.key_locker_enabled = on;
}

static void focus_scroll_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_FOCUSED) return;
    lv_obj_scroll_to_view(lv_event_get_target(e), LV_ANIM_OFF);
}

static void cancel_cb(lv_event_t * e);
static void ok_cb(lv_event_t * e);

static lv_obj_t * ui_settings_create_row(lv_obj_t * parent)
{
    lv_obj_t * row = lv_obj_create(parent);
    lv_obj_set_size(row, lv_pct(100), 24);
    lv_obj_set_scrollbar_mode(row, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_add_style(row, &ui_style_menu_row, 0);
    /* Layout: place label at left and control at right with space between */
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    return row;
}

static lv_obj_t * ui_settings_create_spacer(lv_obj_t * parent, lv_coord_t h)
{
    lv_obj_t * spacer = lv_obj_create(parent);
    lv_obj_set_size(spacer, lv_pct(100), h);
    lv_obj_set_style_bg_opa(spacer, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(spacer, 0, 0);
    lv_obj_set_style_border_opa(spacer, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(spacer, 0, 0);
    lv_obj_clear_flag(spacer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    return spacer;
}

static void ui_settings_create_action_row(void)
{
    lv_obj_t * action = lv_obj_create(scr_settings);
    lv_obj_set_size(action, lv_pct(100), 32);
    lv_obj_clear_flag(action, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(action, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_pad_all(action, 0, 0);
    lv_obj_set_flex_flow(action, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(action, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    btn_cancel = lv_btn_create(action);
        lv_obj_set_size(btn_cancel, 72, 16);
    lv_obj_add_event_cb(btn_cancel, cancel_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * t_cancel = lv_label_create(btn_cancel); lv_label_set_text(t_cancel, "CANCEL"); lv_obj_center(t_cancel);

    btn_ok = lv_btn_create(action);
        lv_obj_set_size(btn_ok, 72, 16);
    lv_obj_add_event_cb(btn_ok, ok_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * t_ok = lv_label_create(btn_ok); lv_label_set_text(t_ok, "OK"); lv_obj_center(t_ok);
}

static void cancel_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_CLICKED) return;
    ui_navigate_to(UI_SCREEN_MAIN);
}

static void ok_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_CLICKED) return;
    /* Values are live-updated; simply return to main */
    ui_navigate_to(UI_SCREEN_MAIN);
}

void ui_settings_init(void)
{
    scr_settings = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(scr_settings, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t * header = lv_label_create(scr_settings);
    lv_label_set_text(header, "SETTINGS MANAGER");
    lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 8);

    /* Content container (vertical) */
    lv_obj_t * cont = lv_obj_create(scr_settings);
    lv_obj_set_size(cont, lv_pct(100), lv_pct(58));
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 34);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_add_style(cont, &ui_style_menu_container, 0);

    /* 1. Buzzer Toggle */
    lv_obj_t * row1 = ui_settings_create_row(cont);
    lv_obj_t * lbl1 = lv_label_create(row1);
    lv_label_set_text(lbl1, "Buzzer");
    buzzer_sw = lv_checkbox_create(row1);
    /* initialize buzzer state from model */
    if(ui_data.buzzer_enabled) lv_obj_add_state(buzzer_sw, LV_STATE_CHECKED);
    else lv_obj_clear_state(buzzer_sw, LV_STATE_CHECKED);
    lv_obj_add_event_cb(buzzer_sw, buzzer_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(buzzer_sw, focus_scroll_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_set_width(buzzer_sw, 40);
        lv_obj_set_style_pad_left(lbl1, 4, 0);
        /* use margin (external spacing) not padding (internal) to move object away from edge */
        lv_obj_set_style_margin_right(buzzer_sw, 4, 0);


    /* 2. Backlight slider */
    lv_obj_t * row2 = ui_settings_create_row(cont);
    lv_obj_t * lbl2 = lv_label_create(row2);
    lv_label_set_text(lbl2, "Backlight");
    backlight_slider = lv_slider_create(row2);
    lv_obj_add_event_cb(backlight_slider, backlight_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(backlight_slider, focus_scroll_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_set_width(backlight_slider, 120);
    lv_slider_set_range(backlight_slider, 0, 100);
    lv_slider_set_value(backlight_slider, (int)ui_data.backlight_brightness, LV_ANIM_OFF);
    backlight_value_lbl = lv_label_create(row2);
    char buf[8]; snprintf(buf, sizeof(buf), "%d%%", (int)ui_data.backlight_brightness); lv_label_set_text(backlight_value_lbl, buf);

    /* 3. Temp unit dropdown */
    lv_obj_t * row3 = ui_settings_create_row(cont);
    lv_obj_t * lbl3 = lv_label_create(row3);
    lv_label_set_text(lbl3, "Temp Unit");
    temp_dd = lv_dropdown_create(row3);
    lv_dropdown_set_options_static(temp_dd, "C\nF");
    lv_obj_add_event_cb(temp_dd, temp_dd_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(temp_dd, focus_scroll_cb, LV_EVENT_FOCUSED, NULL);
    lv_dropdown_set_selected(temp_dd, ui_data.temp_unit_is_fahrenheit ? 1 : 0);

    /* 4. Auto-lock spinbox */
    lv_obj_t * row4 = ui_settings_create_row(cont);
    lv_obj_t * lbl4 = lv_label_create(row4);
    lv_label_set_text(lbl4, "Auto-Lock (s)");
    autolock_spin = lv_spinbox_create(row4);
    lv_spinbox_set_range(autolock_spin, 0, 600);
    lv_spinbox_set_digit_format(autolock_spin, 3, 0);
    lv_spinbox_set_value(autolock_spin, (int)ui_data.auto_lock_timer_sec);
    lv_obj_set_width(autolock_spin, 56);
    lv_obj_add_event_cb(autolock_spin, autolock_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(autolock_spin, focus_scroll_cb, LV_EVENT_FOCUSED, NULL);

    /* 5. Calibration (spinbox -100..100) */
    lv_obj_t * row5 = ui_settings_create_row(cont);
    lv_obj_t * lbl5 = lv_label_create(row5);
    lv_label_set_text(lbl5, "Calibration");
    calibration_spin = lv_spinbox_create(row5);
    lv_spinbox_set_range(calibration_spin, -100, 100);
    lv_spinbox_set_digit_format(calibration_spin, 4, 0);
    lv_spinbox_set_value(calibration_spin, (int)ui_data.calibration_offset);
    lv_obj_set_width(calibration_spin, 56);
    lv_obj_add_event_cb(calibration_spin, calibration_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(calibration_spin, focus_scroll_cb, LV_EVENT_FOCUSED, NULL);

    /* 6. Sleep Time (min) */
    lv_obj_t * row6 = ui_settings_create_row(cont);
    lv_obj_t * lbl6 = lv_label_create(row6); lv_label_set_text(lbl6, "Sleep Time (min)");
    sleep_time_spin = lv_spinbox_create(row6); lv_spinbox_set_range(sleep_time_spin, 0, 240); lv_spinbox_set_digit_format(sleep_time_spin, 3, 0); lv_spinbox_set_value(sleep_time_spin, (int)ui_data.sleep_time_min); lv_obj_set_width(sleep_time_spin, 56); lv_obj_add_event_cb(sleep_time_spin, sleep_time_cb, LV_EVENT_VALUE_CHANGED, NULL); lv_obj_add_event_cb(sleep_time_spin, focus_scroll_cb, LV_EVENT_FOCUSED, NULL);

    /* 7. Sleep Temp */
    lv_obj_t * row7 = ui_settings_create_row(cont);
    lv_obj_t * lbl7 = lv_label_create(row7); lv_label_set_text(lbl7, "Sleep Temp");
    sleep_temp_spin = lv_spinbox_create(row7); lv_spinbox_set_range(sleep_temp_spin, 0, 450); lv_spinbox_set_digit_format(sleep_temp_spin, 3, 0); lv_spinbox_set_value(sleep_temp_spin, (int)ui_data.sleep_temp); lv_obj_set_width(sleep_temp_spin, 56); lv_obj_add_event_cb(sleep_temp_spin, sleep_temp_cb, LV_EVENT_VALUE_CHANGED, NULL); lv_obj_add_event_cb(sleep_temp_spin, focus_scroll_cb, LV_EVENT_FOCUSED, NULL);

    /* 8. Key Lock (checkbox to save space) */
    lv_obj_t * row8 = ui_settings_create_row(cont);
    lv_obj_t * lbl8 = lv_label_create(row8); lv_label_set_text(lbl8, "Key Lock");
    keylock_sw = lv_checkbox_create(row8);
    /* initialize key lock state from model */
    if (ui_data.key_locker_enabled) lv_obj_add_state(keylock_sw, LV_STATE_CHECKED);
    else lv_obj_clear_state(keylock_sw, LV_STATE_CHECKED);
    lv_obj_add_event_cb(keylock_sw, keylock_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(keylock_sw, focus_scroll_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_set_width(keylock_sw, 40);
    /* use margin (external spacing) not padding (internal) to move object away from edge */
    lv_obj_set_style_margin_right(keylock_sw, 4, 0);

    /* invisible spacer at the end to allow the bottom item to scroll into view */
    ui_settings_create_spacer(cont, 16);

    /* Action row */
    ui_settings_create_action_row();
}

void ui_settings_focused(void)
{
    if (ui_encoder_group == NULL) return;
    lv_group_remove_all_objs(ui_encoder_group);
    if (buzzer_sw) lv_group_add_obj(ui_encoder_group, buzzer_sw);
    if (backlight_slider) lv_group_add_obj(ui_encoder_group, backlight_slider);
    if (temp_dd) lv_group_add_obj(ui_encoder_group, temp_dd);
    if (autolock_spin) lv_group_add_obj(ui_encoder_group, autolock_spin);
    if (calibration_spin) lv_group_add_obj(ui_encoder_group, calibration_spin);
    if (sleep_time_spin) lv_group_add_obj(ui_encoder_group, sleep_time_spin);
    if (sleep_temp_spin) lv_group_add_obj(ui_encoder_group, sleep_temp_spin);
    if (keylock_sw) lv_group_add_obj(ui_encoder_group, keylock_sw);
    if (btn_cancel) lv_group_add_obj(ui_encoder_group, btn_cancel);
    if (btn_ok) lv_group_add_obj(ui_encoder_group, btn_ok);

    /* Start focus on the first visible item so encoder navigation works predictably */
    if (buzzer_sw) lv_group_focus_obj(buzzer_sw);
}
