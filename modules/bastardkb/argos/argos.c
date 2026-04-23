
// Copyright 2026 Quentin LEBASTARD <bstkbd@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "argos.h"
#include "argos_combo.h"
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

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 0, 0);

/*
    To make migrating from QMK as easy as possible, on first load we copy
    over the previous configuration.
    QMK does not store combos, tap dance etc in eeprom, so we have to load them
    and then manually copy each one into eeprom through our custom data structure
*/
bool has_copied_qmk_config = false;

// Internal EEPROM access functions - uses eeconfig_kb_datablock
// TODO does this mess with the dilemma screen configuration?
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
*/
void argos_init(void) {
    argos_read_eeprom(ARGOS_OFFSET_HAS_COPIED_QMK, &has_copied_qmk_config, sizeof(has_copied_qmk_config));
    if (!has_copied_qmk_config) {
        argos_copy_combos_from_QMK();
         has_copied_qmk_config = true;
        argos_write_eeprom(ARGOS_OFFSET_HAS_COPIED_QMK, &has_copied_qmk_config, sizeof(has_copied_qmk_config));
    }
    argos_load_combos_eeprom();
}

// Weak keyboard post-init hook
__attribute__((weak)) void keyboard_post_init_argos_kb(void) {}

// Module hook for post-init
void keyboard_post_init_argos(void) {
    keyboard_post_init_argos_kb();
    argos_init();
}


bool argos_handle_command(uint8_t* data, uint8_t length) {
    printf("Handling command: %#08x\n", data[0]);
    uint8_t protocol = data[0];

    if (protocol != ARGOS_CMD_PREFIX) return false; // not an Argos command

    uint8_t* command_id = &(data[1]);
    uint8_t* command_data = &(data[2]);


    switch (*command_id) {
        case argos_id_get_protocol_version: {
            command_data[0] = ARGOS_PROTOCOL_VERSION >> 8;
            command_data[1] = ARGOS_PROTOCOL_VERSION & 0xFF;
            break;
        }


            /*
            process_combo.c 
            keymap_introspection

            #    define COMBO_ACTIVE(combo) (combo->state & 0x80)
#    define COMBO_DISABLED(combo) (combo->state & 0x40)
#    define COMBO_STATE(combo) (combo->state & 0x3F)

            typedef struct combo_t {
        const uint16_t *keys;
        uint16_t        keycode;
    #ifdef EXTRA_SHORT_COMBOS
        uint8_t state;
    #else
        bool disabled;
        bool active;
    #    if defined(EXTRA_EXTRA_LONG_COMBOS)
        uint32_t state;
    #    elif defined(EXTRA_LONG_COMBOS)
        uint16_t state;
    #    else
        uint8_t state;
    #    endif
    #endif
    } combo_t;
                                          */

        //                                   // TODO test if combos enabled
        // case argos_id_get_combos_count: {
        //     uint8_t combo_size = ARRAY_SIZE(key_combos);
            
        //     command_data[0] = combo_size;
        //     break;
        // }

        // TODO: send keys per combo to the webapp so it knows how to unpack data
        case argos_id_get_combo: {
            // uint8_t combo_index = command_data[0];
            // combo_t* combo = combo_get_raw(combo_index);

            // // Then, we need to serialize it into the response data.
            // // For now, we will return: disabled, active, keycode.
            // command_data[1] = combo->disabled;
            // // keycode is 2 bytes, we will split it into 2 uint8_t
            // command_data[2] = combo->keycode & 0xFF;
            // command_data[3] = (combo->keycode >> 8) & 0xFF;
            // // then, we have up to 3 keys that can be pressed together.
            // for (int i = 0; i <= 2; i++) {
            //     uint16_t key = combo->keys[i];
            //     command_data[4 + i * 2] = key & 0xFF;
            //     command_data[5 + i * 2] = (key >> 8) & 0xFF;
            // }
            // break;
            uint8_t combo_index = command_data[0];
            if(combo_index >= ARGOS_COMBO_ENTRIES) break; // invalid index
            argos_combo_t combo = argos_get_combo(combo_index);
            // TODO fix check combo exists..
            // if (combo) { 
                printf("Returning combo %d: enabled=%d output=%d keys=[%d %d %d %d]\n", combo_index, combo.enabled, combo.output, combo.input[0], combo.input[1], combo.input[2], combo.input[3]);
                command_data[1] = !(combo.enabled);
                command_data[2] = combo.output & 0xFF;
                command_data[3] = (combo.output >> 8) & 0xFF;
                for (int i = 0; i < ARGOS_KEYS_PER_COMBO; i++) {
                    uint16_t key = combo.input[i];
                    command_data[4 + i * 2] = key & 0xFF;
                    command_data[5 + i * 2] = (key >> 8) & 0xFF;
                }
            // }
            break;
        }

        // TODO
        case argos_id_set_combo: {
            // uint8_t combo_index = command_data[0];
            // combo_t* combo;

            // combo->disabled = command_data[1];
            // combo->keycode = command_data[2] | (command_data[3] << 8);
            // for (int i = 0; i <= 2; i++) {
            //     uint16_t key = command_data[4 + i * 2] | (command_data[5 + i * 2] << 8);
            //     combo->keys[i] = key;
            // }
            
            break;
        }

        default:
            return false;
    }

    return true;
}

// Override via_command_kb to intercept Via protocol commands
bool via_command_kb(uint8_t* data, uint8_t length) {
    // try to handle it with argos
    bool result = argos_handle_command(data, length);
    if (result) {
        printf("received a ARGOS command!\n");
        raw_hid_send(data, length);
        return true;
    }
    // // if that does not work, we forward it to via
    else {
        printf("received a VIA command!\n");
        return false;
    }
    return false;
}

// Process record hook for Argos features
// bool process_record_argos(uint16_t keycode, keyrecord_t *record) {
//     if (!process_record_argos_tap_dance(keycode, record)) {
//         return false;
//     }
//     return true;
// }
