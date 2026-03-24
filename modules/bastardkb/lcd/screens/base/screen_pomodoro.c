#include QMK_KEYBOARD_H
#include "introspection.h"
#include "lvgl.h"
#include <ctype.h>

#include "screen_pomodoro.h"
#include "screen_base.h"
#include "lcd.h"
#include "ui_elements.h"

obj_event_array_t event_with_objects_array;
lv_obj_t *ui_screen_pomodoro;

// TODO move this into a getter, isolate into lcd.c
extern dilemma_status_t dilemma_lcd_status;
extern dilemma_status_t dilemma_lcd_status_prev;

// TODO isolate the ui_screen_base into this folder, and instead return a pointer to it with this function?
lv_obj_t *init_screen_pomodoro(void) {
    ui_screen_pomodoro = lv_obj_create(NULL);
    init_obj_event_array(&event_with_objects_array);

    // TODO this is duplicate code
    lv_obj_t *cont = lv_obj_create(ui_screen_pomodoro);
    lv_obj_set_size(cont, LCD_WIDTH, LCD_HEIGHT);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    // TODO move this to theme.c, in eg. create_container
    ui_styles_t *styles = get_current_ui_styles();
    lv_obj_add_style(cont, &styles->flex_container, 0);

    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_pomodoro_title(cont),
                                                       NULL,
                                                   });

    // mod buttons: SHIFT, ALT, CTRL, GUI
    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_pomodoro_timer(cont),
                                                       NULL,
                                                   });
   
    return ui_screen_pomodoro;
}

void refresh_screen_pomodoro(void) {
    int i = 0;
    for (i = 0; i < event_with_objects_array.amount_elements; i++) {
        lv_obj_t *obj = event_with_objects_array.array[i].obj;
        if (obj && event_with_objects_array.array[i].update_function) event_with_objects_array.array[i].update_function(obj);
    }
}

lv_obj_t *ui_create_pomodoro_title(lv_obj_t *cont){
    lv_obj_t *button = lv_btn_create(cont);
    ui_styles_t *styles = get_current_ui_styles();
    lv_obj_add_style(button, &styles->layer_name, 0);
    lv_obj_set_flex_grow(button, 1); // take all remaining space in line

    lv_obj_t *label = lv_label_create(button);
    // TODO, this should not be here
    lv_label_set_text(label, "POMODORO");
    lv_obj_center(label);
    lv_obj_set_width(label, LV_SIZE_CONTENT);
    // lv_obj_set_x(ui_label_layer, 0);
    // lv_obj_set_y(ui_label_layer, 20);

    return label;
}

// for now only a text timer with dummy content
// TODO change text
lv_obj_t *ui_create_pomodoro_timer(lv_obj_t *cont){
    lv_obj_t *button = lv_btn_create(cont);
    ui_styles_t *styles = get_current_ui_styles();
    lv_obj_add_style(button, &styles->layer_name, 0);
    lv_obj_set_flex_grow(button, 1); // take all remaining space in line
    lv_obj_add_flag(button, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK); // new line

    lv_obj_t *label = lv_label_create(button);
    // TODO, this should not be here
    lv_label_set_text(label, "25:00");
    lv_obj_center(label);
    lv_obj_set_width(label, LV_SIZE_CONTENT);
    lv_obj_set_height(label, 100);
    // lv_obj_set_x(label, 0);
    // lv_obj_set_y(label, 20);

    return label;
}
