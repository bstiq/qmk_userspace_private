// Copyright 2026 Quentin LEBASTARD <bstkbd@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
    Here we do a bit of shenanigans.
    I can't find a way to dynamically pull the amount of combo entries and size of a combo entry
    from argos.h, so instead we set it manually.
    TODO: fix.
*/

#define ARGOS_OFFSET_CONFIG 0
#define ARGOS_SIZE_CONFIG 7

#define ARGOS_OFFSET_COMBO (ARGOS_OFFSET_CONFIG + ARGOS_SIZE_CONFIG)
#define ARGOS_COMBO_ENTRIES 16 // this was already defined in argos.h, TODO fix this hardcoding...
#define ARGOS_SIZE_COMBO 13
#define ARGOS_SIZE_COMBOS (ARGOS_COMBO_ENTRIES * ARGOS_SIZE_COMBO)

#define ARGOS_OFFSET_TAP_DANCE (ARGOS_OFFSET_COMBO + ARGOS_SIZE_COMBOS)
#define ARGOS_TAP_DANCE_ENTRIES 50 // should be enough for anyone
#define ARGOS_SIZE_TAP_DANCE 11
#define ARGOS_SIZE_TAP_DANCES (ARGOS_TAP_DANCE_ENTRIES * ARGOS_SIZE_TAP_DANCE)

#define ARGOS_OFFSET_RGB_MATRIX (ARGOS_OFFSET_TAP_DANCE + ARGOS_SIZE_TAP_DANCES)
#if defined(POINTING_DEVICE_DRIVER_digitizer) || defined(CIRQUE_PINNACLE_DIAMETER_MM)
// dilemma, with underglow
#define RGB_ENTRIES_PER_LAYER MATRIX_COLS*MATRIX_ROWS*2
#else
// others, no underglow
#define RGB_ENTRIES_PER_LAYER MATRIX_COLS*MATRIX_ROWS
#endif
#define ARGOS_RGB_MATRIX_ENTRIES RGB_ENTRIES_PER_LAYER*10 // up to 10 layers supported
#define ARGOS_SIZE_RGB_MATRIX_KEY_DATA 5
#define ARGOS_SIZE_RGB_MATRIX_ENTRIES (ARGOS_RGB_MATRIX_ENTRIES * ARGOS_SIZE_RGB_MATRIX_KEY_DATA)

#define ARGOS_EEPROM_SIZE_CALC (\
    ARGOS_SIZE_CONFIG + \
    ARGOS_SIZE_COMBOS + \
    ARGOS_SIZE_TAP_DANCES + \
    ARGOS_SIZE_RGB_MATRIX_ENTRIES)

// Reduce max address for dynamic keymap to ensure we don't overlap with Argos' EEPROM storage
// much easier than trying to set the start address.
#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR (TOTAL_EEPROM_BYTE_COUNT - 1 - ARGOS_EEPROM_SIZE_CALC)