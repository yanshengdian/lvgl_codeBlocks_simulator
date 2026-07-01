#include "ui_manager.h"
#include "ui_power_up.h"
#include "ui_main.h"
#include "ui_settings.h"
#include "ui_curves.h"

/* -------------------------------------------------------------------------
 * 1. MEMORY ALLOCATION: Base Screens & System Instances
 * ------------------------------------------------------------------------- */
lv_obj_t * scr_power_up = NULL;
lv_obj_t * scr_main     = NULL;
lv_obj_t * scr_settings = NULL;
lv_obj_t * scr_curves   = NULL;

lv_group_t * ui_encoder_group = NULL;
ui_system_data_t ui_data;

/* -------------------------------------------------------------------------
 * 2. SUBROUTINE: Local Data Model Factory Defaults
 * ------------------------------------------------------------------------- */
static void ui_init_default_data(void)
{
    /* Initialize monitoring parameters with baseline mock data */
    ui_data.current_temp           = 25.0f;
    ui_data.current_voltage        = 24.0f;
    ui_data.current_current        = 0.0f;
    ui_data.current_power          = 0.0f;
    ui_data.power_output_percent   = 0;
    
    /* Operational Configuration Presets */
    ui_data.setting_temp           = 350.0f;
    ui_data.buzzer_enabled         = true;
    ui_data.backlight_brightness   = 80;
    ui_data.temp_unit_is_fahrenheit = false; /* Default to Celsius */
    ui_data.auto_lock_timer_sec    = 30;
    ui_data.calibration_offset     = 0.0f;
    ui_data.sleep_time_min         = 5;
    ui_data.sleep_temp             = 150.0f;
    ui_data.key_locker_enabled     = false;
}

/* -------------------------------------------------------------------------
 * 3. MAIN UI ROUTER ARCHITECTURE
 * ------------------------------------------------------------------------- */
void ui_init(void)
{
    /* Initialize default parameter states */
    ui_init_default_data();

    /* Allocate the global master encoder navigation group */
    ui_encoder_group = lv_group_create();
    
    /* Dynamically loop through simulator HAL drivers and bind the active input device */
    lv_indev_t * indev = lv_indev_get_next(NULL);
    while(indev) {
        lv_indev_type_t type = lv_indev_get_type(indev);
        if(type == LV_INDEV_TYPE_ENCODER || type == LV_INDEV_TYPE_KEYPAD) {
            lv_indev_set_group(indev, ui_encoder_group);
            break;
        }
        indev = lv_indev_get_next(indev);
    }
    
    /* Pre-compile layout components into background system memory */
    ui_power_up_init();
    ui_main_init();
    ui_settings_init();
    ui_curves_init();
    
    /* Launch system straight into the animated entry screen */
    ui_navigate_to(UI_SCREEN_POWER_UP);
}

void ui_navigate_to(ui_screen_t target_screen)
{
    /* Instantly strip old selection pointers out of the encoder tracker loop */
    lv_group_remove_all_objs(ui_encoder_group);
    
    /* Route execution matrix context and hand tracking over to target files */
    switch(target_screen) {
        case UI_SCREEN_POWER_UP:
            lv_screen_load(scr_power_up);
            ui_power_up_focused();
            break;
            
        case UI_SCREEN_MAIN:
            lv_screen_load(scr_main);
            ui_main_focused();
            break;
            
        case UI_SCREEN_SETTINGS:
            lv_screen_load(scr_settings);
            ui_settings_focused();
            break;
            
        case UI_SCREEN_CURVES:
            lv_screen_load(scr_curves);
            ui_curves_focused();
            break;
    }
}
