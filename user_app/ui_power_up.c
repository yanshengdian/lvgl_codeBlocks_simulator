
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

void ui_power_up_init(void)
{
    // 1. Create a screen object (or use an existing active screen)
    lv_obj_t * scr_power_up = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_power_up, lv_color_black(), 0); // Black background

    // 2. Create the image widget on this screen
    lv_obj_t * img_logo = lv_image_create(scr_power_up);
    
    // 3. Set the source to your converted C array image descriptor
    lv_image_set_src(img_logo, &logo);
    
    // 4. Center the logo perfectly on your 320x240 screen
    lv_obj_center(img_logo);

    // 5. Load the screen to make it visible
    lv_screen_load(scr_power_up);
}
