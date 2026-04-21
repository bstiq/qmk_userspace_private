// Copyright 2025 Ira Cooper <ira@wakeful.net>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "action.h"
#include "argos_qmk_settings.h"

// Argos protocol version
#define ARGOS_PROTOCOL_VERSION 0x00000001

// Keyboard UID - use VIAL_KEYBOARD_UID for backwards compatibility with .vil files
#ifndef ARGOS_KEYBOARD_UID
#   ifdef VIAL_KEYBOARD_UID
#       define ARGOS_KEYBOARD_UID VIAL_KEYBOARD_UID
#   else
#       define ARGOS_KEYBOARD_UID {0, 0, 0, 0, 0, 0, 0, 0}
#   endif
#endif

// Protocol prefix for 0xDF direct protocol
#define ARGOS_PREFIX 0xDF
#define WRAPPER_PREFIX 0xDD
#define BOOTSTRAP_PREFIX 0x00

// USB serial number magic for GUI/web detection
// TODO is this used? if we don't use vial anymore
#ifndef SERIAL_NUMBER
#    define SERIAL_NUMBER "argos:12345-00"
#endif

// Argos command IDs (0xDF protocol v2)
enum argos_command_id {
    argos_cmd_get_info          = 0x90,
    argos_cmd_tap_dance_get     = 0x91,
    argos_cmd_tap_dance_set     = 0x92,
    argos_cmd_combo_get         = 0x93,
    argos_cmd_combo_set         = 0x94,
    argos_cmd_key_override_get  = 0x95,
    argos_cmd_key_override_set  = 0x96,
    argos_cmd_alt_repeat_key_get = 0x97,
    argos_cmd_alt_repeat_key_set = 0x98,
    argos_cmd_one_shot_get      = 0x99,
    argos_cmd_one_shot_set      = 0x9A,
    argos_cmd_save              = 0x9B,
    argos_cmd_reset             = 0x9C,
    argos_cmd_definition_size   = 0x9D,
    argos_cmd_definition_chunk  = 0x9E,
    // QMK Settings commands
    argos_cmd_qmk_settings_query = 0x9F,
    argos_cmd_qmk_settings_get   = 0xA0,
    argos_cmd_qmk_settings_set = 0xA1,
    argos_cmd_qmk_settings_reset = 0xA2,
    argos_cmd_layer_state_get = 0xA5,
    argos_cmd_error             = 0xDE,
    argos_cmd_keycodes_version    = 0x07, // in new via version, has not landed yet - for now, override
};

// Feature capability flags (returned in protocol info)
enum argos_feature_flags {
    argos_flag_caps_word   = (1 << 0),
    argos_flag_layer_lock = (1 << 1),
    argos_flag_oneshot = (1 << 2),
    argos_flag_leader = (1 << 3),
    // bits 4-7 reserved
};

// Keyboard definition chunk size (fits in 32-byte HID packet with header)
#define ARGOS_DEFINITION_CHUNK_SIZE 28

// Entry counts - default values, can be overridden
#ifndef ARGOS_TAP_DANCE_ENTRIES
#    define ARGOS_TAP_DANCE_ENTRIES 16
#endif

#ifndef ARGOS_COMBO_ENTRIES
#    define ARGOS_COMBO_ENTRIES 16
#endif

#ifndef ARGOS_KEY_OVERRIDE_ENTRIES
#    define ARGOS_KEY_OVERRIDE_ENTRIES 16
#endif

#ifndef ARGOS_ALT_REPEAT_KEY_ENTRIES
#    define ARGOS_ALT_REPEAT_KEY_ENTRIES 16
#endif

// Tap Dance entry structure (10 bytes)
// Enabled when custom_tapping_term bit 15 = 1
typedef struct __attribute__((packed)) {
    uint16_t on_tap;
    uint16_t on_hold;
    uint16_t on_double_tap;
    uint16_t on_tap_hold;
    uint16_t custom_tapping_term;  // bit 15 = enabled, bits 0-14 = timing (ms)
} argos_tap_dance_entry_t;
_Static_assert(sizeof(argos_tap_dance_entry_t) == 10, "argos_tap_dance_entry_t must be 10 bytes");

// Combo entry structure (12 bytes)
// Enabled when custom_combo_term bit 15 = 1
typedef struct __attribute__((packed)) {
    uint16_t input[4];             // Up to 4 trigger keys (0x0000 = unused)
    uint16_t output;               // Output keycode
    uint16_t custom_combo_term;    // bit 15 = enabled, bits 0-14 = timing (ms)
} argos_combo_entry_t;
_Static_assert(sizeof(argos_combo_entry_t) == 12, "argos_combo_entry_t must be 12 bytes");

// Key Override entry structure (12 bytes)
// Enabled when options bit 7 = 1
typedef struct __attribute__((packed)) {
    uint16_t trigger;              // Trigger keycode
    uint16_t replacement;          // Replacement keycode
    uint32_t layers;               // Layer mask (bit per layer, 32 layers)
    uint8_t  trigger_mods;         // Required modifiers
    uint8_t  negative_mod_mask;    // Modifiers that cancel override
    uint8_t  suppressed_mods;      // Modifiers to suppress
    uint8_t  options;              // Option flags (bit 7 = enabled)
} argos_key_override_entry_t;
_Static_assert(sizeof(argos_key_override_entry_t) == 12, "argos_key_override_entry_t must be 12 bytes");

// Key override option bits
enum argos_key_override_options {
    argos_ko_option_activation_trigger_down         = (1 << 0),
    argos_ko_option_activation_required_mod_down    = (1 << 1),
    argos_ko_option_activation_negative_mod_up      = (1 << 2),
    argos_ko_option_one_mod                         = (1 << 3),
    argos_ko_option_no_reregister_trigger           = (1 << 4),
    argos_ko_option_no_unregister_on_other_key_down = (1 << 5),
    // bit 6 reserved
    argos_ko_enabled                                = (1 << 7),
};

// Alt Repeat Key entry structure (6 bytes)
// Enabled when options bit 3 = 1
typedef struct __attribute__((packed)) {
    uint16_t keycode;              // Original keycode to match
    uint16_t alt_keycode;          // Alternate keycode to send on repeat
    uint8_t  allowed_mods;         // Modifier mask for matching
    uint8_t  options;              // Option flags (bit 3 = enabled)
} argos_alt_repeat_key_entry_t;
_Static_assert(sizeof(argos_alt_repeat_key_entry_t) == 6, "argos_alt_repeat_key_entry_t must be 6 bytes");

// Alt repeat key option bits
enum argos_alt_repeat_key_options {
    argos_ark_option_default_to_alt       = (1 << 0),
    argos_ark_option_bidirectional        = (1 << 1),
    argos_ark_option_ignore_mod_handedness = (1 << 2),
    argos_ark_enabled                     = (1 << 3),
    // bits 4-7 reserved
};

// One-shot settings structure (3 bytes)
typedef struct __attribute__((packed)) {
    uint16_t timeout;      // One-shot timeout in ms (0 = disabled)
    uint8_t  tap_toggle;   // Number of taps to toggle (0 = disabled)
} argos_one_shot_t;
_Static_assert(sizeof(argos_one_shot_t) == 3, "argos_one_shot_t must be 3 bytes");

// EEPROM layout constants - shared across all argos modules
#define ARGOS_TAP_DANCE_OFFSET      0
#define ARGOS_TAP_DANCE_SIZE        (ARGOS_TAP_DANCE_ENTRIES * sizeof(argos_tap_dance_entry_t))

#define ARGOS_COMBO_OFFSET          (ARGOS_TAP_DANCE_OFFSET + ARGOS_TAP_DANCE_SIZE)
#define ARGOS_COMBO_SIZE            (ARGOS_COMBO_ENTRIES * sizeof(argos_combo_entry_t))

#define ARGOS_KEY_OVERRIDE_OFFSET   (ARGOS_COMBO_OFFSET + ARGOS_COMBO_SIZE)
#define ARGOS_KEY_OVERRIDE_SIZE     (ARGOS_KEY_OVERRIDE_ENTRIES * sizeof(argos_key_override_entry_t))

#define ARGOS_ALT_REPEAT_KEY_OFFSET (ARGOS_KEY_OVERRIDE_OFFSET + ARGOS_KEY_OVERRIDE_SIZE)
#define ARGOS_ALT_REPEAT_KEY_SIZE   (ARGOS_ALT_REPEAT_KEY_ENTRIES * sizeof(argos_alt_repeat_key_entry_t))

#define ARGOS_ONE_SHOT_OFFSET       (ARGOS_ALT_REPEAT_KEY_OFFSET + ARGOS_ALT_REPEAT_KEY_SIZE)
#define ARGOS_ONE_SHOT_SIZE         sizeof(argos_one_shot_t)

#define ARGOS_MAGIC_SIZE            6
#define ARGOS_MAGIC_OFFSET          (ARGOS_ONE_SHOT_OFFSET + ARGOS_ONE_SHOT_SIZE)

#define ARGOS_QMK_SETTINGS_OFFSET   (ARGOS_MAGIC_OFFSET + ARGOS_MAGIC_SIZE)

// Total EEPROM size (excluding qmk_settings which has its own size constant)
#define ARGOS_EEPROM_SIZE           (ARGOS_TAP_DANCE_SIZE + ARGOS_COMBO_SIZE + ARGOS_KEY_OVERRIDE_SIZE + ARGOS_ALT_REPEAT_KEY_SIZE + ARGOS_ONE_SHOT_SIZE)

// Public API
void argos_init(void);

// Protocol handler for 0xDF commands
// Returns true if command was handled
bool argos_handle_command(uint8_t *data, uint8_t length);

void argos_read_eeprom(uint16_t offset, void *buf, uint16_t size);
void argos_write_eeprom(uint16_t offset, const void *buf, uint16_t size);

// Storage API - Tap Dance
int argos_get_tap_dance(uint8_t index, argos_tap_dance_entry_t *entry);
int argos_set_tap_dance(uint8_t index, const argos_tap_dance_entry_t *entry);

// Storage API - Combo
int argos_get_combo(uint8_t index, argos_combo_entry_t *entry);
int argos_set_combo(uint8_t index, const argos_combo_entry_t *entry);

// Storage API - Key Override
int argos_get_key_override(uint8_t index, argos_key_override_entry_t *entry);
int argos_set_key_override(uint8_t index, const argos_key_override_entry_t *entry);

// Storage API - Alt Repeat Key
int argos_get_alt_repeat_key(uint8_t index, argos_alt_repeat_key_entry_t *entry);
int argos_set_alt_repeat_key(uint8_t index, const argos_alt_repeat_key_entry_t *entry);

// Storage API - One-Shot
void argos_get_one_shot(argos_one_shot_t *settings);
void argos_set_one_shot(const argos_one_shot_t *settings);

// Administrative functions
void argos_save(void);
void argos_reset(void);

// Get feature flags for protocol info response
uint8_t argos_get_feature_flags(void);

// Reload functions (called after settings change)
void argos_reload_tap_dance(void);
void argos_reload_combo(void);
void argos_reload_key_override(void);
void argos_reload_alt_repeat_key(void);

// Keycode execution helpers
void argos_keycode_down(uint16_t keycode);
void argos_keycode_up(uint16_t keycode);
void argos_keycode_tap(uint16_t keycode);

// Keyboard definition functions
uint32_t argos_get_definition_size(void);
uint8_t argos_get_definition_chunk(uint16_t offset, uint8_t *buffer);

// Weak keyboard hook for post-init
void keyboard_post_init_argos_kb(void);

// Tap dance process_record hook
bool process_record_argos_tap_dance(uint16_t keycode, keyrecord_t *record);
