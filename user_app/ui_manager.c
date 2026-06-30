#include "ui_manager.h"
#include "ui_main.h"
#include "ui_settings.h"
#include "ui_curves.h"

/* Allocate global screens */
lv_obj_t * scr_main = NULL;
lv_obj_t * scr_settings = NULL;
lv_obj_t * scr_curves = NULL;

/* Allocate global navigation group */
lv_group_t * ui_encoder_group = NULL;

void ui_init(void)
{
    /* 1. Create a global input group for our encoder simulator */
    ui_encoder_group = lv_group_create();

    /* 2. Bind the default simulator input device to our new group */
    lv_indev_t * indev = lv_indev_get_next(NULL);
    while(indev) {
        if(lv_indev_get_type(indev) == LV_INDEV_TYPE_ENCODER ||
           lv_indev_get_type(indev) == LV_INDEV_TYPE_KEYPAD) {
            lv_indev_set_group(indev, ui_encoder_group);
            break;
        }
        indev = lv_indev_get_next(indev);
    }

    /* 3. Pre-initialize all screen architectures in memory */
    ui_main_init();
    ui_settings_init();
    ui_curves_init();

    /* 4. Start the application on the main interface */
    ui_navigate_to(UI_SCREEN_MAIN);
}

void ui_navigate_to(ui_screen_t screen)
{
    /* Clear any old highlighted focus items from our encoder wheel */
    lv_group_remove_all_objs(ui_encoder_group);

    /* Route the target view layout and let the respective screen load its encoder targets */
    switch(screen) {
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
