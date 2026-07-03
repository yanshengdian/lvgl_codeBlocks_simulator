/**
 * @file main
 *
 * During this session, only read or modify these paths:
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\main.c
- f:\work\Desktop\lvgl_pc_simulation_codeBlocks\lvgl_codeBlocks_simulator\user_app\*

Do not read, search, or touch any other files in the project.
 */

#include <stdlib.h>
#include <unistd.h>

#include "lvgl/lvgl.h"
#include "user_app/ui_manager.h"

static const wchar_t * title = L"LVGL port Windows CodeBlocks.      https://lvgl.io | https://docs.lvgl.io";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
    /* Initialize LVGL */
    lv_init();

    /* Initialize the HAL for LVGL */
    lv_display_t * display = lv_windows_create_display(title, 320, 240, 100, FALSE, FALSE);
    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
    lv_windows_acquire_pointer_indev(display);
    lv_windows_acquire_encoder_indev(display);

    ui_init();

    while (1) {
        lv_task_handler();
        usleep(5000);
    }

    return 0;
}
