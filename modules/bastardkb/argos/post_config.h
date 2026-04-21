// Copyright 2025 Ira Cooper <ira@wakeful.net>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Calculate EECONFIG_KB_DATA_SIZE for Argos storage
// Must be set before eeconfig.h is processed

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

// QMK settings storage size (argos_qmk_settings_t)
#define ARGOS_QMK_SETTINGS_SIZE 40

// Total size: tap_dance*10 + combo*12 + key_override*12 + alt_repeat*6 + one_shot(3) + magic(6) + qmk_settings(40)
#define ARGOS_EEPROM_SIZE_CALC ( \
    (ARGOS_TAP_DANCE_ENTRIES * 10) + \
    (ARGOS_COMBO_ENTRIES * 12) + \
    (ARGOS_KEY_OVERRIDE_ENTRIES * 12) + \
    (ARGOS_ALT_REPEAT_KEY_ENTRIES * 6) + \
    3 + 6 + ARGOS_QMK_SETTINGS_SIZE)

// Reduce max address for dynamic keymap to ensure we don't overlap with Argos's EEPROM storage
// much easier than trying to set the start address.
#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR (TOTAL_EEPROM_BYTE_COUNT - 1 - ARGOS_EEPROM_SIZE_CALC)
