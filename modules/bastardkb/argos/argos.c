
// Copyright 2026 Quentin LEBASTARD <bstkbd@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

#include "argos.h"
#include "argos_combo.h"
#include "argos_tapdance.h"
#include "quantum.h"
#include "via.h"
#include "raw_hid.h"
#include "eeprom.h"
#include <string.h>
#include "print.h"
#include "version.h"
#include "eeconfig.h"
#include "nvm_eeprom_eeconfig_internal.h"
#include "nvm_eeprom_via_internal.h"
#include "keymap_introspection.h"
#include <time.h>
#include <stdlib.h>
#include "transactions.h"
#include "pointer_config_hardcoded.h"
#include "dilemma.h"

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 0, 0);

// Magic position for keycode execution
#define ARGOS_MATRIX_MAGIC 240

// Magic keycode override
uint16_t g_argos_magic_keycode_override = 0;

argos_config_t argos_config;

// Internal EEPROM access functions - uses eeconfig_kb_datablock
// TODO does this mess with the dilemma screen configuration? It should not
__attribute__((weak)) void argos_read_eeprom(uint16_t offset, void *buf, uint16_t size) {
        void *ee_start = (void *)(uintptr_t)(DYNAMIC_KEYMAP_EEPROM_MAX_ADDR + offset);
        void *ee_end   = (void *)(uintptr_t)(DYNAMIC_KEYMAP_EEPROM_MAX_ADDR + MIN(ARGOS_EEPROM_SIZE_CALC, offset + size));
        eeprom_read_block(buf, ee_start, ee_end - ee_start);
}

__attribute__((weak)) void argos_write_eeprom(uint16_t offset, const void *buf, uint16_t size) {
       void *ee_start = (void *)(uintptr_t)(DYNAMIC_KEYMAP_EEPROM_MAX_ADDR + offset);
       void *ee_end   = (void *)(uintptr_t)(DYNAMIC_KEYMAP_EEPROM_MAX_ADDR + MIN(ARGOS_EEPROM_SIZE_CALC, offset + size));
       eeprom_update_block(buf, ee_start, ee_end - ee_start);
}

/*
    To make migrating from QMK as easy as possible, on first load we copy
    over the combos.
    QMK does not store combos in eeprom, so we have to load them using combo_get_raw
    and then manually copy each one into eeprom through our custom data structure
    TODO other things, not only combos
*/
void keyboard_post_init_argos(void) {

    // Read configuration from eeprom
    argos_read_eeprom(ARGOS_OFFSET_CONFIG, &argos_config, sizeof(argos_config));
    if (!argos_config.has_copied_qmk_config) {
        // this is our first load ever
        argos_combos_copy_from_QMK();
        argos_config.has_copied_qmk_config = true;
        argos_config.themeId = 13; // default to dark theme
        argos_write_eeprom(ARGOS_OFFSET_CONFIG, &argos_config, sizeof(argos_config));
    }
    argos_combos_load_from_eeprom();
    argos_tap_dances_load_from_eeprom();
    argos_reload_tap_dances();
}

bool argos_handle_command(uint8_t* data, uint8_t length) {
    uint8_t protocol = data[0];

    if (protocol != ARGOS_CMD_PREFIX) return false; // not an Argos command

    uint8_t* command_id = &(data[1]);
    uint8_t* command_data = &(data[2]);

    // whether we should send a response back after handling the command
    // for example, when listening for a combo key, we want to wait until the key is pressed to send the data 
    bool send_data = false; 

    switch (*command_id) {
        case argos_id_get_kb_info: {
            command_data[0] = ARGOS_PROTOCOL_VERSION >> 8;
            command_data[1] = ARGOS_PROTOCOL_VERSION & 0xFF;
            command_data[2] = ARGOS_TAP_DANCE_ENTRIES;
            command_data[3] = ARGOS_COMBO_ENTRIES;
            command_data[4] = ARGOS_KEYS_PER_COMBO;
            command_data[5] = argos_config.themeId;
            command_data[6] = QMK_KEYCODES_VERSION_COMPATIBLE_0;
            command_data[7] = QMK_KEYCODES_VERSION_COMPATIBLE_1;
            command_data[8] = QMK_KEYCODES_VERSION_COMPATIBLE_2;
            send_data = true;
            break;
        }

        case argos_id_get_pointing_device_info: {
            command_data[0] = pointing_device_type_unknown;
            // TODO dilemma v2, TODO check charybdis working
#ifdef POINTING_DEVICE_DRIVER_pmw3360 // TODO test
                command_data[0] = pointing_device_type_trackball;
#endif
#ifdef POINTING_DEVICE_DRIVER_digitizer // Dilemma v3, todo test procyon?
                // TODO move this to other file?...
                command_data[0] = pointing_device_type_trackpad;
                // pointing dpi is up to 400+16*200 = 3600, 2 bytes
                command_data[1] = dilemma_get_pointer_default_dpi() & 0xFF;
                command_data[2] = (dilemma_get_pointer_default_dpi() >> 8) & 0xFF;
                // minimum default DPI is 400, 2 bytes
                command_data[3] = ARGOS_DILEMMA_MINIMUM_DEFAULT_DPI & 0xFF;
                command_data[4] = (ARGOS_DILEMMA_MINIMUM_DEFAULT_DPI >> 8) & 0xFF;
                // default DPI config step is 200, so one byte, but we use 2 just in case
                command_data[5] = ARGOS_DILEMMA_DEFAULT_DPI_CONFIG_STEP & 0xFF;
                command_data[6] = (ARGOS_DILEMMA_DEFAULT_DPI_CONFIG_STEP >> 8) & 0xFF;
                // sniping DPI is up to 200+4*100 = 600, 2 bytes
                command_data[7] = dilemma_get_pointer_sniping_dpi() & 0xFF;
                command_data[8] = (dilemma_get_pointer_sniping_dpi() >> 8) & 0xFF;
                // mininmum sniping dpi is 200, but ue use 2 bytes ju) in case
                command_data[9] = ARGOS_DILEMMA_MINIMUM_SNIPING_DPI & 0xFF;
                command_data[10] = (ARGOS_DILEMMA_MINIMUM_SNIPING_DPI >> 8) & 0xFF;
                // sniping DPI config step is 100, so one byte, but we use 2 just in case
                command_data[11] = ARGOS_DILEMMA_SNIPING_DPI_CONFIG_STEP & 0xFF;
                command_data[12] = (ARGOS_DILEMMA_SNIPING_DPI_CONFIG_STEP >> 8) & 0xFF;
                // dragscroll DPI is 100, but use 2 bytes just in case
                command_data[13] = ARGOS_DILEMMA_DRAGSCROLL_DPI & 0xFF;
                command_data[14] = (ARGOS_DILEMMA_DRAGSCROLL_DPI >> 8) & 0xFF;
                // pointing DPI max steps is 16, so one byte is plenty
                // this is hardcoded here as we can't read it from dilemma.c (private config structure dilemma_config_t)
                command_data[15] = 16;
                // sniping DPI max steps is 4, so one byte is plenty
                // this is hardcoded here as we can't read it from dilemma.c (private config structure dilemma_config_t)
                command_data[16] = 4;

#endif
            send_data = true;
            break;
        }

        case argos_id_set_dpi: {
            // TODO dilemma v2
            // TODO charybdis
#ifdef POINTING_DEVICE_DRIVER_digitizer
            printf("setting dpi: %d\n", command_data[0] | (command_data[1] << 8));
            // new dpi is on 2 bytes:
            uint16_t new_dpi = command_data[0] | (command_data[1] << 8);
            // get the old DPI:
            uint16_t old_dpi = dilemma_get_pointer_default_dpi();
            // calculate the difference:
            int16_t difference = new_dpi - old_dpi;
            printf("difference: %d\n", difference);
            // calculate how many steps we need, it could be negative
            int8_t new_steps = difference / ARGOS_DILEMMA_DEFAULT_DPI_CONFIG_STEP;
            printf("new steps: %d\n", new_steps);
            // apply the steps one by one
            bool forward = new_steps > 0;
            for(int i = 0; i < abs(new_steps); i++) {
                dilemma_cycle_pointer_default_dpi(forward);
            }
#endif
            send_data = true;
            break;
        }

        case argos_id_set_sniping_dpi: {
            // TODO dilemma v2
            // TODO charybdis
#ifdef POINTING_DEVICE_DRIVER_digitizer
            printf("setting sniping dpi: %d\n", command_data[0] | (command_data[1] << 8));
            // new dpi is on 2 bytes:
            uint16_t new_dpi = command_data[0] | (command_data[1] << 8);
            // get the old DPI:
            uint16_t old_dpi = dilemma_get_pointer_sniping_dpi();
            // calculate the difference:
            int16_t difference = new_dpi - old_dpi;
            printf("difference: %d\n", difference);
            // calculate how many steps we need, it could be negative
            int8_t new_steps = difference / ARGOS_DILEMMA_SNIPING_DPI_CONFIG_STEP;
            printf("new steps: %d\n", new_steps);
            // apply the steps one by one
            bool forward = new_steps > 0;
            for(int i = 0; i < abs(new_steps); i++) {
                dilemma_cycle_pointer_sniping_dpi(forward);
            }
#endif
            send_data = true;
            break;
        }

        // TODO : delete this, we already load it in argos_id_get_kb_info
        case argos_id_get_theme_id: {
            command_data[0] = argos_config.themeId;
            send_data = true;
            printf("Reading theme id: %d\n", argos_config.themeId);
            break;
        }

        // TODO : with the whole config?
        case argos_id_set_theme_id: {
            argos_config.themeId = command_data[0];
            argos_write_eeprom(ARGOS_OFFSET_CONFIG, &argos_config, sizeof(argos_config));
            send_data = true;
            printf("set theme id to %d\n", argos_config.themeId);
            break;
        }
        
     case argos_id_get_tap_dance: {
        uint8_t index = command_data[0];
        argos_td_entry_t entry = {0};
        // TODO: is it really necessary to read from the eeprom here?
        // can't we just have an array of tap dances?
        argos_tap_dance_read_eeprom(index, &entry);
        // TODO send the data back?
        // memcpy(command_data, &entry, sizeof(argos_td_entry_t));
        command_data[1] = entry.on_tap & 0xFF;
        command_data[2] = (entry.on_tap >> 8) & 0xFF;
        command_data[3] = entry.on_hold & 0xFF;
        command_data[4] = (entry.on_hold >> 8) & 0xFF;
        command_data[5] = entry.on_double_tap & 0xFF;
        command_data[6] = (entry.on_double_tap >> 8) & 0xFF;
        command_data[7] = entry.on_tap_hold & 0xFF;
        command_data[8] = (entry.on_tap_hold >> 8) & 0xFF;
        command_data[9] = entry.custom_tapping_term & 0xFF;
        command_data[10] = (entry.custom_tapping_term >> 8) & 0xFF;
        send_data = true;
        break;
     }

    //  // TODO: I don't think we actually use this
    //  case argos_id_set_tap_dance: {
    //     uint8_t index = command_data[0];
    //     argos_td_entry_t entry = {0};
    //     memcpy(&entry, &command_data[1], sizeof(argos_td_entry_t));
    //     // TODO status? 
    //     argos_tap_dance_write_eeprom(index, &entry);
    //     // TODO reload only one tap dance
    //     // TODO why is this needed?
    //     argos_reload_tap_dances();
    //     break;
    //  }

        // TODO manage custom tapping terms?
        case argos_id_get_combo: {
            uint8_t combo_index = command_data[0];
            if(combo_index >= ARGOS_COMBO_ENTRIES) break; // invalid index
            combo_t combo = argos_combo_get(combo_index);
            // TODO fix check combo exists..
                command_data[1] = !combo.disabled;
                command_data[2] = combo.keycode & 0xFF;
                command_data[3] = (combo.keycode >> 8) & 0xFF;
                // data 4 and 5 reserved for custom tapping term later
                for (int i = 0; i < ARGOS_KEYS_PER_COMBO; i++) {
                    uint16_t key = combo.keys[i];
                    command_data[6 + i * 2] = key & 0xFF;
                    command_data[7 + i * 2] = (key >> 8) & 0xFF;
                }
                send_data = true;
            break;
        }

        case argos_id_capture_combo_key: {
            // This command is used to capture the next key press and return it in the response.
            // It is meant to be used when setting up a combo, to easily capture the keycode of each key in the combo.
            // We will also process the assignment of the captured key directly, without having to process another HID message.
            argos_combo_listen_for_key(command_data);
            break;
        }

        case argos_id_capture_tap_dance_key: {
            // This command is used to capture the next key press and return it in the response.
            // It is meant to be used when setting up a tap dance, to easily capture the keycode of each key in the tap dance.
            // We will also process the assignment of the captured key directly, without having to process another HID message.
            argos_tap_dance_listen_for_key(command_data);
            break;
        }

        case argos_id_delete_tap_dance_key: {
            uint8_t index = command_data[0];
            argos_tap_dance_reset_capturing_tap_dance_key_index(index);
            break;
        }

        // TODO
        case argos_id_delete_combo_key: {
            uint8_t key_index = command_data[0];
            argos_combo_reset_capturing_combo_key_index(key_index);
            break;
        }

        default:
            return false;
    }

    if(send_data){
        raw_hid_send(data, length);
    }

    return true;
}

// Override via_command_kb to intercept Via protocol commands
bool via_command_kb(uint8_t* data, uint8_t length) {
    // try to handle it with argos
    bool result = argos_handle_command(data, length);
    if (result) {
        return true;
    }
    else {
        printf("received a VIA command!\n");
        return false;
    }
    return false;
}

bool process_record_argos(uint16_t keycode, keyrecord_t *record) {
    if(record->event.pressed){
        // process combo first
        bool captured = !process_record_argos_combo(keycode, record);
        // then, process tap dance
        if(!captured)
            captured = !process_record_argos_tap_dance(keycode, record);
        if(captured) {
            return false; // we captured a combo key, no need to process further
        }
    }
    return true;
}

// Override keymap_key_to_keycode to handle magic position for tap dance/combo execution
uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key) {
    if (key.row == ARGOS_MATRIX_MAGIC && key.col == ARGOS_MATRIX_MAGIC) {
       return g_argos_magic_keycode_override;
   } else  if (key.row < MATRIX_ROWS && key.col < MATRIX_COLS) {
       return keycode_at_keymap_location(layer, key.row, key.col);
   }
#ifdef ENCODER_MAP_ENABLE
   else if (key.row == KEYLOC_ENCODER_CW && key.col < NUM_ENCODERS) {
       return keycode_at_encodermap_location(layer, key.col, true);
   } else if (key.row == KEYLOC_ENCODER_CCW && key.col < NUM_ENCODERS) {
       return keycode_at_encodermap_location(layer, key.col, false);
   }
#endif // ENCODER_MAP_ENABLE
#ifdef DIP_SWITCH_MAP_ENABLE
   else if (key.row == KEYLOC_DIP_SWITCH_ON && key.col < NUM_DIP_SWITCHES) {
       return keycode_at_dip_switch_map_location(key.col, true);
   } else if (key.row == KEYLOC_DIP_SWITCH_OFF && key.col < NUM_DIP_SWITCHES) {
       return keycode_at_dip_switch_map_location(key.col, false);
   }
#endif // DIP_SWITCH_MAP_ENABLE

   // Use dynamic keymap for normal keys
   return KC_NO;
}


// Keycode execution helpers
// We need it for tap dance
void argos_keycode_down(uint16_t keycode) {
    g_argos_magic_keycode_override = keycode;

    if (keycode <= QK_MODS_MAX) {
        register_code16(keycode);
    } else {
        action_exec((keyevent_t){
            .type = KEY_EVENT,
            .key = (keypos_t){.row = ARGOS_MATRIX_MAGIC, .col = ARGOS_MATRIX_MAGIC},
            .pressed = 1,
            .time = (timer_read() | 1)
        });
    }
}

// Keycode execution helpers
// We need it for tap dance
void argos_keycode_up(uint16_t keycode) {
    g_argos_magic_keycode_override = keycode;

    if (keycode <= QK_MODS_MAX) {
        unregister_code16(keycode);
    } else {
        action_exec((keyevent_t){
            .type = KEY_EVENT,
            .key = (keypos_t){.row = ARGOS_MATRIX_MAGIC, .col = ARGOS_MATRIX_MAGIC},
            .pressed = 0,
            .time = (timer_read() | 1)
        });
    }
}

void argos_keycode_tap(uint16_t keycode) {
    argos_keycode_down(keycode);
    wait_ms(ARGOS_TAP_CODE_DELAY);
    argos_keycode_up(keycode);
}