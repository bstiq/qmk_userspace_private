#include QMK_KEYBOARD_H
#include "introspection.h"
#include "lvgl.h"
#include <ctype.h>

#include "screen_pomodoro.h"
#include "screen_base.h"
#include "lcd.h"
#include "ui_elements.h"

obj_event_array_t objects_and_events;
obj_event_array_t menus;
lv_obj_t         *ui_screen_pomodoro;
lv_obj_t         *ui_screen_pomodoro_menu;

// TODO move this into a getter, isolate into lcd.c
extern dilemma_status_t dilemma_lcd_status;
extern dilemma_status_t dilemma_lcd_status_prev;

uint32_t timer_start = 0;
// TODO add config options for this
uint32_t timer_max = 3 * 60 * 1000; // default 3 minutes for now

uint8_t menu_index = 0;

bool timer_is_running = false;

// TODO isolate the ui_screen_base into this folder, and instead return a pointer to it with this function?
lv_obj_t *init_screen_pomodoro(void) {
    ui_screen_pomodoro      = lv_obj_create(NULL);
    ui_screen_pomodoro_menu = lv_obj_create(NULL);

    lv_obj_t *cont      = ui_create_container(ui_screen_pomodoro);
    lv_obj_t *cont_menu = ui_create_container(ui_screen_pomodoro_menu);

    init_obj_event_array(&objects_and_events);
    init_obj_event_array(&menus);

    /* ----- Widgets ----- */

    add_obj_event_array(&objects_and_events, (obj_update_t){
                                                 ui_create_pomodoro_title(cont),
                                                 NULL,
                                             });

    add_obj_event_array(&objects_and_events, (obj_update_t){
                                                 ui_create_pomodoro_arc(cont),
                                                 &update_pomodoro_arc,
                                             });

    add_obj_event_array(&objects_and_events, (obj_update_t){
                                                 ui_create_pomodoro_time(cont),
                                                 &update_pomodoro_time,
                                             });

    /* ----- Menus ----- */

    add_obj_event_array(&menus, (obj_update_t){
                                    ui_create_mod_button(cont_menu, "Back to pomodoro", true, 0),
                                    &menu_pomodoro_back_to_main,
                                });
    add_obj_event_array(&menus, (obj_update_t){
                                    ui_create_mod_button(cont_menu, "Start 25 minutes", true, 0),
                                    &menu_pomodoro_back_to_main,
                                });
    add_obj_event_array(&menus, (obj_update_t){
                                    ui_create_mod_button(cont_menu, "Play / Pause", true, 0),
                                    &menu_pomodoro_back_to_main,
                                });
    add_obj_event_array(&menus, (obj_update_t){
                                    ui_create_mod_button(cont_menu, "Start 10 minutes", true, 0),
                                    &menu_pomodoro_back_to_main,
                                });
    add_obj_event_array(&menus, (obj_update_t){
                                    ui_create_mod_button(cont_menu, "Reset", true, 0),
                                    &menu_pomodoro_back_to_main,
                                });
    add_obj_event_array(&menus, (obj_update_t){
                                    ui_create_mod_button(cont_menu, "< Back to Main", true, 0),
                                    &menu_pomodoro_back_to_main,
                                });

    return ui_screen_pomodoro;
}

void menu_pomodoro_back_to_main(lv_obj_t *obj) {
    // TODO
}

// TODO run a loop of this, rather than using hard-set array numbers
// TODO does this work if the keyboard is secondary instead of master?
// we probably need to send the info over through rpc, just like in screen_base.
// would it be worth it to make a pattern? since we will need to do this for other screens as well
void refresh_screen_pomodoro(void) {
    static int last_layer;
    int        current_layer = get_highest_layer(layer_state);

    if (current_layer != last_layer) {
        switch (current_layer) {
            case 0:
            default:
                lv_disp_load_scr(ui_screen_pomodoro);
                // make sure all the buttons are released, sometimes they don't unpress correctly
                for (int i = 0; i < menus.amount_elements; i++) {
                    lv_event_send(menus.array[i].obj, LV_EVENT_RELEASED, NULL);
                }
                // todo move this to a function?
                // we went back to base layer. Let's check if there was a menu selected
                switch (menu_index) {
                    case 0:
                    default:
                        break;
                    case 1:
                        // start 25 minutes
                        timer_start      = timer_read32();
                        timer_max        = 25 * 60 * 1000;
                        timer_is_running = true;
                        break;
                    case 2:
                        // play / pause
                        timer_is_running = !timer_is_running;
                        break;
                    case 3:
                        // start 10 minutes
                        timer_start      = timer_read32();
                        timer_max        = 10 * 60 * 1000;
                        timer_is_running = true;
                        break;
                    case 4:
                        // reset
                        // TODO is this really useful?
                        timer_is_running = false;
                        break;
                }

                break;
            // TODO replace with LAYER_LCD instead of hardcoding
            // display the menu
            case 4:
                // by default, the top button is pushed
                menu_index = 0;
                lv_event_send(menus.array[0].obj, LV_EVENT_PRESSED, NULL);
                lv_disp_load_scr(ui_screen_pomodoro_menu);
                break;
        }
    }

    last_layer = current_layer;

    // if timer is running, then update the time
    // if (timer_is_running) {
        // TODO this is duplicate code
        int i = 0;
        for (i = 0; i < objects_and_events.amount_elements; i++) {
            lv_obj_t *obj = objects_and_events.array[i].obj;
            if (obj && objects_and_events.array[i].update_function) objects_and_events.array[i].update_function(obj);
        }
    // }
}

void update_pomodoro_time(lv_obj_t *obj) {
    if (timer_is_running == true) {
        // TODO update the timer text
        uint32_t elapsed         = timer_max - timer_elapsed32(timer_start);
        uint16_t minutes_elapsed = elapsed / 60000;
        uint16_t seconds_elapsed = (elapsed % 60000) / 1000;

        char  buffer[50];
        char *at = buffer;
        at += sprintf(at, "%02u", minutes_elapsed);
        at += sprintf(at, ":");
        at += sprintf(at, "%02u", seconds_elapsed);

        lv_label_set_text(obj, buffer);
    } else {
        lv_label_set_text(obj, "PAUSED");
    }
}

lv_obj_t *ui_create_pomodoro_title(lv_obj_t *cont) {
    lv_obj_t    *button = lv_btn_create(cont);
    ui_styles_t *styles = get_current_ui_styles();
    lv_obj_add_style(button, &styles->layer_name, 0);
    lv_obj_set_flex_grow(button, 1); // take all remaining space in line

    lv_obj_t *label = lv_label_create(button);
    // TODO, this should not be here
    lv_label_set_text(label, "POMODORO");
    lv_obj_center(label);
    lv_obj_set_width(label, LV_SIZE_CONTENT);
    lv_obj_set_height(label, 30);
    // lv_obj_set_x(ui_label_layer, 0);
    // lv_obj_set_y(ui_label_layer, 20);

    return label;
}

// for now only a text timer with dummy content
// TODO change text
lv_obj_t *ui_create_pomodoro_time(lv_obj_t *cont) {
    lv_obj_t    *button = lv_btn_create(cont);
    ui_styles_t *styles = get_current_ui_styles();
    lv_obj_add_style(button, &styles->layer_name, 0);
    lv_obj_set_flex_grow(button, 1);                        // take all remaining space in line
    lv_obj_add_flag(button, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK); // new line

    lv_obj_t *label = lv_label_create(button);
    // TODO, should this really be here?
    lv_label_set_text(label, "PAUSED");
    lv_obj_center(label);
    lv_obj_set_width(label, LV_SIZE_CONTENT);
    lv_obj_set_height(label, 30);

    return label;
}

lv_obj_t *ui_create_pomodoro_arc(lv_obj_t *cont) {
    lv_obj_t    *arc    = lv_arc_create(cont);
    ui_styles_t *styles = get_current_ui_styles();

    lv_obj_add_style(arc, &styles->bar, LV_PART_INDICATOR);
    lv_obj_add_flag(arc, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK); // new line

    // TODO move this into a style init
    // we could do a post-processing in style init where we copy over the values... right now this breaks on theme change
    // each screen should have their own load_themes function, right now everything lives in theme.c
    // ideally we should derive a new pomodoro_styles->arc from styles->bar, and then set the arc specific styles in the init function
    // copy the bar color into the arc color
    lv_style_value_t v;
    lv_res_t         res = lv_style_get_prop(&styles->bar, LV_STYLE_BG_COLOR, &v);
    if (res == LV_RES_OK) { /*Found*/
        lv_style_set_arc_color(&styles->bar, v.color);
        // hide the knob
        lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    }

    lv_obj_set_size(arc, 150, 150);
    lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 0, 270);
    lv_arc_set_value(arc, 40);
    lv_obj_center(arc);

    return arc;
}

void update_pomodoro_arc(lv_obj_t *obj) {
    uint32_t elapsed         = timer_max - timer_elapsed32(timer_start);
    uint16_t elapsed_percent = (elapsed * 100) / timer_max;
    lv_arc_set_value(obj, elapsed_percent);
}

// TODO does this work well when the keyboard is not master?....
// TODO this is something that will be reused in other menus, so we should move it maybe to screens/menu_nav.c ?
bool process_record_screen_pomodoro(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LCD_MENU_NEXT:
            if (record->event.pressed) {
                lv_event_send(menus.array[menu_index].obj, LV_EVENT_RELEASED, NULL);
                menu_index = (menu_index + 1) % menus.amount_elements;
                lv_event_send(menus.array[menu_index].obj, LV_EVENT_PRESSED, NULL);
            }
            break;
        case LCD_MENU_PREV:
            if (record->event.pressed) {
                lv_event_send(menus.array[menu_index].obj, LV_EVENT_RELEASED, NULL);
                menu_index = (menu_index - 1 + menus.amount_elements) % menus.amount_elements;
                lv_event_send(menus.array[menu_index].obj, LV_EVENT_PRESSED, NULL);
            }
            break;
    }
    return true;
}
