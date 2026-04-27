// Copyright 2026 Quentin LEBASTARD <bstkbd@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "action.h"
#include "util.h"

// Argos protocol version
#define ARGOS_PROTOCOL_VERSION 0x0001
#define ARGOS_CMD_PREFIX 0x90 // hopefully something that won't conflict with VIA

// Used to capture keys (for combos and tap dances)
uint32_t last_activity_time;

enum argos_command_id {
    argos_id_get_protocol_version = 0x01,
    argos_id_get_combo = 0x02,
    argos_id_delete_combo_key = 0x03,
    argos_id_capture_combo_key = 0x04,
    argos_id_get_theme_id = 0x05,
    argos_id_set_theme_id = 0x06,
    argos_id_get_tap_dance = 0x07,
    argos_id_set_tap_dance = 0x08,
    argos_id_capture_tap_dance_key = 0x09,
};

#define ARGOS_COMBO_ENTRIES 16
#define ARGOS_KEYS_PER_COMBO 4

// We define our own structure, with the keys NOT const
typedef struct PACKED {
    uint16_t keys[ARGOS_KEYS_PER_COMBO]; 
    uint16_t keycode;
    uint8_t  state; 
    bool     disabled : 1;
    bool     active : 1;
    bool valid: 1; 
} argos_combo_t;
// If we modify the structure, we also need to modify its size in post_config.h
_Static_assert(sizeof(argos_combo_t) <= 13, "Invalid size for argos_combo_t");

typedef struct PACKED {
    bool has_copied_qmk_config : 1;
    uint8_t themeId;
} argos_config_t;
_Static_assert(sizeof(argos_config_t) == 2, "Invalid size for argos_config_t");

#define ARGOS_TAPPING_TERM 175
#define ARGOS_TAP_CODE_DELAY 10

// TODO get rid of custom tapping term, and use a global one
typedef struct __attribute__((packed)) {
    uint16_t on_tap;
    uint16_t on_hold;
    uint16_t on_double_tap;
    uint16_t on_tap_hold;
    uint16_t custom_tapping_term;  // bit 15 = enabled, bits 0-14 = timing (ms)
    bool enabled : 1;
} argos_td_entry_t;
_Static_assert(sizeof(argos_td_entry_t) <= 11, "Invalid size for argos_td_entry_t");


// -------------------------------

__attribute__((weak)) void argos_read_eeprom(uint16_t offset, void *buf, uint16_t size);
__attribute__((weak)) void argos_write_eeprom(uint16_t offset, const void *buf, uint16_t size);
void keyboard_post_init_argos(void);
bool argos_handle_command(uint8_t* data, uint8_t length);
void argos_raw_hid_send_captured_key(void);
void argos_keycode_down(uint16_t keycode);
void argos_keycode_up(uint16_t keycode);
void argos_keycode_tap(uint16_t keycode);
bool argos_tap_dance_read_eeprom(uint8_t index, argos_td_entry_t *entry);
bool argos_tap_dance_write_eeprom(uint8_t index, const argos_td_entry_t *entry);
