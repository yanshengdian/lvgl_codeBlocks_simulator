/*
During this session, only read or modify these paths:
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\main.c
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\user_app\*

Do not read, search, or touch any other files in the project.

*/


#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "lvgl/lvgl.h"

/* -------------------------------------------------------------------------
 * 1. ENUMERATIONS: Application Screen States
 * ------------------------------------------------------------------------- */
typedef enum {
    UI_SCREEN_POWER_UP,
    UI_SCREEN_MAIN,
    UI_SCREEN_SETTINGS,
    UI_SCREEN_CURVES
} ui_screen_t;

/* -------------------------------------------------------------------------
 * 2. STRUCTURES: Central System Data Synchronization Model
 * ------------------------------------------------------------------------- */
typedef struct {
    /* Real-Time Measurements */
    float current_temp;
    float current_voltage;
    float current_current;
    float current_power;
    uint32_t power_output_percent;

    /* Adjustable Configurations */
    float setting_temp;
    bool buzzer_enabled;
    uint32_t backlight_brightness;
    bool temp_unit_is_fahrenheit; /* false = °C, true = °F */
    uint32_t auto_lock_timer_sec;
    float calibration_offset;
    uint32_t sleep_time_min;
    float sleep_temp;
    bool key_locker_enabled;
} ui_system_data_t;

/* -------------------------------------------------------------------------
 * 3. GLOBAL VARIABLES (Shared Interfaces)
 * ------------------------------------------------------------------------- */
extern lv_obj_t * scr_power_up;
extern lv_obj_t * scr_main;
extern lv_obj_t * scr_settings;
extern lv_obj_t * scr_curves;
extern lv_group_t * ui_encoder_group;
extern ui_system_data_t ui_data;

/* -------------------------------------------------------------------------
 * 4. PUBLIC INTERFACE FUNCTIONS
 * ------------------------------------------------------------------------- */
void ui_init(void);
void ui_navigate_to(ui_screen_t target_screen);

#endif /* UI_MANAGER_H */
