#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "lvgl/lvgl.h"

/* Enum for screen tracking */
typedef enum {
    UI_SCREEN_MAIN,
    UI_SCREEN_SETTINGS,
    UI_SCREEN_CURVES
} ui_screen_t;

/* Global initialization function called by main.c */
void ui_init(void);

/* Global function to navigate between screens */
void ui_navigate_to(ui_screen_t screen);

/* Global pointer to the encoder group so individual screens can add their buttons to it */
extern lv_group_t * ui_encoder_group;

#endif /* UI_MANAGER_H */
