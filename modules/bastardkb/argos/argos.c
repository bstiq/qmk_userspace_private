
// Copyright 2026 Quentin LEBASTARD <bstkbd@gmail.com>
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
// #define ARGOS_MATRIX_MAGIC 240


void argos_init(void) {
  
}

// Weak keyboard post-init hook
__attribute__((weak)) void keyboard_post_init_argos_kb(void) {}

// Module hook for post-init
void keyboard_post_init_argos(void) {
    keyboard_post_init_argos_kb();
    argos_init();
}

// TODO: get leaders, layer states, hardware fragments
bool argos_handle_command(uint8_t* data, uint8_t length) {
    printf("Handling command: %#08x\n", data[0]);
    uint8_t command_id = data[0];

    switch (command_id) {
        // todo switch to command_data just like in VIA
        // case argos_cmd_get_info: {
        //     data[1] = ARGOS_PROTOCOL_VERSION & 0xFF;
        //     data[2] = (ARGOS_PROTOCOL_VERSION >> 8) & 0xFF;
        //     data[3] = (ARGOS_PROTOCOL_VERSION >> 16) & 0xFF;
        //     data[4] = (ARGOS_PROTOCOL_VERSION >> 24) & 0xFF;
        //     data[5] = ARGOS_TAP_DANCE_ENTRIES;
        //     data[6] = ARGOS_COMBO_ENTRIES;
        //     data[7] = ARGOS_KEY_OVERRIDE_ENTRIES;
        //     data[8] = ARGOS_ALT_REPEAT_KEY_ENTRIES;
        //     data[9] = argos_get_feature_flags();
        //     uint8_t uid[] = ARGOS_KEYBOARD_UID;
        //     memcpy(&data[10], uid, 8);
        //     break;
        // }

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

        // Not used for now
        // case argos_cmd_definition_size: {
        //     // uint32_t size = argos_get_definition_size();
        //     printf("Definition size: %lu\n", size);
        //     // todo memcpy instead?
        //     data[1] = size & 0xFF;
        //     data[2] = (size >> 8) & 0xFF;
        //     data[3] = (size >> 16) & 0xFF;
        //     data[4] = (size >> 24) & 0xFF;
        //     break;
        // }

                                      // Not used for now
        // case argos_cmd_definition_chunk: {
        //     // Request: [cmd] [offset_lo] [offset_hi] [request_size]
        //     // Response: [cmd] [offset_lo] [offset_hi] [request_size] [22 bytes data]
        //     const uint16_t offset = data[1] | (data[2] << 8); // offset is on 2 bytes
        //     const uint8_t chunk_size = argos_get_definition_chunk(offset, &data[4]);
        //     data[3] = chunk_size; 
        //     printf("sending definition chunk %d at offset %u\n", data[3], offset);
        //     break;
        // }

        // case argos_cmd_qmk_settings_query: {
        //     // Request: [cmd] [qsid_lo] [qsid_hi]
        //     // Response: [cmd] [qsid1_lo] [qsid1_hi] [qsid2_lo] ... [0xFF] [0xFF]
        //     uint16_t qsid_gt = data[1] | (data[2] << 8);
        //     // why is lefgth 2 here?
        //     argos_qmk_settings_query(qsid_gt, &data[1], length - 2);
        //     break;
        // }

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
// bool process_record_argos(uint16_t keycode, keyrecord_t *record) {
//     if (!process_record_argos_tap_dance(keycode, record)) {
//         return false;
//     }
//     return true;
// }
