// Copyright 2026 Quentin LEBASTARD <bstkbd@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "action.h"

// Argos protocol version
#define ARGOS_PROTOCOL_VERSION 0x0001
#define ARGOS_CMD_PREFIX 0x90 // hopefully something that won't conflict with VIA

enum argos_command_id {
    argos_id_get_protocol_version = 0x01,
    argos_id_get_combo = 0x02,
    argos_id_set_combo = 0x03,
    argos_id_capture_combo_key = 0x04,
    // argos_id_get_combos_count = 0x03,
    // argos_cmd_tap_dance_get     = 0x91,
    // argos_cmd_tap_dance_set     = 0x92,
    // argos_cmd_combo_get         = 0x93,
    // argos_cmd_combo_set         = 0x94,
    // argos_cmd_key_override_get  = 0x95,
    // argos_cmd_key_override_set  = 0x96,
    // argos_cmd_alt_repeat_key_get = 0x97,
    // argos_cmd_alt_repeat_key_set = 0x98,
    // argos_cmd_one_shot_get      = 0x99,
    // argos_cmd_one_shot_set      = 0x9A,
    // argos_cmd_save              = 0x9B,
    // argos_cmd_reset             = 0x9C,
    // argos_cmd_definition_size   = 0x9D,
    // argos_cmd_definition_chunk  = 0x9E,
    // // QMK Settings commands
    // argos_cmd_qmk_settings_query = 0x9F,
    // argos_cmd_qmk_settings_get   = 0xA0,
    // argos_cmd_qmk_settings_set = 0xA1,
    // argos_cmd_qmk_settings_reset = 0xA2,
    // argos_cmd_layer_state_get = 0xA5,
    // argos_cmd_error             = 0xDE,
};

#define ARGOS_COMBO_ENTRIES 16
#define ARGOS_KEYS_PER_COMBO 4

// -------------------------------
// TODO is this really needed? maybe for a reset? do we even need
// resets when QMK bootmagic is a thing?
// or maybe when data is not valid, TODO later....
#define ARGOS_OFFSET_HAS_COPIED_QMK 0
#define ARGOS_SIZE_HAS_COPIED_QMK sizeof(bool)

#define ARGOS_OFFSET_COMBO (ARGOS_OFFSET_HAS_COPIED_QMK + ARGOS_SIZE_HAS_COPIED_QMK)
#define ARGOS_SIZE_COMBOS (ARGOS_COMBO_ENTRIES * ARGOS_SIZE_COMBO)
#define ARGOS_SIZE_EEPROM (ARGOS_OFFSET_COMBO + ARGOS_SIZE_COMBOS)
// TODO END
// -------------------------------

__attribute__((weak)) void argos_read_eeprom(uint16_t offset, void *buf, uint16_t size);
__attribute__((weak)) void argos_write_eeprom(uint16_t offset, const void *buf, uint16_t size);
void keyboard_post_init_argos(void);
bool argos_handle_command(uint8_t* data, uint8_t length);
void argos_raw_hid_send_captured_key(void);