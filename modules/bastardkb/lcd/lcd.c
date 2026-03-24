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
#include "color.h"
#include "transactions.h"

// TODO manage this better
#include "screens/base/screen_base.h"
#include "screens/base/screen_pomodoro.h"

dilemma_status_t dilemma_lcd_status_prev = {0};
dilemma_status_t dilemma_lcd_status      = {0};
painter_device_t lcd;

lcd_module_t lcd_module_base = {
    .init_module                                      = &init_screen_base,
    .load_custom_theme_elements                       = &load_themes,
    .load_module                                      = &load_module_screen_base,
    .update_custom_elements_styles_from_current_theme = &update_styles_from_current_theme,
    .refresh_module                                   = &refresh_screen_base,
};

lcd_module_t lcd_module_pomodoro = {
    .init_module                                      = &init_screen_pomodoro,
    .load_custom_theme_elements                       = NULL,
    .update_custom_elements_styles_from_current_theme = NULL,
    .refresh_module                                   = &refresh_screen_pomodoro,
};

uint8_t selected_module = 0;

lcd_module_t *lcd_modules[] = {&lcd_module_base, &lcd_module_pomodoro};

void init_display(void) {
    // Display timeout
    wait_ms(LCD_WAIT_TIME);

    lcd = qp_st7789_make_spi_device(LCD_WIDTH, LCD_HEIGHT, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, LCD_SPI_DIVISOR, SPI_MODE);
    qp_init(lcd, LCD_ROTATION);

    // Display offset
    qp_set_viewport_offsets(lcd, LCD_OFFSET_X, LCD_OFFSET_Y);

    qp_lvgl_attach(lcd);

    // Power on display, fill with black
    qp_power(lcd, 1);
    qp_rect(lcd, 0, 0, 300, 300, HSV_BLACK, 1);
    qp_flush(lcd);

    // TODO move the load themes and init styles into the custom structure
    load_themes();
    init_styles();

    for (int i = 0; i < sizeof(lcd_modules) / sizeof(lcd_module_t *); i++) {
        if (lcd_modules[i]->init_module != NULL) {
            lcd_modules[i]->init_module();
        }
    }
    lcd_modules[selected_module]->load_module();

    // lv_obj_t *ui_screen_pomodoro = init_screen_pomodoro();
    // lv_obj_t *ui_screen_pomodoro = init_screen_pomodoro();

    // display base layer screen upon init
    // TODO is this necessary here? can we move it to a spot that makes more sense?
    // lv_disp_load_scr(ui_screen_pomodoro);
}

void keyboard_post_init_lcd(void) {
    load_dilemma_theme_config_from_eeprom();

    // copy only the relevant information from eeprom into local config
    // todo move this to update_dilemma_status?
    dilemma_lcd_status.current_theme_id = get_current_theme_id();

    if (is_keyboard_left()) {
        init_display();
    }

    // register rpc mouse data syncing
    transaction_register_rpc(RPC_ID_MOUSE_SYNC, mouse_info_sync_handler);
}

// TODO get colors based on real layer colors, instead of hardcoding them
void update_theme_color(void) {
    // static bool first_display = true;
    // if (get_current_theme().change_colors_on_layer_change) {
    //     if (dilemma_lcd_status.layer != dilemma_lcd_status_prev.layer || first_display) {
    //         HSV hsv;
    //         switch (dilemma_lcd_status.layer) {
    //             case 0:
    //             default:
    //                 hsv.h = 218;
    //                 hsv.s = 70;
    //                 hsv.v = 93;
    //                 break;
    //             case 1:
    //                 hsv.h = 250;
    //                 hsv.s = 100;
    //                 hsv.v = 80;
    //                 break;
    //             case 2:
    //                 hsv.h = 35;
    //                 hsv.s = 100;
    //                 hsv.v = 80;
    //                 break;
    //             case 3:
    //                 hsv.h = 195;
    //                 hsv.s = 30;
    //                 hsv.v = 80;
    //                 break;
    //         }

    //         change_style_colors(hsv);
    //     }
    // }
    // first_display = false;
}

void refresh_lcd_info(void) {
    update_theme_color();
    if (lcd_modules[selected_module]->refresh_module != NULL) {
        lcd_modules[selected_module]->refresh_module();
    }
}

void housekeeping_task_lcd(void) {
    if (is_keyboard_master()) {
        update_dilemma_status();
        // if the keyboard is left, nothing to do - just refresh the screen
        if (is_keyboard_left()) {
            refresh_lcd_info();
        }
        // if the keyboard is right, we need to send the sync info over to the left side
        // saving the theme id to eeprom has already been done in process_record
        else {
            bool            needs_sync   = false;
            static bool     needs_resync = true; // perform an initial first sync
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
            // perform the sync if requested
            if (needs_sync) {
                // try to sync, if it fails we will retry in the next housekeeping loop
                if (transaction_rpc_send(RPC_ID_MOUSE_SYNC, sizeof(dilemma_lcd_status), &dilemma_lcd_status) == false) {
                    needs_resync = true;
                }
                last_sync = timer_read32();
            }
        }

        dilemma_lcd_status_prev = dilemma_lcd_status;
    }
}

bool process_record_lcd(uint16_t keycode, keyrecord_t *record) {
    // TODO call process_records of current screen
    // for now we call the pomodoro one
    process_record_screen_pomodoro(keycode, record);
    switch (keycode) {
        case LCD_MODULE_CHANGE_THEME:
            if (record->event.pressed) {
                if (is_keyboard_master()) {
                    cycle_theme_and_save_in_eeprom();
                    // if the keyboard is left, then we directly update the styles
                    // if the keyboard is right, we need to send the sync info over to the left side
                    // that will be done in housekeeping
                    if (is_keyboard_left()) {
                        update_styles_from_current_theme();
                    }
                    // TODO this is done in cycle_theme_and_save_in_eeprom, we can remove it
                    dilemma_lcd_status.current_theme_id = get_current_theme_id();
                }
            }
            break;
    }
    return true;
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
                set_current_theme_id(dilemma_lcd_status.current_theme_id);
                update_styles_from_current_theme();
            }
            refresh_lcd_info();
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