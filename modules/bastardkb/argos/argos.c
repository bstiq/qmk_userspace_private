// Copyright 2025 Ira Cooper <ira@wakeful.net>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "argos.h"
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

// Magic position for keycode execution
#define ARGOS_MATRIX_MAGIC 240

// Global for keycode override during tap dance execution
uint16_t g_argos_magic_keycode_override;

// Internal EEPROM access functions - uses eeconfig_kb_datablock
__attribute__((weak)) void argos_read_eeprom(uint16_t offset, void *buf, uint16_t size) {
        void *ee_start = (void *)(uintptr_t)(DYNAMIC_KEYMAP_EEPROM_MAX_ADDR + offset);
        void *ee_end   = (void *)(uintptr_t)(DYNAMIC_KEYMAP_EEPROM_MAX_ADDR + MIN(ARGOS_EEPROM_SIZE_CALC, offset + size));
        // xprintf("EEPROM read: offset=%p size=%p\n", ee_start, ee_end);
        eeprom_read_block(buf, ee_start, ee_end - ee_start);
}

__attribute__((weak)) void argos_write_eeprom(uint16_t offset, const void *buf, uint16_t size) {
       void *ee_start = (void *)(uintptr_t)(DYNAMIC_KEYMAP_EEPROM_MAX_ADDR + offset);
       void *ee_end   = (void *)(uintptr_t)(DYNAMIC_KEYMAP_EEPROM_MAX_ADDR + MIN(ARGOS_EEPROM_SIZE_CALC, offset + size));
       eeprom_update_block(buf, ee_start, ee_end - ee_start);
}

// Magic header for EEPROM validation - derived from QMK_BUILDDATE
// QMK_BUILDDATE format: "2019-11-05-11:29:54"
// Use full timestamp (date + time) so every build gets unique magic
static void argos_get_magic(uint8_t *magic) {
    char *p = QMK_BUILDDATE;
    magic[0] = ((p[2] & 0x0F) << 4) | (p[3] & 0x0F);  // year low 2 digits
    magic[1] = ((p[5] & 0x0F) << 4) | (p[6] & 0x0F);  // month
    magic[2] = ((p[8] & 0x0F) << 4) | (p[9] & 0x0F);  // day
    magic[3] = ((p[11] & 0x0F) << 4) | (p[12] & 0x0F); // hour
    magic[4] = ((p[14] & 0x0F) << 4) | (p[15] & 0x0F); // minute
    magic[5] = ((p[17] & 0x0F) << 4) | (p[18] & 0x0F); // second
}

static bool argos_eeprom_is_valid(void) {
    uint8_t stored[ARGOS_MAGIC_SIZE];
    uint8_t expected[ARGOS_MAGIC_SIZE];
    argos_read_eeprom(ARGOS_MAGIC_OFFSET, stored, ARGOS_MAGIC_SIZE);
    argos_get_magic(expected);
    return memcmp(stored, expected, ARGOS_MAGIC_SIZE) == 0;
}

static void argos_eeprom_set_valid(void) {
    uint8_t magic[ARGOS_MAGIC_SIZE];
    argos_get_magic(magic);
    argos_write_eeprom(ARGOS_MAGIC_OFFSET, magic, ARGOS_MAGIC_SIZE);
}

void argos_init(void) {
    // Check if EEPROM data is valid (matches current firmware version)
    if (!argos_eeprom_is_valid()) {
        // Reset all argos data to defaults
        argos_reset();
        argos_qmk_settings_reset();
        // Mark as valid
        argos_eeprom_set_valid();
    }

    argos_reload_tap_dance();
    argos_reload_combo();
    argos_reload_key_override();
    argos_reload_alt_repeat_key();
    argos_qmk_settings_init();
}

// Weak keyboard post-init hook
__attribute__((weak)) void keyboard_post_init_argos_kb(void) {}

// Module hook for post-init
void keyboard_post_init_argos(void) {
    keyboard_post_init_argos_kb();
    argos_init();
}

// Override QMK's get_oneshot_timeout for runtime configuration
// TEMPORARILY DISABLED - may be called before EEPROM ready
// uint16_t get_oneshot_timeout(void) {
//     argos_one_shot_t settings;
//     argos_get_one_shot(&settings);
//     return settings.timeout;
// }

// Get feature flags based on what's enabled
uint8_t argos_get_feature_flags(void) {
    uint8_t flags = 0;
#ifdef CAPS_WORD_ENABLE
    flags |= argos_flag_caps_word;
#endif
#ifdef LAYER_LOCK_ENABLE
    flags |= argos_flag_layer_lock;
#endif
#ifdef ONESHOT_ENABLE
    flags |= argos_flag_oneshot;
#endif
#ifdef LEADER_ENABLE
    flags |= argos_flag_leader;
#endif
    return flags;
}

// Storage functions - Tap Dance
int argos_get_tap_dance(uint8_t index, argos_tap_dance_entry_t *entry) {
    if (index >= ARGOS_TAP_DANCE_ENTRIES) return -1;
    argos_read_eeprom(ARGOS_TAP_DANCE_OFFSET + index * sizeof(argos_tap_dance_entry_t),
                       entry, sizeof(argos_tap_dance_entry_t));
    return 0;
}

int argos_set_tap_dance(uint8_t index, const argos_tap_dance_entry_t *entry) {
    if (index >= ARGOS_TAP_DANCE_ENTRIES) return -1;
    argos_write_eeprom(ARGOS_TAP_DANCE_OFFSET + index * sizeof(argos_tap_dance_entry_t),
                        entry, sizeof(argos_tap_dance_entry_t));
    return 0;
}

// Storage functions - Combo
int argos_get_combo(uint8_t index, argos_combo_entry_t *entry) {
    if (index >= ARGOS_COMBO_ENTRIES) return -1;
    argos_read_eeprom(ARGOS_COMBO_OFFSET + index * sizeof(argos_combo_entry_t),
                       entry, sizeof(argos_combo_entry_t));
    return 0;
}

int argos_set_combo(uint8_t index, const argos_combo_entry_t *entry) {
    if (index >= ARGOS_COMBO_ENTRIES) return -1;
    argos_write_eeprom(ARGOS_COMBO_OFFSET + index * sizeof(argos_combo_entry_t),
                        entry, sizeof(argos_combo_entry_t));
    return 0;
}

// Storage functions - Key Override
int argos_get_key_override(uint8_t index, argos_key_override_entry_t *entry) {
    if (index >= ARGOS_KEY_OVERRIDE_ENTRIES) return -1;
    argos_read_eeprom(ARGOS_KEY_OVERRIDE_OFFSET + index * sizeof(argos_key_override_entry_t),
                       entry, sizeof(argos_key_override_entry_t));
    return 0;
}

int argos_set_key_override(uint8_t index, const argos_key_override_entry_t *entry) {
    if (index >= ARGOS_KEY_OVERRIDE_ENTRIES) return -1;
    argos_write_eeprom(ARGOS_KEY_OVERRIDE_OFFSET + index * sizeof(argos_key_override_entry_t),
                        entry, sizeof(argos_key_override_entry_t));
    return 0;
}

// Storage functions - Alt Repeat Key
int argos_get_alt_repeat_key(uint8_t index, argos_alt_repeat_key_entry_t *entry) {
    if (index >= ARGOS_ALT_REPEAT_KEY_ENTRIES) return -1;
    argos_read_eeprom(ARGOS_ALT_REPEAT_KEY_OFFSET + index * sizeof(argos_alt_repeat_key_entry_t),
                       entry, sizeof(argos_alt_repeat_key_entry_t));
    return 0;
}

int argos_set_alt_repeat_key(uint8_t index, const argos_alt_repeat_key_entry_t *entry) {
    if (index >= ARGOS_ALT_REPEAT_KEY_ENTRIES) return -1;
    argos_write_eeprom(ARGOS_ALT_REPEAT_KEY_OFFSET + index * sizeof(argos_alt_repeat_key_entry_t),
                        entry, sizeof(argos_alt_repeat_key_entry_t));
    return 0;
}

// Storage functions - One-Shot
void argos_get_one_shot(argos_one_shot_t *settings) {
    argos_read_eeprom(ARGOS_ONE_SHOT_OFFSET, settings, sizeof(argos_one_shot_t));
}

void argos_set_one_shot(const argos_one_shot_t *settings) {
    argos_write_eeprom(ARGOS_ONE_SHOT_OFFSET, settings, sizeof(argos_one_shot_t));
}

void argos_save(void) {
    // Data is written directly to EEPROM, nothing additional to flush
}

void argos_reset(void) {
    // Zero out all EEPROM storage
    uint8_t zero[16] = {0};
    for (uint16_t i = 0; i < ARGOS_EEPROM_SIZE; i += sizeof(zero)) {
        uint16_t chunk = sizeof(zero);
        if (i + chunk > ARGOS_EEPROM_SIZE) {
            chunk = ARGOS_EEPROM_SIZE - i;
        }
        argos_write_eeprom(i, zero, chunk);
    }
    argos_reload_tap_dance();
    argos_reload_combo();
    argos_reload_key_override();
    argos_reload_alt_repeat_key();
}

// Keycode execution helpers
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
    wait_ms(TAP_CODE_DELAY);
    argos_keycode_up(keycode);
}

// TODO: get leaders, layer states, hardware fragments
bool argos_handle_command(uint8_t* data, uint8_t length) {
    printf("Handling command: %#08x\n", data[0]);
    uint8_t command_id = data[0];
    uint8_t *command_data = &(data[1]);

    switch (command_id) {
        // todo switch to command_data just like in VIA
        case argos_cmd_get_info: {
            data[1] = ARGOS_PROTOCOL_VERSION & 0xFF;
            data[2] = (ARGOS_PROTOCOL_VERSION >> 8) & 0xFF;
            data[3] = (ARGOS_PROTOCOL_VERSION >> 16) & 0xFF;
            data[4] = (ARGOS_PROTOCOL_VERSION >> 24) & 0xFF;
            data[5] = ARGOS_TAP_DANCE_ENTRIES;
            data[6] = ARGOS_COMBO_ENTRIES;
            data[7] = ARGOS_KEY_OVERRIDE_ENTRIES;
            data[8] = ARGOS_ALT_REPEAT_KEY_ENTRIES;
            data[9] = argos_get_feature_flags();
            uint8_t uid[] = ARGOS_KEYBOARD_UID;
            memcpy(&data[10], uid, 8);
            break;
        }

        // this will be present in future version of VIA (0x000D).
        // for now, we override it
        case argos_cmd_keycodes_version: {
            uint32_t value  = QMK_KEYCODES_VERSION_BCD;
            command_data[1] = (value >> 24) & 0xFF;
            command_data[2] = (value >> 16) & 0xFF;
            command_data[3] = (value >> 8) & 0xFF;
            command_data[4] = value & 0xFF;
            break;
        }

        // // TODO change data numbering for all other entries...
        // case argos_cmd_tap_dance_get: {
        //     // Request: [0xDF] [0x01] [index]
        //     // Response: [0xDF] [0x01] [index] [10 bytes entry]
        //     uint8_t idx = data[2];
        //     argos_tap_dance_entry_t entry = {0};
        //     argos_get_tap_dance(idx, &entry);
        //     memcpy(&data[3], &entry, sizeof(entry));
        //     break;
        // }

        // case argos_cmd_tap_dance_set: {
        //     // Request: [0xDF] [0x02] [index] [10 bytes entry]
        //     // Response: [0xDF] [0x02] [status]
        //     uint8_t idx = data[2];
        //     argos_tap_dance_entry_t entry;
        //     memcpy(&entry, &data[3], sizeof(entry));
        //     data[2] = argos_set_tap_dance(idx, &entry) == 0 ? 0 : 1;
        //     argos_reload_tap_dance();
        //     break;
        // }

        // case argos_cmd_combo_get: {
        //     // Request: [0xDF] [0x03] [index]
        //     // Response: [0xDF] [0x03] [index] [12 bytes entry]
        //     uint8_t idx = data[2];
        //     argos_combo_entry_t entry = {0};
        //     argos_get_combo(idx, &entry);
        //     memcpy(&data[3], &entry, sizeof(entry));
        //     break;
        // }

        // case argos_cmd_combo_set: {
        //     // Request: [0xDF] [0x04] [index] [12 bytes entry]
        //     // Response: [0xDF] [0x04] [status]
        //     uint8_t idx = data[2];
        //     argos_combo_entry_t entry;
        //     memcpy(&entry, &data[3], sizeof(entry));
        //     data[2] = argos_set_combo(idx, &entry) == 0 ? 0 : 1;
        //     argos_reload_combo();
        //     break;
        // }

        // case argos_cmd_key_override_get: {
        //     // Request: [0xDF] [0x05] [index]
        //     // Response: [0xDF] [0x05] [index] [12 bytes entry]
        //     uint8_t idx = data[2];
        //     argos_key_override_entry_t entry = {0};
        //     argos_get_key_override(idx, &entry);
        //     memcpy(&data[3], &entry, sizeof(entry));
        //     break;
        // }

        // case argos_cmd_key_override_set: {
        //     // Request: [0xDF] [0x06] [index] [12 bytes entry]
        //     // Response: [0xDF] [0x06] [status]
        //     uint8_t idx = data[2];
        //     argos_key_override_entry_t entry;
        //     memcpy(&entry, &data[3], sizeof(entry));
        //     data[2] = argos_set_key_override(idx, &entry) == 0 ? 0 : 1;
        //     argos_reload_key_override();
        //     break;
        // }

        // case argos_cmd_alt_repeat_key_get: {
        //     // Request: [0xDF] [0x07] [index]
        //     // Response: [0xDF] [0x07] [index] [6 bytes entry]
        //     uint8_t idx = data[2];
        //     argos_alt_repeat_key_entry_t entry = {0};
        //     argos_get_alt_repeat_key(idx, &entry);
        //     memcpy(&data[3], &entry, sizeof(entry));
        //     break;
        // }

        // case argos_cmd_alt_repeat_key_set: {
        //     // Request: [0xDF] [0x08] [index] [6 bytes entry]
        //     // Response: [0xDF] [0x08] [status]
        //     uint8_t idx = data[2];
        //     argos_alt_repeat_key_entry_t entry;
        //     memcpy(&entry, &data[3], sizeof(entry));
        //     data[2] = argos_set_alt_repeat_key(idx, &entry) == 0 ? 0 : 1;
        //     argos_reload_alt_repeat_key();
        //     break;
        // }

        // case argos_cmd_one_shot_get: {
        //     // Request: [0xDF] [0x09]
        //     // Response: [0xDF] [0x09] [timeout_lo] [timeout_hi] [tap_toggle]
        //     argos_one_shot_t settings = {0};
        //     argos_get_one_shot(&settings);
        //     data[2] = settings.timeout & 0xFF;
        //     data[3] = (settings.timeout >> 8) & 0xFF;
        //     data[4] = settings.tap_toggle;
        //     break;
        // }

        // case argos_cmd_one_shot_set: {
        //     // Request: [0xDF] [0x0A] [timeout_lo] [timeout_hi] [tap_toggle]
        //     // Response: [0xDF] [0x0A]
        //     argos_one_shot_t settings;
        //     settings.timeout = data[2] | (data[3] << 8);
        //     settings.tap_toggle = data[4];
        //     argos_set_one_shot(&settings);
        //     break;
        // }

        // case argos_cmd_save: {
        //     // Request: [0xDF] [0x0B]
        //     // Response: [0xDF] [0x0B]
        //     argos_save();
        //     break;
        // }

        // case argos_cmd_reset: {
        //     // Request: [0xDF] [0x0C]
        //     // Response: [0xDF] [0x0C]
        //     argos_reset();
        //     break;
        // }

        case argos_cmd_definition_size: {
            uint32_t size = argos_get_definition_size();
            printf("Definition size: %lu\n", size);
            // todo memcpy instead?
            data[1] = size & 0xFF;
            data[2] = (size >> 8) & 0xFF;
            data[3] = (size >> 16) & 0xFF;
            data[4] = (size >> 24) & 0xFF;
            break;
        }

        case argos_cmd_definition_chunk: {
            // Request: [cmd] [offset_lo] [offset_hi] [request_size]
            // Response: [cmd] [offset_lo] [offset_hi] [request_size] [22 bytes data]
            const uint16_t offset = data[1] | (data[2] << 8); // offset is on 2 bytes
            const uint8_t chunk_size = argos_get_definition_chunk(offset, &data[4]);
            data[3] = chunk_size; 
            printf("sending definition chunk %d at offset %u\n", data[3], offset);
            break;
        }

        case argos_cmd_qmk_settings_query: {
            // Request: [cmd] [qsid_lo] [qsid_hi]
            // Response: [cmd] [qsid1_lo] [qsid1_hi] [qsid2_lo] ... [0xFF] [0xFF]
            uint16_t qsid_gt = data[1] | (data[2] << 8);
            // why is lefgth 2 here?
            argos_qmk_settings_query(qsid_gt, &data[1], length - 2);
            break;
        }

        // case argos_cmd_qmk_settings_get: {
        //     // Request: [0xDF] [0x11] [qsid_lo] [qsid_hi]
        //     // Response: [0xDF] [0x11] [status] [value bytes...]
        //     uint16_t qsid = data[2] | (data[3] << 8);
        //     data[2] = argos_qmk_settings_get(qsid, &data[3], length - 3);
        //     break;
        // }

        // case argos_cmd_qmk_settings_set: {
        //     // Request: [0xDF] [0x12] [qsid_lo] [qsid_hi] [value bytes...]
        //     // Response: [0xDF] [0x12] [status]
        //     uint16_t qsid = data[2] | (data[3] << 8);
        //     data[2] = argos_qmk_settings_set(qsid, &data[4], length - 4);
        //     break;
        // }

        // case argos_cmd_qmk_settings_reset: {
        //     // Request: [0xDF] [0x13]
        //     // Response: [0xDF] [0x13]
        //     argos_qmk_settings_reset();
        //     break;
        // }

        default:
            return false;
    }

    return true;
}

// Override via_command_kb to intercept Via protocol commands
bool via_command_kb(uint8_t *data, uint8_t length) {
    // try to handle it with argos
    bool result = argos_handle_command(data, length);
    if (result) {
        printf("received a ARGOS command!\n");
        raw_hid_send(data, length);
        return true;
    }
    // if that does not work, we forward it to via
    else {
        printf("received a VIA command!\n");
        return false;
    }
}

// Process record hook for Argos features
bool process_record_argos(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_argos_tap_dance(keycode, record)) {
        return false;
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
