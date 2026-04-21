// Copyright 2025 Ira Cooper <ira@wakeful.net>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "argos.h"
#include "quantum.h"

#ifdef COMBO_ENABLE

#include "process_combo.h"

// Bit mask for enabled flag in custom_combo_term
#define ARGOS_COMBO_ENABLED_BIT 0x8000
// Mask for timing value (bits 0-14)
#define ARGOS_COMBO_TIMING_MASK 0x7FFF

// Storage for combo key sequences (4 keys + COMBO_END terminator)
static uint16_t argos_combo_keys[ARGOS_COMBO_ENTRIES][5];

// Storage for combo structures
static combo_t argos_combos[ARGOS_COMBO_ENTRIES];

// Storage for custom combo terms (0 = use global default)
static uint16_t argos_combo_terms[ARGOS_COMBO_ENTRIES];

// Track which combos are enabled
static bool argos_combo_enabled[ARGOS_COMBO_ENTRIES];

void argos_reload_combo(void) {
    // Initialize with all keys = COMBO_END
    memset(argos_combo_keys, 0, sizeof(argos_combo_keys));
    memset(argos_combos, 0, sizeof(argos_combos));
    memset(argos_combo_terms, 0, sizeof(argos_combo_terms));
    memset(argos_combo_enabled, 0, sizeof(argos_combo_enabled));

    // Load from EEPROM
    for (size_t i = 0; i < ARGOS_COMBO_ENTRIES; ++i) {
        uint16_t *seq = argos_combo_keys[i];
        argos_combos[i].keys = seq;

        argos_combo_entry_t entry;
        if (argos_get_combo(i, &entry) == 0) {
            // Check if enabled (bit 15 of custom_combo_term)
            argos_combo_enabled[i] = (entry.custom_combo_term & ARGOS_COMBO_ENABLED_BIT) != 0;

            if (argos_combo_enabled[i]) {
                memcpy(seq, entry.input, sizeof(entry.input));
                // Ensure null termination
                seq[4] = COMBO_END;
                argos_combos[i].keycode = entry.output;

                // Extract custom timing (bits 0-14), 0 means use global default
                argos_combo_terms[i] = entry.custom_combo_term & ARGOS_COMBO_TIMING_MASK;
            } else {
                // Disabled combo: empty key sequence
                seq[0] = COMBO_END;
                argos_combos[i].keycode = KC_NO;
            }
        }
    }
}

// Override the introspection functions
uint16_t combo_count(void) {
    return ARGOS_COMBO_ENTRIES;
}

combo_t* combo_get(uint16_t combo_idx) {
    if (combo_idx >= ARGOS_COMBO_ENTRIES) {
        return NULL;
    }
    return &argos_combos[combo_idx];
}

// User hook: override this for custom per-combo timing logic
// Return 0 to use Argos's setting, or a positive value to override
__attribute__((weak)) uint16_t get_combo_term_argos(uint16_t combo_idx, combo_t *combo) {
    return 0;  // Default: use Argos's setting
}

// Argos owns this function - user hook is checked FIRST
uint16_t get_combo_term(uint16_t combo_idx, combo_t *combo) {
    // User hook gets first priority
    uint16_t user_term = get_combo_term_argos(combo_idx, combo);
    if (user_term > 0) {
        return user_term;
    }

    // Then check for per-combo custom timing from Argos
    if (combo_idx < ARGOS_COMBO_ENTRIES && argos_combo_terms[combo_idx] > 0) {
        return argos_combo_terms[combo_idx];
    }

    // Fall back to Argos's global setting
    return argos_get_combo_term();
}

#else
// Stubs when COMBO_ENABLE is not defined
void argos_reload_combo(void) {}
#endif
