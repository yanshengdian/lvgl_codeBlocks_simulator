/*
During this session, only read or modify these paths:
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\main.c
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\user_app\*

Do not read, search, or touch any other files in the project.
*/



/*


+-------------------------------------------------------------------+ Y=0

| [??][??][??][??]  24.0V  10.0A  240W                 ??100% 10:15 | Y=24 (Status Bar)
+-------------------------------------------------------------------+

|                                                                   |
|   CURRENT VALUE                                                   |
|   #######################                                         |
|   #                     #        SETTING VALUE                    |
|   #   375.4 °C          #       ###############                   |
|   #                     #       # [ 380.0 °C ]# <-- Half size font| Y=115 (Main Values)
|   #######################       ###############                   |
|                                                                   |
|   POWER OUTPUT                                                    |
|   [?????????????????????????] 60%                                 | Y=165 (Power Bar)
|   |                                                               |
|   +-------------------------+     +-------------------------+     |
|   |      ?? SETTINGS         |     |       ?? GRAPH VIEW     |     | Y=225 (Buttons)
|   +-------------------------+     +-------------------------+     |
+-------------------------------------------------------------------+ Y=240
X=0                       X=160   X=170                         X=320


*/






#include "ui_power_up.h"
#include "ui_manager.h"

static void ui_power_up_timeout_cb(lv_timer_t * timer)
{
    lv_timer_del(timer);
    ui_navigate_to(UI_SCREEN_MAIN);
}

void ui_power_up_init(void)
{
    /* Create a screen object and keep it in the UI manager. */
    scr_power_up = lv_obj_create(NULL);

    /* Create the image widget on this screen. */
    lv_obj_t * img_logo = lv_image_create(scr_power_up);
    lv_image_set_src(img_logo, &logo);
    lv_obj_center(img_logo);

    /* Display this screen for 3 seconds, then switch to main. */
    lv_timer_create(ui_power_up_timeout_cb, 3000, NULL);
}
