
/**
 * @file main
 *
 */


#include <stdlib.h>
#include <unistd.h>

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"

 #include <stdio.h>


lv_obj_t * main_screen;
lv_obj_t * settings_screen;

lv_obj_t * digit_labels[3];
lv_obj_t * preset_btn_1;
lv_obj_t * preset_btn_2;
lv_obj_t * preset_btn_3;
lv_obj_t * settings_btn;
lv_obj_t * num_label;
lv_obj_t * unit_label;
lv_obj_t * power_bar;
lv_obj_t * set_temp_label;
lv_group_t * g;

 // 1. Declare both fonts
LV_FONT_DECLARE(ds_digital_100);

static int current_temp = 20;
int target_temp = 350;

static const wchar_t * title = L"LVGL port Windows CodeBlocks.      https://lvgl.io | https://docs.lvgl.io";



void setup_main_screen(lv_obj_t * parent);
void power_timer_cb1(lv_timer_t * timer);
void power_timer_cb2(lv_timer_t * timer);
static void encoder_event_cb(lv_event_t * e);
static void preset_btn_event_cb(lv_event_t * e);
static void settings_event_cb(lv_event_t * e);
void create_settings_menu(lv_obj_t * parent);
static void back_to_main_cb(lv_event_t * e);





int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL for LVGL*/
    lv_display_t * display = lv_windows_create_display(title, 320, 240, 100, FALSE, FALSE);
    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
    lv_windows_acquire_pointer_indev(display);

    lv_indev_t * encoder_indev = lv_windows_acquire_encoder_indev(display);
    g = lv_group_create();
     lv_indev_set_group(encoder_indev, g);
    lv_group_set_default(g);




    main_screen = lv_obj_create(NULL); // Create the base object
    setup_main_screen(main_screen);    // Build the UI on it
    lv_scr_load(main_screen);          // Show it



    lv_timer_create(power_timer_cb1, 100, NULL);
    lv_timer_create(power_timer_cb2, 500, NULL);



    while(1) {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/

        lv_task_handler();
        usleep(5000);       /*Just to let the system breath*/
    }
    return 0;
}





void setup_main_screen(lv_obj_t * parent) {



    int start_x = -60; // Adjust this so your 3 digits are centered
    int spacing = 50;  // Width of your widest digit (the '8')

    for(int i = 0; i < 3; i++) {
        digit_labels[i] = lv_label_create(parent);
        lv_obj_set_style_text_font(digit_labels[i], &ds_digital_100, 0);
        lv_obj_set_style_text_color(digit_labels[i], lv_palette_main(LV_PALETTE_ORANGE), 0);
        lv_label_set_text(digit_labels[i], "0");

        // Lock the width so the '1' takes as much space as an '8'
        lv_obj_set_size(digit_labels[i], spacing, 100);
        lv_obj_set_style_text_align(digit_labels[i], LV_TEXT_ALIGN_CENTER, 0);

        // Position them side-by-side
        lv_obj_align(digit_labels[i], LV_ALIGN_CENTER, start_x + (i * spacing), 5);
    }


    // Degree Symbol & 'C' (lv_font_montserrat_18)
    unit_label = lv_label_create(parent);
    lv_obj_set_style_text_font(unit_label, &lv_font_montserrat_18, 0);
    lv_label_set_text(unit_label, "\xC2\xB0" "C");
    lv_obj_set_style_text_color(unit_label, lv_palette_main(LV_PALETTE_GREY), 0);

    // Position the "¡ãC" relative to the top-right of the numbers
    lv_obj_align_to(unit_label, digit_labels[2], LV_ALIGN_OUT_RIGHT_TOP, -5, -25);

        /* Create a style for the bar background */
    static lv_style_t style_bg;
    lv_style_init(&style_bg);
    lv_style_set_bg_color(&style_bg, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_opa(&style_bg, LV_OPA_30);
    lv_style_set_radius(&style_bg, 0); // Square look like the reference

    /* Create a style for the indicator (the part that grows) */
    static lv_style_t style_indic;
    lv_style_init(&style_indic);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_DEEP_ORANGE));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_YELLOW));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER); // Vertical gradient
    lv_style_set_radius(&style_indic, 0);

    /* Create the bar object */
   power_bar = lv_bar_create(parent);
    lv_obj_add_style(power_bar, &style_bg, 0);
    lv_obj_add_style(power_bar, &style_indic, LV_PART_INDICATOR);

    /* Make it vertical by setting width < height */
    lv_obj_set_size(power_bar, 12, 100);
    lv_obj_align(power_bar, LV_ALIGN_LEFT_MID, 20, 0); // Position it on the left side

    /* Set a value to test (e.g., 70%) */
    lv_bar_set_value(power_bar, 70, LV_ANIM_OFF);


        /* 1. Create a transparent vertical container (the "rail") */
    lv_obj_t * col = lv_obj_create(parent);
    lv_obj_set_size(col, 60, 60); // Enough width for "350"
    lv_obj_align(col, LV_ALIGN_RIGHT_MID, -10, 0); // Position the whole column

    /* Remove container styling so it's invisible */
    lv_obj_set_style_bg_opa(col, 0, 0);
    lv_obj_set_style_border_opa(col, 0, 0);
    lv_obj_set_scrollbar_mode(col, LV_SCROLLBAR_MODE_OFF);

    /* Use Flex to stack them vertically and center them */
    lv_obj_set_flex_flow(col, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(col, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(col, 2, 0); // Spacing between "SET" and "350"

    /* 2. Add "SET" label to the column */
    lv_obj_t * set_title = lv_label_create(col);
    lv_label_set_text(set_title, "SET");
    lv_obj_set_style_text_font(set_title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(set_title, lv_palette_main(LV_PALETTE_GREY), 0);

    /* 3. Add the value label to the column */
    set_temp_label = lv_label_create(col);
     lv_group_add_obj(g, set_temp_label); // Add to encoder group
     lv_obj_add_event_cb(set_temp_label, encoder_event_cb, LV_EVENT_ALL, NULL);
    lv_label_set_text(set_temp_label, "350");
    lv_obj_set_style_text_font(set_temp_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(set_temp_label, lv_palette_main(LV_PALETTE_ORANGE), 0);
    lv_obj_set_style_text_color(set_temp_label, lv_palette_main(LV_PALETTE_DEEP_ORANGE), LV_STATE_FOCUSED);
    lv_obj_set_style_text_color(set_temp_label, lv_palette_main(LV_PALETTE_PINK), LV_STATE_EDITED);
    lv_obj_add_flag(set_temp_label, LV_OBJ_FLAG_CLICKABLE);



        /* 1. Create a container to hold the buttons */
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 220, 50); // Width fits 320 screen well
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, -10);

    /* Remove container border and background for a clean look */
    lv_obj_set_style_bg_opa(cont, 0, 0);
    lv_obj_set_style_border_opa(cont, 0, 0);

    /* Use Flexbox to align buttons automatically */
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);



    /* 2. Helper to create three buttons */
    const char * temps[] = {"300", "320", "350"};


    preset_btn_1 = lv_btn_create(cont);
    lv_obj_set_size(preset_btn_1, 60, 30);

    lv_obj_set_style_border_width(preset_btn_1, 1, 0);
    lv_obj_set_style_radius(preset_btn_1, 4, 0);

    /* Add the temperature text */
    lv_obj_t * preset_label_1 = lv_label_create(preset_btn_1);
    lv_label_set_text(preset_label_1, temps[0]);
    lv_obj_center(preset_label_1);
    lv_obj_add_event_cb(preset_btn_1, preset_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_clear_flag(preset_btn_1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);


    preset_btn_2 = lv_btn_create(cont);
    lv_obj_set_size(preset_btn_2, 60, 30);

    lv_obj_set_style_border_width(preset_btn_2, 1, 0);
    lv_obj_set_style_radius(preset_btn_2, 4, 0);

    /* Add the temperature text */
    lv_obj_t * preset_label_2 = lv_label_create(preset_btn_2);
    lv_label_set_text(preset_label_2, temps[1]);
    lv_obj_center(preset_label_2);
    lv_obj_add_event_cb(preset_btn_2, preset_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_clear_flag(preset_btn_2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);


    preset_btn_3 = lv_btn_create(cont);
    lv_obj_set_size(preset_btn_3, 60, 30);

    lv_obj_set_style_border_width(preset_btn_3, 1, 0);
    lv_obj_set_style_radius(preset_btn_3, 4, 0);

    /* Add the temperature text */
    lv_obj_t * preset_label_3 = lv_label_create(preset_btn_3);
    lv_label_set_text(preset_label_3, temps[2]);
    lv_obj_center(preset_label_3);
    lv_obj_add_event_cb(preset_btn_3, preset_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_clear_flag(preset_btn_3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);






    lv_obj_set_style_pad_all(lv_scr_act(), 0, 0); // Clear screen padding

    lv_obj_t * header = lv_obj_create(parent);
    lv_obj_set_size(header, lv_pct(100), 20);
    lv_obj_set_scrollbar_mode(header, LV_SCROLLBAR_MODE_OFF);

    // Styling
    lv_obj_set_style_radius(header, 0, 0);
    lv_obj_set_style_bg_color(header, lv_color_hex(0x222222), 0);
    lv_obj_set_style_bg_opa(header, LV_OPA_20, 0);
    lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_width(header, 1, 0);
    lv_obj_set_style_border_color(header, lv_palette_main(LV_PALETTE_GREY), 0);

    // Align items inside (using the 20px padding you wanted)
    lv_obj_set_style_pad_hor(header, 10, 0);
    lv_obj_set_style_pad_ver(header, 0, 0);


    lv_obj_t * tool_type = lv_label_create(header);
    lv_label_set_text(tool_type, "C245");
    lv_obj_set_style_text_font(tool_type, &lv_font_montserrat_14, 0);
    lv_obj_align(tool_type, LV_ALIGN_CENTER, -10,  0);


    lv_obj_t * stats = lv_label_create(header);
    lv_label_set_text(stats, "24.2V  1.5A");
    lv_obj_set_style_text_font(stats, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(stats, lv_palette_main(LV_PALETTE_CYAN), 0);
    lv_obj_align(stats, LV_ALIGN_RIGHT_MID, 0, 0);


//    lv_obj_t * icon = lv_label_create(header);
//    lv_label_set_text(icon, LV_SYMBOL_SETTINGS);
//    lv_obj_align(icon, LV_ALIGN_LEFT_MID, 0, 0);
//
//    lv_obj_add_flag(icon, LV_OBJ_FLAG_CLICKABLE); // Make it responsive
//    lv_obj_add_event_cb(icon, settings_event_cb, LV_EVENT_CLICKED, NULL); // Add a callback
//    lv_group_add_obj(g, icon);

    // 1. Create a small button in the header
    settings_btn = lv_btn_create(header);
    lv_obj_set_size(settings_btn, 20, 20); // Small enough for the header
    lv_obj_align(settings_btn, LV_ALIGN_LEFT_MID, 0, 0);
    //lv_obj_set_style_bg_opa(settings_btn, 0, 0);      // Make it invisible
    lv_obj_set_style_shadow_width(settings_btn, 0, 0); // No shadow
    lv_obj_set_style_border_width(settings_btn, 0, 0); // No border

    // 2. Put the icon INSIDE the button
    lv_obj_t * icon_label = lv_label_create(settings_btn);
    lv_label_set_text(icon_label, LV_SYMBOL_SETTINGS);
    lv_obj_center(icon_label);

    lv_obj_set_style_text_color(icon_label, lv_color_white(), 0);
// Also set it for the pressed/focused state so it doesn't turn gray then either
lv_obj_set_style_text_color(icon_label, lv_color_white(), LV_STATE_FOCUS_KEY);
lv_obj_set_style_text_color(icon_label, lv_color_white(), LV_STATE_PRESSED);
lv_obj_set_style_pad_all(settings_btn, 0, 0);

    // 3. Add the callback to the BUTTON, not the label
    lv_obj_add_event_cb(settings_btn, settings_event_cb, LV_EVENT_CLICKED, NULL);

    // 4. Add the BUTTON to your group
    lv_group_add_obj(g, settings_btn);


}





// Example: Update the bar and the color based on power level
void power_timer_cb1(lv_timer_t * timer){
//    static int p = 0;
//    static int dir = 1;
//
//    p += dir;
//    if(p >= 100 || p <= 0) dir *= -1; // Bounce 0-100
//    lv_bar_set_value(power_bar, p, LV_ANIM_ON);


    if(current_temp < target_temp) {
        current_temp += 2; // Simulate heating
        lv_bar_set_value(power_bar, 100, LV_ANIM_ON); // Full power while heating
    } else {
        current_temp = target_temp + (rand() % 2 - 1); // Slight flicker at target
        lv_bar_set_value(power_bar, 20, LV_ANIM_ON);  // Low power to maintain
    }

}


void power_timer_cb2(lv_timer_t * timer){

    char buf[8];
    sprintf(buf, "%03d", current_temp);

    for(int i = 0; i < 3; i++) {
        // Create a temporary string for one character
        char single_digit[2] = { buf[i], '\0' };
        lv_label_set_text(digit_labels[i], single_digit);
    }
}










static void encoder_event_cb(lv_event_t * e) {

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    // Only change temperature if the object is in "EDIT" mode (clicked/entered)
    if(code == LV_EVENT_KEY) {
        if(lv_obj_has_state(obj, LV_STATE_EDITED)) {
            uint32_t key = lv_event_get_key(e);
            if(key == LV_KEY_RIGHT) target_temp -= 5;
            if(key == LV_KEY_LEFT)  target_temp += 5;

            if(target_temp > 450) target_temp = 450; // Max limit
            if(target_temp < 150) target_temp = 150; // Min limit

            char buf[8];
            sprintf(buf, "%d", target_temp);
            lv_label_set_text(set_temp_label, buf);
        }
    }
    else if(code == LV_EVENT_LONG_PRESSED) {
        // Handle your menu transition here
        lv_obj_clean((lv_scr_act()));
    }
}


static void preset_btn_event_cb(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(btn, 0);
    const char * txt = lv_label_get_text(label);

    target_temp = atoi(txt); // Update global target
    lv_label_set_text(set_temp_label, txt); // Update UI
}



static void settings_event_cb(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {

        // 1. Clean the group first
        lv_group_remove_all_objs(g);

        // 2. Create the screen only if it doesn't exist
        if(settings_screen == NULL) {
            settings_screen = lv_obj_create(NULL);
            lv_obj_set_style_bg_color(settings_screen, lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_bg_opa(settings_screen, LV_OPA_COVER, 0);
        } else {
            lv_obj_clean(settings_screen); // Clear old menu items if they exist
        }

        // 3. Rebuild the menu items
        create_settings_menu(settings_screen);

        // 4. Load with FALSE (do NOT delete the main screen)
        lv_screen_load_anim(settings_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
    }
}


void create_settings_menu(lv_obj_t * parent) {
    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(100));
    lv_obj_center(list);

    // Clear the group first
    //lv_group_remove_all_objs(g);

    lv_list_add_text(list, "Soldering Settings");

    // Capture the button pointers and add them to the group
    lv_obj_t * btn_cal = lv_list_add_button(list, LV_SYMBOL_EDIT, "Calibration");
    lv_group_add_obj(g, btn_cal);

    lv_obj_t * btn_adv = lv_list_add_button(list, LV_SYMBOL_SETTINGS, "Advanced");
    lv_group_add_obj(g, btn_adv);

    lv_list_add_text(list, "System");

    lv_obj_t * btn_back = lv_list_add_button(list, LV_SYMBOL_CLOSE, "Back");
    lv_group_add_obj(g, btn_back);

    // Now the encoder will cycle through these three buttons!
    // Optional: add your callback back to btn_back
    lv_obj_add_event_cb(btn_back, back_to_main_cb, LV_EVENT_CLICKED, NULL);
}





static void back_to_main_cb(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        // 1. Clear the menu buttons from the group
        lv_group_remove_all_objs(g);

        // 2. Re-add the main screen items so the encoder works again
        lv_group_add_obj(g, set_temp_label);
        lv_group_add_obj(g, preset_btn_1);
        lv_group_add_obj(g, preset_btn_2);
        lv_group_add_obj(g, preset_btn_3);
        lv_group_add_obj(g, settings_btn); // Don't forget the gear icon!

        // 3. Slide back to main screen
        lv_screen_load_anim(main_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
    }
}



