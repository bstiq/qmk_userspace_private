#include QMK_KEYBOARD_H
#include "introspection.h"
#include "lvgl.h"
#include <ctype.h>

#include "screen_base.h"
#include "lcd.h"
#include "ui_elements.h"

obj_event_array_t event_with_objects_array;

// TODO move this into a getter, isolate into lcd.c
extern dilemma_status_t dilemma_lcd_status;
extern dilemma_status_t dilemma_lcd_status_prev;

void init_obj_event_array(obj_event_array_t *a) {
    a->used            = 0;
    a->size            = 0;
    a->amount_elements = 0;
}

void add_obj_event_array(obj_event_array_t *a, obj_update_t element) {
    if (a->used == a->size) {
        a->size  = (a->size * 3) / 2 + 8;
        a->array = realloc(a->array, a->size * sizeof(obj_update_t));
    }
    a->array[a->used++] = element;
    a->amount_elements++;
}

// TODO isolate the ui_screen_base into this folder, and instead return a pointer to it with this function?
void init_screen_base(void) {
    ui_screen_base = lv_obj_create(NULL);
    init_obj_event_array(&event_with_objects_array);

    lv_obj_t *cont = lv_obj_create(ui_screen_base);
    lv_obj_set_size(cont, LCD_WIDTH, LCD_HEIGHT);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    // TODO move this to theme.c, in eg. create_container
    ui_styles_t *styles = get_current_ui_styles();
    lv_obj_add_style(cont, &styles->flex_container, 0);

    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_layer_label(cont),
                                                       &update_layer_name,
                                                   });

    // mod buttons: SHIFT, ALT, CTRL, GUI
    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_mod_button(cont, "SHFT", true, MOD_MASK_SHIFT),
                                                       &update_mod_shift,
                                                   });
    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_mod_button(cont, "ALT", false, MOD_MASK_ALT),
                                                       &update_mod_alt,
                                                   });
    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_mod_button(cont, "CTRL", false, MOD_MASK_CTRL),
                                                       &update_mod_ctrl,
                                                   });
    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_mod_button(cont, "GUI", false, MOD_MASK_GUI),
                                                       &update_mod_gui,
                                                   });
    ui_create_line_separator(cont, 1, 3);

    // mouse special buttons
    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_mod_button(cont, "SCROLL", true, 0),
                                                       &update_mod_scroll,
                                                   });
    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_mod_button(cont, "SNIPE", false, 0),
                                                       &update_mod_snipe,
                                                   });

    // sniping DPI widgets
    ui_create_secondary_text(cont, "SNIPE DPI", true, 4);
    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_progress_bar(cont, 4),
                                                       &update_mod_snipe_dpi_bar,
                                                   });
    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_number_label(cont, 2),
                                                       &update_mod_snipe_dpi_number,
                                                   });

    // regular DPI widgets
    ui_create_secondary_text(cont, "DPI", true, 2);
    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_progress_bar(cont, 6),
                                                       &update_mod_dpi_bar,
                                                   });
    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_number_label(cont, 2),
                                                       &update_mod_dpi_number,
                                                   });

    // line separator
    ui_create_line_separator(cont, 1, 3);

    // rgb widgets
    ui_create_secondary_text(cont, "RGB", true, 2);

    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_progress_bar(cont, 6),
                                                       &update_rgb_bar,
                                                   });
    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_number_label(cont, 2),
                                                       &update_rgb_value,
                                                   });

    add_obj_event_array(&event_with_objects_array, (obj_update_t){
                                                       ui_create_secondary_text(cont, "effect...", true, 1),
                                                       &update_rgb_effect,
                                                   });
}


const char *rgb_matrix_get_effect_name(void) {
    // thank you drashna!
    static char    buf[32]     = {0};
    static uint8_t last_effect = 0;
    if (last_effect != rgb_matrix_get_mode()) {
        last_effect = rgb_matrix_get_mode();
        snprintf(buf, sizeof(buf), "%s", rgb_matrix_get_mode_name(rgb_matrix_get_mode()));
        for (uint8_t i = 1; i < sizeof(buf); ++i) {
            if (buf[i] == 0)
                break;
            else if (buf[i] == '_')
                buf[i] = ' ';
            else if (buf[i - 1] == ' ')
                buf[i] = toupper(buf[i]);
            else if (buf[i - 1] != ' ')
                buf[i] = tolower(buf[i]);
        }
    }
    return buf;
}

// TODO add dilemma layers, not only MAX
void update_layer_name(lv_obj_t *obj) {
    if (dilemma_lcd_status.layer != dilemma_lcd_status_prev.layer) {
        switch (dilemma_lcd_status.layer) {
            case 0:
            default:
                lv_label_set_text(obj, "LAYER: BASE");
                break;
            case 1:
                lv_label_set_text(obj, "LAYER: LOWER");
                break;
            case 2:
                lv_label_set_text(obj, "LAYER: RAISE");
                break;
            case 3:
                lv_label_set_text(obj, "LAYER: MOUSE");
                break;
        }
    }
}

void update_mod_shift(lv_obj_t *obj) {
    update_mod_xx(obj, MOD_MASK_SHIFT);
}
void update_mod_ctrl(lv_obj_t *obj) {
    update_mod_xx(obj, MOD_MASK_CTRL);
}
void update_mod_alt(lv_obj_t *obj) {
    update_mod_xx(obj, MOD_MASK_ALT);
}
void update_mod_gui(lv_obj_t *obj) {
    update_mod_xx(obj, MOD_MASK_GUI);
}

void update_mod_xx(lv_obj_t *obj, uint8_t mod_mask) {
    if ((dilemma_lcd_status.mods & mod_mask) != (dilemma_lcd_status_prev.mods & mod_mask)) {
        if ((dilemma_lcd_status.mods & mod_mask)) {
            lv_event_send(obj, LV_EVENT_PRESSED, NULL);
        } else {
            lv_event_send(obj, LV_EVENT_RELEASED, NULL);
        }
    }
}

void update_rgb_effect(lv_obj_t *obj) {
    const bool rgb_change = (dilemma_lcd_status.rgb_enabled != dilemma_lcd_status_prev.rgb_enabled);

    if (!dilemma_lcd_status.rgb_enabled) {
        if (rgb_change) {
            lv_label_set_text(obj, "");
        }
    } else {
        if ((rgb_change) || (dilemma_lcd_status.rgb_effect_mode != dilemma_lcd_status_prev.rgb_effect_mode)) {
            const char *effect_name = rgb_matrix_get_effect_name();
            lv_label_set_text(obj, effect_name);
        }
    }
}

void update_rgb_value(lv_obj_t *obj) {
    const bool rgb_change = (dilemma_lcd_status.rgb_enabled != dilemma_lcd_status_prev.rgb_enabled);

    if (!dilemma_lcd_status.rgb_enabled) {
        if (rgb_change) {
            lv_label_set_text(obj, "Off");
        }
    } else {
        if ((rgb_change) || (dilemma_lcd_status.rgb_val != dilemma_lcd_status_prev.rgb_val)) {
            char rgbval[50];
            sprintf(rgbval, "%u", dilemma_lcd_status.rgb_val);
            lv_label_set_text(obj, rgbval);
        }
    }
}

void update_rgb_bar(lv_obj_t *obj) {
    const bool rgb_change = (dilemma_lcd_status.rgb_enabled != dilemma_lcd_status_prev.rgb_enabled);
    if (!dilemma_lcd_status.rgb_enabled) {
        if (rgb_change) {
            // lv_label_set_text(ui_label_rgb_number, "Off"); // test
            lv_bar_set_value(obj, 0, LV_ANIM_OFF);
        }
    } else {
        if ((rgb_change) || (dilemma_lcd_status.rgb_val != dilemma_lcd_status_prev.rgb_val)) {
            float rel = (float)(dilemma_lcd_status.rgb_val) * 100 / 156;
            lv_bar_set_value(obj, (uint16_t)rel, LV_ANIM_OFF);
        }
    }
}

void update_mod_scroll(lv_obj_t *obj) {
    if (dilemma_lcd_status.scrolling != dilemma_lcd_status_prev.scrolling) {
        if (dilemma_lcd_status.scrolling) {
            lv_event_send(obj, LV_EVENT_PRESSED, NULL);
        } else {
            lv_event_send(obj, LV_EVENT_RELEASED, NULL);
        }
    }
}

void update_mod_snipe(lv_obj_t *obj) {
    if (dilemma_lcd_status.sniping != dilemma_lcd_status_prev.sniping) {
        if (dilemma_lcd_status.sniping) {
            lv_event_send(obj, LV_EVENT_PRESSED, NULL);
        } else {
            lv_event_send(obj, LV_EVENT_RELEASED, NULL);
        }
    }
}

// TODO dynamically get max DPI, instead of using hardcoded values
void update_mod_snipe_dpi_number(lv_obj_t *obj) {
    if (dilemma_lcd_status.s_dpi != dilemma_lcd_status_prev.s_dpi) {
        char c_s_dpi[50];
        sprintf(c_s_dpi, "%u", (uint16_t)dilemma_lcd_status.s_dpi);
        lv_label_set_text(obj, c_s_dpi);
    }
}

// TODO dynamically get max DPI, instead of using hardcoded values
void update_mod_snipe_dpi_bar(lv_obj_t *obj) {
    if (dilemma_lcd_status.s_dpi != dilemma_lcd_status_prev.s_dpi) {
        static const uint16_t rel_max_s_dpi = 100 * 4;
        const float           rel           = (float)((dilemma_lcd_status.s_dpi + 100 - 200)) * 100 / rel_max_s_dpi;
        lv_bar_set_value(obj, (uint16_t)rel, LV_ANIM_OFF);
    }
}

// TODO dynamically get max DPI, instead of using hardcoded values
void update_mod_dpi_number(lv_obj_t *obj) {
    if (dilemma_lcd_status.dpi != dilemma_lcd_status_prev.dpi) {
        char c_dpi[50];
        sprintf(c_dpi, "%u", (uint16_t)dilemma_lcd_status.dpi);
        lv_label_set_text(obj, c_dpi);
    }
}

// TODO dynamically get max DPI, instead of using hardcoded values
void update_mod_dpi_bar(lv_obj_t *obj) {
    if (dilemma_lcd_status.dpi != dilemma_lcd_status_prev.dpi) {
        static const uint16_t rel_max_dpi = 200 * 16;
        const float           rel         = (float)((dilemma_lcd_status.dpi + 200 - 400)) * 100 / rel_max_dpi;
        lv_bar_set_value(obj, (uint16_t)rel, LV_ANIM_OFF);
    }
}

void refresh_screen_base(void) {
    int i = 0;
    for (i = 0; i < event_with_objects_array.amount_elements; i++) {
        lv_obj_t *obj = event_with_objects_array.array[i].obj;
        if (obj && event_with_objects_array.array[i].update_function) event_with_objects_array.array[i].update_function(obj);
    }
}