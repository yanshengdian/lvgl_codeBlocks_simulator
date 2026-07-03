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

|  REAL-TIME CHART                                                  | Y=24 (Text Header)
+-------------------------------------------------------------------+

| 400|   ____________________..--'''``--.._                         | Y=35
|    | _.-'                                 `-.                     |
| 200|'                                        `-.                  | Y=110 (Chart Canvas)
|    |                                            `-.               |
|   0+-------------------------------------------------             | Y=185
|     0s      10s     20s     30s     40s     50s    60s            |
+-------------------------------------------------------------------+

|    +-------------------------+     +-------------------------+    |
|    |       ⬅️ MAIN UI        |     |      ⚙️ SETTINGS         |    | Y=230 (Action Row)
|    +-------------------------+     +-------------------------+    |
+-------------------------------------------------------------------+ Y=240
X=0                       X=160   X=170                         X=320

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
