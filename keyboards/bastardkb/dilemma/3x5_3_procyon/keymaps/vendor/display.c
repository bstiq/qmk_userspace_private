#include "display.h"
#include "quantum.h"

lv_obj_t *ui_screen_base;
lv_obj_t *ui_screen_pointer;

lv_obj_t *ui_label_layer_name_base;
// lv_obj_t *ui_label_layer_name_pointer;
lv_obj_t *ui_label_mod_gui;
lv_obj_t *ui_button_mod_gui;
lv_obj_t *ui_label_mod_shift;
lv_obj_t *ui_button_mod_shift;
lv_obj_t *ui_label_mod_control;
lv_obj_t *ui_button_mod_control;
lv_obj_t *ui_label_mod_alt;
lv_obj_t *ui_button_mod_alt;
lv_obj_t *ui_label_dpi;
lv_obj_t *ui_bar_dpi;
lv_obj_t *ui_label_s_dpi;
lv_obj_t *ui_bar_s_dpi;
lv_obj_t *ui_label_sniping;
lv_obj_t *ui_switch_sniping;
lv_obj_t *ui_label_scroll;
lv_obj_t *ui_switch_scroll;

lv_style_t style_btn;
lv_style_t style_bar;
lv_style_t style_btn_pressed;
lv_style_t style_flex_container;
uint8_t    last_mods;
uint8_t    mods;
bool       last_sniping;
bool       last_scrolling;

enum ui_user_events {
    EVENT_LAYER_CHANGE = 0,
    EVENT_MOD_CHANGE,
    EVENT_LAST_EVENT,
};

const char *ui_layer_strings[] = {"BASE", "FUNCTION", "NAV", "MED/RGB", "POINTER", "NUM", "SYM"};

void display_init(void) {
    /*
        Base layer screen
    */
    ui_screen_base = lv_obj_create(NULL);
    style_init_mod_indicator();
    style_pressed_init_mod_indicator();
    style_bar_init();
    style_flex_container_init();

    lv_obj_t *cont = lv_obj_create(ui_screen_base);
    lv_obj_set_size(cont, 240, 280); // todo change to screen height
    lv_obj_center(cont);
    // lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_add_style(cont, &style_flex_container, 0);

    ui_label_layer_name_base = lv_label_create(cont);
    ui_init_layer_name(ui_label_layer_name_base, "Base");
    lv_obj_set_size(ui_label_layer_name_base, 200, 30);

    ui_button_mod_gui = lv_btn_create(cont);
    ui_init_button_mod_indicator(ui_button_mod_gui, 80, 80);
    ui_label_mod_gui = lv_label_create(ui_button_mod_gui);
    lv_label_set_text(ui_label_mod_gui, "Gui");
    lv_obj_center(ui_label_mod_gui);

    ui_button_mod_control = lv_btn_create(cont);
    ui_init_button_mod_indicator(ui_button_mod_control, 150, 80);
    ui_label_mod_control = lv_label_create(ui_button_mod_control);
    lv_label_set_text(ui_label_mod_control, "Ctrl");
    lv_obj_center(ui_label_mod_control);

    ui_button_mod_alt = lv_btn_create(cont);
    ui_init_button_mod_indicator(ui_button_mod_alt, 80, 150);
    ui_label_mod_alt = lv_label_create(ui_button_mod_alt);
    lv_label_set_text(ui_label_mod_alt, "Alt");
    lv_obj_center(ui_label_mod_alt);

    ui_button_mod_shift = lv_btn_create(cont);
    ui_init_button_mod_indicator(ui_button_mod_shift, 150, 150);
    ui_label_mod_shift = lv_label_create(ui_button_mod_shift);
    lv_label_set_text(ui_label_mod_shift, "Shft");
    lv_obj_center(ui_label_mod_shift);

    // display base layer screen upon init
    lv_disp_load_scr(ui_screen_base);

    /*
        Pointer screen
    */
    // todo delete this line
    ui_screen_pointer = lv_obj_create(NULL);

    // cont      = lv_obj_create(ui_screen_pointer);
    lv_obj_set_size(cont, 240, 280); // todo change to screen height
    lv_obj_add_style(cont, &style_flex_container, 0);

    // ui_label_layer_name_pointer = lv_label_create(cont);
    // ui_init_layer_name(ui_label_layer_name_pointer, "Pointer");
    // lv_obj_set_size(ui_label_layer_name_pointer, 200, 40);

    ui_label_dpi = lv_label_create(cont);
    lv_label_set_text(ui_label_dpi, "DPI");
    lv_obj_add_flag(ui_label_dpi, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK); // force new line
    ui_bar_dpi = lv_bar_create(cont);
    lv_obj_set_height(ui_bar_dpi, 20);
    lv_obj_set_flex_grow(ui_bar_dpi, 1); // take all remaining space in line
    lv_obj_add_style(ui_bar_dpi, &style_bar, LV_PART_INDICATOR);

    ui_label_s_dpi = lv_label_create(cont);
    lv_label_set_text(ui_label_s_dpi, "Snip. DPI");
    lv_obj_add_flag(ui_label_s_dpi, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK); // force new line
    ui_bar_s_dpi = lv_bar_create(cont);
    lv_obj_set_height(ui_bar_s_dpi, 20);
    lv_obj_set_flex_grow(ui_bar_s_dpi, 1); // take all remaining space in line
    lv_obj_add_style(ui_bar_s_dpi, &style_bar, LV_PART_INDICATOR);

    ui_label_sniping = lv_label_create(cont);
    lv_label_set_text(ui_label_sniping, "Snip");
    lv_obj_add_flag(ui_label_sniping, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK); // force new line
    lv_obj_set_size(ui_label_sniping, 45, 20);
    ui_switch_sniping = lv_switch_create(cont);
    lv_obj_set_size(ui_switch_sniping, 40, 20);
    lv_obj_add_event_cb(ui_switch_sniping, event_screen_pointer_sniping_toggle, LV_EVENT_ALL, NULL);

    ui_label_scroll = lv_label_create(cont);
    lv_label_set_text(ui_label_scroll, "Scroll");
    lv_obj_set_size(ui_label_scroll, 60, 20);
    ui_switch_scroll = lv_switch_create(cont);
    lv_obj_set_size(ui_switch_scroll, 40, 20);
    lv_obj_add_event_cb(ui_switch_scroll, event_screen_pointer_scroll_toggle, LV_EVENT_ALL, NULL);

    /*
        Theme
    */
    lv_disp_t  *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    prev_layer   = 99;
    last_mods    = get_mods();
    last_sniping = false;
}

void style_init_mod_indicator(void) {
    /*Create a simple button style*/
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 3);
    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);

    lv_style_set_bg_color(&style_btn, lv_palette_darken(LV_PALETTE_PINK, 1));
    lv_style_set_bg_grad_color(&style_btn, lv_palette_darken(LV_PALETTE_PINK, 3));
    lv_style_set_bg_grad_dir(&style_btn, LV_GRAD_DIR_VER);
    
    // lv_style_set_bg_color(&style_btn, lv_palette_lighten(LV_PALETTE_PINK, 1), LV_STATE_PRESSED);

    lv_style_set_border_color(&style_btn, lv_palette_lighten(LV_PALETTE_PINK, 1));
    lv_style_set_border_opa(&style_btn, LV_OPA_20);
    lv_style_set_border_width(&style_btn, 2);

    lv_style_set_text_color(&style_btn, lv_color_white());
}

void style_pressed_init_mod_indicator(void) {
    /*Create a simple button style*/
    lv_style_init(&style_btn_pressed);
    lv_style_set_radius(&style_btn_pressed, 2);
    lv_style_set_bg_opa(&style_btn_pressed, LV_OPA_COVER);

    lv_style_set_bg_color(&style_btn_pressed, lv_palette_lighten(LV_PALETTE_PINK, 3));
    lv_style_set_bg_grad_color(&style_btn_pressed, lv_palette_lighten(LV_PALETTE_PINK, 1));
    lv_style_set_bg_grad_dir(&style_btn_pressed, LV_GRAD_DIR_VER);

    lv_style_set_border_color(&style_btn_pressed, lv_palette_lighten(LV_PALETTE_PINK, 1));
    lv_style_set_border_opa(&style_btn_pressed, LV_OPA_20);
    lv_style_set_border_width(&style_btn_pressed, 2);

    lv_style_set_text_color(&style_btn_pressed, lv_color_black());
}

void style_bar_init(void) {
    // lv_style_set_border_color(&style_bar, lv_palette_lighten(LV_PALETTE_PINK, 1));
    lv_style_set_radius(&style_bar, 3);
    lv_style_set_radius(&style_bar, 6);
    lv_style_set_bg_color(&style_bar, lv_palette_darken(LV_PALETTE_PINK, 3));
}

void ui_init_layer_name(lv_obj_t *label, const char *layer_name) {
    lv_obj_remove_style_all(label);
    lv_label_set_text(label, layer_name);
    lv_obj_set_width(label, LV_SIZE_CONTENT);
    // lv_obj_set_height(label, 30);
    lv_obj_set_x(label, 0);
    lv_obj_set_y(label, 20);
    lv_obj_set_align(label, LV_ALIGN_TOP_MID);
}


void style_flex_container_init(void){
    lv_style_set_bg_color(&style_flex_container, lv_color_black());
    lv_style_set_flex_flow(&style_flex_container, LV_FLEX_FLOW_ROW_WRAP);
    // lv_style_set_pad_row(&style_flex_container, 5);
}

void ui_init_button_mod_indicator(lv_obj_t *button, int x, int y) {
    // styles
    // lv_obj_remove_style_all(button);
    lv_obj_add_style(button, &style_btn, 0);
    lv_obj_add_style(button, &style_btn_pressed, LV_STATE_PRESSED);

    // behaviour
    // lv_label_set_text(label, indicator_name);
    // lv_obj_set_align(label, LV_ALIGN_CENTER);
    // lv_obj_add_event_cb(button, event_screen_base_update_mods, EVENT_MOD_CHANGE, NULL);
    lv_obj_add_event_cb(button, event_screen_base_update_mods, LV_EVENT_ALL, NULL);
    // lv_obj_add_flag(button, LV_OBJ_FLAG_CHECKABLE);

    // position and width
    lv_obj_set_width(button, 45);
    lv_obj_set_height(button, 30);
    // lv_obj_set_x(button, x);
    // lv_obj_set_y(button, y);
}

void event_screen_pointer_sniping_toggle(lv_event_t *e) {}
void event_screen_pointer_scroll_toggle(lv_event_t *e) {}

void event_screen_base_update_mods(lv_event_t *e) {
    // todo implement new / old event storage
    // todo test if new screen, then re-draw everything...
    // if(layer_state == 0){ // todo replace with enum from keymap.c
    // TODO test if GUI is different... for now trigger a test re-draw
    // lv_obj_t * btn = lv_event_get_target(e); // get target
    // lv_obj_t * label = lv_obj_get_child(btn, 0); // get first child (the label)
    // lv_label_set_text(label, "TEST 2");
    // }
}

// void ui_layer_change(lv_event_t *e) {
//     lv_event_code_t event_code = lv_event_get_code(e);
//     if (event_code == (uint8_t)EVENT_LAYER_CHANGE) {
//         int layer = get_highest_layer(layer_state); // todo test that it's smaller than the max size (compare to size of layer_strings)
//         lv_label_set_text(ui_label_layer_name_base, ui_layer_strings[layer]);
//     }
// }

void housekeeping_task_display(void) {
    mods = get_mods();

    // TODO use enum from keymap.c instead of hard coded layer numbers
    uint8_t layer = get_highest_layer(layer_state);
    // if (layer != prev_layer) {
    //     switch (layer) {
    //         case 0:
    //         default:
    //             lv_disp_load_scr(ui_screen_base);
    //             break;
    //         case 4:
    //             lv_disp_load_scr(ui_screen_pointer);
    //             break;
    //     }
    // }

    // TODO use enum from keymap.c instead of hard coded layer numbers
    // TODO move to specific function

    // TODO maintenance... we are using only one screen at the moment.
    switch (layer) {
        case 0:
        default:
            lv_label_set_text(ui_label_layer_name_base, "Layer Base");
            break;
        case 3:
            lv_label_set_text(ui_label_layer_name_base, "Layer RGB");
            break;
        case 4:
            lv_label_set_text(ui_label_layer_name_base, "Layer Pointer");
            break;
    }

    housekeeping_task_screen_base();
    housekeeping_task_screen_rgb();
    housekeeping_task_screen_pointer();

    last_mods  = mods;
    prev_layer = layer;
}

// TODO only redraw if rgb changed
void housekeeping_task_screen_rgb(void) {}

void housekeeping_task_screen_base(void) {
    mods = get_mods();
    if ((mods & MOD_MASK_SHIFT) != (last_mods & MOD_MASK_SHIFT)) {
        if ((mods & MOD_MASK_SHIFT)) {
            lv_event_send(ui_button_mod_shift, LV_EVENT_PRESSED, NULL);
        } else {
            lv_event_send(ui_button_mod_shift, LV_EVENT_RELEASED, NULL);
        }
    }
    if ((mods & MOD_MASK_ALT) != (last_mods & MOD_MASK_ALT)) {
        if ((mods & MOD_MASK_ALT)) {
            lv_event_send(ui_button_mod_alt, LV_EVENT_PRESSING, NULL);
        } else {
            lv_event_send(ui_button_mod_alt, LV_EVENT_RELEASED, NULL);
        }
    }

    if ((mods & MOD_MASK_CTRL) != (last_mods & MOD_MASK_CTRL)) {
        if ((mods & MOD_MASK_CTRL)) {
            lv_event_send(ui_button_mod_control, LV_EVENT_PRESSED, NULL);
        } else {
            lv_event_send(ui_button_mod_control, LV_EVENT_RELEASED, NULL);
        }
    }

    if ((mods & MOD_MASK_GUI) != (last_mods & MOD_MASK_GUI)) {
        if ((mods & MOD_MASK_GUI)) {
            lv_event_send(ui_button_mod_gui, LV_EVENT_PRESSED, NULL);
        } else {
            lv_event_send(ui_button_mod_gui, LV_EVENT_RELEASED, NULL);
        }
    }
}

// TODO only redraw if DPI / sniping DPI changed
// TODO switch to event-based when dpi changed
void housekeeping_task_screen_pointer(void) {
    // TODO dynamically get max DPI, instead of using hardcoded values
    static const uint16_t rel_max_dpi = 200 * 16;
    float                 rel         = (float)((dilemma_get_pointer_default_dpi() + 200 - 400)) * 100 / rel_max_dpi;
    lv_bar_set_value(ui_bar_dpi, (uint16_t)rel, LV_ANIM_OFF);

    char dpi[50];
    sprintf(dpi, "DPI: %u", (uint16_t)dilemma_get_pointer_default_dpi());
    lv_label_set_text(ui_label_dpi, dpi);

    static const uint16_t rel_max_s_dpi = 100 * 4;
    rel                                 = (float)((dilemma_get_pointer_sniping_dpi() + 100 - 200)) * 100 / rel_max_s_dpi;
    lv_bar_set_value(ui_bar_s_dpi, (uint16_t)rel, LV_ANIM_OFF);

    char s_dpi[50];
    sprintf(s_dpi, "Sniper DPI: %u", (uint16_t)dilemma_get_pointer_sniping_dpi());
    lv_label_set_text(ui_label_s_dpi, s_dpi);

    const bool sniping = dilemma_get_pointer_sniping_enabled();
    if (sniping != last_sniping) {
        if (sniping) {
            lv_obj_add_state(ui_switch_sniping, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(ui_switch_sniping, LV_STATE_CHECKED);
        }
    }
    last_sniping = sniping;

    const bool scrolling = dilemma_get_pointer_dragscroll_enabled();
    if (scrolling != last_scrolling) {
        if (scrolling) {
            lv_obj_add_state(ui_switch_scroll, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(ui_switch_scroll, LV_STATE_CHECKED);
        }
    }
    last_scrolling = scrolling;
}

bool process_records_display(uint16_t keycode, keyrecord_t *record) {
    // switch (keycode) {
    //     case KC_Q: // test
    //         // ui_screen_base_update_mods();
    //         // lv_event_send(ui_button_mod_gui, EVENT_MOD_CHANGE, NULL);
    //         // lv_event_send(ui_button_mod_gui, LV_EVENT_CLICKED, NULL);
    //         if (record->event.pressed) {
    //             lv_event_send(ui_button_mod_gui, LV_EVENT_PRESSING, NULL);
    //         } else {
    //             lv_event_send(ui_button_mod_gui, LV_EVENT_RELEASED, NULL);
    //         }
    //         break;
    // }

    return true;
}