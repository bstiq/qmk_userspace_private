#include QMK_KEYBOARD_H
#include "introspection.h"

#include "lcd.h"
#include "config.h"
#include "ui_elements.h"
#include "quantum.h"
#include <stdio.h>

#include "qp.h"
#include "qp_comms.h"
#include "qp_st77xx_opcodes.h"
#include "qp_surface.h"
#include "color.h"
#include "transactions.h"

lv_obj_t *ui_screen_base;

static mod_button_pair_t mod_buttons[4];
static mod_button_pair_t mouse_buttons[2];

lv_obj_t *ui_label_layer;
lv_obj_t *ui_button_layer;
lv_obj_t *ui_label_dpi;
lv_obj_t *ui_label_dpi_number;
lv_obj_t *ui_bar_dpi;
lv_obj_t *ui_label_s_dpi;
lv_obj_t *ui_bar_s_dpi;
lv_obj_t *ui_label_s_dpi_number;
lv_obj_t *ui_image_scroll;
lv_obj_t *ui_label_rgb;
lv_obj_t *ui_bar_rgb;
lv_obj_t *ui_label_rgb_effect;
lv_obj_t *ui_label_rgb_number;

lv_obj_t *ui_line_1;
lv_obj_t *ui_line_2;

enum ui_user_events {
    EVENT_LAYER_CHANGE = 0,
    EVENT_MOD_CHANGE,
    EVENT_LAST_EVENT,
};

static dilemma_status_t dilemma_lcd_status_prev = {0};
static dilemma_status_t dilemma_lcd_status      = {0};

// TODO keep this variable contained in theme.c
extern ui_styles_t ui_styles;

const char *ui_layer_strings[] = {"BASE", "FUNCTION", "NAV", "MED/RGB", "POINTER", "NUM", "SYM"};

painter_device_t        lcd;
static painter_device_t surface;
// Buffer required for a 240x280 16bpp surface:
static uint8_t surface_buffer[SURFACE_REQUIRED_BUFFER_BYTE_SIZE(LCD_WIDTH, LCD_HEIGHT, 16)];

void init_display(void) {
    // Display timeout
    wait_ms(LCD_WAIT_TIME);

    lcd = qp_st7789_make_spi_device(LCD_WIDTH, LCD_HEIGHT, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, LCD_SPI_DIVISOR, SPI_MODE);
    qp_init(lcd, LCD_ROTATION);

    surface = qp_make_rgb565_surface(LCD_WIDTH, LCD_HEIGHT, surface_buffer);
    qp_init(surface, LCD_ROTATION);

    // Display offset
    qp_set_viewport_offsets(lcd, LCD_OFFSET_X, LCD_OFFSET_Y);

    qp_lvgl_attach(lcd);

    // Power on display, fill with black
    qp_power(lcd, 1);
    qp_rect(lcd, 0, 0, 300, 300, HSV_BLACK, 1);
    qp_flush(lcd);
    ui_screen_base = lv_obj_create(NULL);

    init_themes();
    init_styles();

    lv_obj_t *cont = lv_obj_create(ui_screen_base);
    lv_obj_set_size(cont, LCD_WIDTH, LCD_HEIGHT);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    // TODO move this to theme.c, in eg. create_container
    lv_obj_add_style(cont, &ui_styles.flex_container, 0);

    ui_button_layer = lv_btn_create(cont);
    // TODO move this to theme.c, in eg. create_laer_name
    lv_obj_add_style(ui_button_layer, &ui_styles.layer_name, 0);
    lv_obj_set_height(ui_button_layer, 33);
    lv_obj_set_flex_grow(ui_button_layer, 1); // take all remaining space in line

    // layer title
    ui_label_layer = lv_label_create(ui_button_layer);
    ui_init_layer_name(ui_label_layer);
    lv_label_set_text(ui_label_layer, "LAYER: BASE");
    lv_obj_center(ui_label_layer);

    // mod buttons: SHIFT, ALT, CTRL, GUI
    mod_buttons[0] = ui_create_mod_button(cont, "SHFT", true, MOD_MASK_SHIFT);
    mod_buttons[1] = ui_create_mod_button(cont, "ALT", false, MOD_MASK_ALT);
    mod_buttons[2] = ui_create_mod_button(cont, "CTRL", false, MOD_MASK_CTRL);
    mod_buttons[3] = ui_create_mod_button(cont, "GUI", false, MOD_MASK_GUI);

    // line separator
    ui_line_1 = ui_create_line_separator(cont, 1, 3);

    // display base layer screen upon init
    lv_disp_load_scr(ui_screen_base);

    // mouse special buttons
    mouse_buttons[0] = ui_create_mod_button(cont, "SNIPE", true, 0);
    mouse_buttons[1] = ui_create_mod_button(cont, "SCROLL", false, 0);

    // sniping DPI widgets
    ui_label_s_dpi        = ui_create_secondary_text(cont, "SNIPE DPI", true, 4);
    ui_bar_s_dpi          = ui_create_progress_bar(cont, 4);
    ui_label_s_dpi_number = ui_create_number_label(cont, 2);

    // regular DPI widgets
    ui_label_dpi        = ui_create_secondary_text(cont, "DPI", true, 2);
    ui_bar_dpi          = ui_create_progress_bar(cont, 6);
    ui_label_dpi_number = ui_create_number_label(cont, 2);

    // line separator
    ui_line_2 = ui_create_line_separator(cont, 1, 3);

    // rgb widgets
    ui_label_rgb        = ui_create_secondary_text(cont, "RGB", true, 2);
    ui_bar_rgb          = ui_create_progress_bar(cont, 6);
    ui_label_rgb_number = ui_create_number_label(cont, 2);

    ui_label_rgb_effect = ui_create_secondary_text(cont, "effect...", true, 1);

    // theme and backgrounds
    lv_disp_t  *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(BK_PALETTE), lv_palette_main(BK_PALETTE), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    lv_obj_set_style_bg_color(cont, lv_color_black(), LV_PART_MAIN);
}

void keyboard_post_init_lcd(void) {
    update_dilemma_status();
    load_dilemma_theme_config_from_eeprom();

    // copy only the relevant information from eeprom into local config
    dilemma_lcd_status.current_theme_id = get_current_theme_id();

    // important when connecting both sides with a different-than-standard config
    // otherwise master tries to send an RPC message when left is not ready yet (lcd init takes time...)
    // dilemma_lcd_status_prev = dilemma_lcd_status;

    // sync mouse data across halves
    transaction_register_rpc(RPC_ID_MOUSE_SYNC, mouse_info_sync_handler);

    // TODO: the load theme config needs to be done BEFORE the display init. How do we solve this?
    if (is_keyboard_left()) {
        init_display();
        refresh_lcd_info(true);
    }
}

// TODO get colors based on real layer colors, instead of hardcoding them
void update_theme_color(bool force) {
    if (get_current_theme().change_colors_on_layer_change) {
        if (dilemma_lcd_status.layer != dilemma_lcd_status_prev.layer || force) {
            HSV hsv;
            switch (dilemma_lcd_status.layer) {
                case 0:
                default:
                    hsv.h = 218;
                    hsv.s = 70;
                    hsv.v = 93;
                    break;
                case 1:
                    hsv.h = 250;
                    hsv.s = 100;
                    hsv.v = 80;
                    break;
                case 2:
                    hsv.h = 35;
                    hsv.s = 100;
                    hsv.v = 80;
                    break;
                case 3:
                    hsv.h = 195;
                    hsv.s = 30;
                    hsv.v = 80;
                    break;
            }

            change_style_colors(hsv);
        }
    }
}

void refresh_lcd_info(bool force) {
    if (is_keyboard_left()) {
        update_layer_name(force);
        update_mods(force);
        update_rgb_info(force);
        update_mouse_info(force);
        update_theme_color(force);
    }
}

void housekeeping_task_lcd(void) {
    if (is_keyboard_master()) {
        update_dilemma_status();
        // if the keyboard is left, nothing to do - just refresh the screen
        if (is_keyboard_left()) {
            refresh_lcd_info(false);
        }
        // if the keyboard is right, we need to send the sync info over to the left side
        // saving the theme id to eeprom has already been done in process_record
        else {
            bool            needs_sync   = false;
            static bool     needs_resync = false;
            static uint32_t last_sync    = 0;
            // // Check if the state values are different.
            if (memcmp(&dilemma_lcd_status, &dilemma_lcd_status_prev, sizeof(dilemma_lcd_status))) {
                needs_sync = true;
            }
            // check if a previous sync has failed
            if (needs_resync) {
                // we only want to retry syncing after a set amount of time
                if (timer_elapsed32(last_sync) > 200) {
                    needs_sync = true;
                }
            }
            // Perform the sync if requested.
            if (needs_sync) {
                // try to sync, and store the results in needs_resync
                needs_resync = !(transaction_rpc_send(RPC_ID_MOUSE_SYNC, sizeof(dilemma_lcd_status), &dilemma_lcd_status));
                last_sync    = timer_read32();
            }
        }

        dilemma_lcd_status_prev = dilemma_lcd_status;
    }
}

void update_layer_name(bool force) {
    if (dilemma_lcd_status.layer != dilemma_lcd_status_prev.layer || force) {
        switch (dilemma_lcd_status.layer) {
            case 0:
            default:
                lv_label_set_text(ui_label_layer, "LAYER: BASE");
                break;
            case 1:
                lv_label_set_text(ui_label_layer, "LAYER: LOWER");
                break;
            case 2:
                lv_label_set_text(ui_label_layer, "LAYER: RAISE");
                break;
            case 3:
                lv_label_set_text(ui_label_layer, "LAYER: MOUSE");
                break;
        }
    }
}

void update_dilemma_status(void) {
    dilemma_lcd_status.mods            = get_mods();
    dilemma_lcd_status.layer           = get_highest_layer(layer_state);
    dilemma_lcd_status.sniping         = dilemma_get_pointer_sniping_enabled();
    dilemma_lcd_status.dpi             = dilemma_get_pointer_default_dpi();
    dilemma_lcd_status.s_dpi           = dilemma_get_pointer_sniping_dpi();
    dilemma_lcd_status.scrolling       = dilemma_get_pointer_dragscroll_enabled();
    dilemma_lcd_status.rgb_enabled     = rgb_matrix_is_enabled();
    dilemma_lcd_status.rgb_effect_mode = rgb_matrix_get_mode();
    dilemma_lcd_status.rgb_val         = rgb_matrix_get_val();
}

// TODO remove force, should not be necessary anymore if we do a sync // retry on initial connection
void update_mods(bool force) {
    int i = 0;
    for (i = 0; i < (sizeof(mod_buttons) / sizeof(mod_button_pair_t)); i++) {
        if ((dilemma_lcd_status.mods & mod_buttons[i].mod_mask) != (dilemma_lcd_status_prev.mods & mod_buttons[i].mod_mask) || force) {
            if ((dilemma_lcd_status.mods & mod_buttons[i].mod_mask)) {
                lv_event_send(mod_buttons[i].button, LV_EVENT_PRESSED, NULL);
            } else {
                lv_event_send(mod_buttons[i].button, LV_EVENT_RELEASED, NULL);
            }
        }
    }
}

// TODO remove force, should not be necessary anymore if we do a sync // retry on initial connection
void update_rgb_info(bool force) {
    const bool rgb_change = (dilemma_lcd_status.rgb_enabled != dilemma_lcd_status_prev.rgb_enabled);

    if (!dilemma_lcd_status.rgb_enabled) {
        if (rgb_change || force) {
            lv_label_set_text(ui_label_rgb_number, "Off");
            lv_bar_set_value(ui_bar_rgb, 0, LV_ANIM_OFF);
            lv_label_set_text(ui_label_rgb_effect, "");
        }
    } else {
        if ((rgb_change) || (dilemma_lcd_status.rgb_val != dilemma_lcd_status_prev.rgb_val) || force) {
            char rgbval[50];
            sprintf(rgbval, "%u", dilemma_lcd_status.rgb_val);
            lv_label_set_text(ui_label_rgb_number, rgbval);
            float rel = (float)(dilemma_lcd_status.rgb_val) * 100 / 156;
            lv_bar_set_value(ui_bar_rgb, (uint16_t)rel, LV_ANIM_OFF);
        }
        if ((rgb_change) || (dilemma_lcd_status.rgb_effect_mode != dilemma_lcd_status_prev.rgb_effect_mode) || force) {
            const char *effect_name = rgb_matrix_get_effect_name();
            lv_label_set_text(ui_label_rgb_effect, effect_name);
        }
    }
}

// TODO remove force, should not be necessary anymore if we do a sync // retry on initial connection
void update_mouse_info(bool force) {
    // TODO dynamically get max DPI, instead of using hardcoded values
    if (dilemma_lcd_status.dpi != dilemma_lcd_status_prev.dpi || force) {
        static const uint16_t rel_max_dpi = 200 * 16;
        const float           rel         = (float)((dilemma_lcd_status.dpi + 200 - 400)) * 100 / rel_max_dpi;
        lv_bar_set_value(ui_bar_dpi, (uint16_t)rel, LV_ANIM_OFF);

        char c_dpi[50];
        sprintf(c_dpi, "%u", (uint16_t)dilemma_lcd_status.dpi);
        lv_label_set_text(ui_label_dpi_number, c_dpi);
    }

    if (dilemma_lcd_status.s_dpi != dilemma_lcd_status_prev.s_dpi || force) {
        char                  c_s_dpi[50];
        static const uint16_t rel_max_s_dpi = 100 * 4;
        const float           rel           = (float)((dilemma_lcd_status.s_dpi + 100 - 200)) * 100 / rel_max_s_dpi;
        lv_bar_set_value(ui_bar_s_dpi, (uint16_t)rel, LV_ANIM_OFF);
        sprintf(c_s_dpi, "%u", (uint16_t)dilemma_lcd_status.s_dpi);
        lv_label_set_text(ui_label_s_dpi_number, c_s_dpi);
    }

    if (dilemma_lcd_status.sniping != dilemma_lcd_status_prev.sniping || force) {
        if (dilemma_lcd_status.sniping) {
            lv_event_send(mouse_buttons[0].button, LV_EVENT_PRESSED, NULL);
        } else {
            lv_event_send(mouse_buttons[0].button, LV_EVENT_RELEASED, NULL);
        }
    }

    if (dilemma_lcd_status.scrolling != dilemma_lcd_status_prev.scrolling || force) {
        if (dilemma_lcd_status.scrolling) {
            lv_event_send(mouse_buttons[1].button, LV_EVENT_PRESSED, NULL);
        } else {
            lv_event_send(mouse_buttons[1].button, LV_EVENT_RELEASED, NULL);
        }
    }
}

bool process_record_lcd(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LCD_MODULE_CHANGE_THEME:
            if (record->event.pressed) {
                if (is_keyboard_master()) {
                    cycle_theme_and_save_in_eeprom();
                    // no need to update the secondary side: it will be done in housekeeping
                    // TODO explain this: save the config theme id into the local "no eeprom" status
                    dilemma_lcd_status.current_theme_id = get_current_theme_id();
                }
                // housekeeping_task_lcd();
                // qp_flush(lcd);
            }
            break;
    }
    return true;
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

/*
called by right side, executed by left side (where the screen is)
we do not store the updated config in eeprom, this is done by master in cycle_theme
if later we would like to do that, first we need to sync halves in the dilemma code with kb eeprom, and then implement
theme sync here with user eeprom
*/
void mouse_info_sync_handler(uint8_t initiator2target_buffer_size, const void *initiator2target_buffer, uint8_t target2initiator_buffer_size, void *target2initiator_buffer) {
    if (is_keyboard_left()) {
        if (initiator2target_buffer_size == sizeof(dilemma_lcd_status)) {
            dilemma_lcd_status_prev = dilemma_lcd_status;
            dilemma_lcd_status      = *(const dilemma_status_t *)initiator2target_buffer;

            if (dilemma_lcd_status_prev.current_theme_id != dilemma_lcd_status.current_theme_id) {
                update_styles_from_current_theme_if_left();
            }
            refresh_lcd_info(false);
        }
    }
}