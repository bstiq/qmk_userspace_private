// Copyright 2026 Quentin LEBASTARD <bstkbd@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
    Here we do a bit of shenanigans.
    I can't find a way to dynamically pull the amount of combo entries and size of a combo entry
    from argos.h, so instead we set it manually.
    TODO: fix.
*/
#ifndef ARGOS_COMBO_ENTRIES
#    define ARGOS_COMBO_ENTRIES 16 // this was already defined in argos.h, TODO fix this hardcoding...
#endif

// TODO: why do we have to define this again here? when it's already been done in argos.h...
#ifndef ARGOS_SIZE_COMBO
#    define ARGOS_SIZE_COMBO 14 // sizeof(combo_t) - TODO fix this hardcoding
#endif

#ifndef ARGOS_SIZE_HAS_COPIED_QMK
#    define ARGOS_SIZE_HAS_COPIED_QMK sizeof(bool)
#endif

// TODO size of combo for argos_combo_entries instead of hardcoding 12
#define ARGOS_EEPROM_SIZE_CALC (\
    ARGOS_SIZE_HAS_COPIED_QMK + \
    ARGOS_COMBO_ENTRIES * ARGOS_SIZE_COMBO \
    + 3 + 6)

// Reduce max address for dynamic keymap to ensure we don't overlap with Argos' EEPROM storage
// much easier than trying to set the start address.
#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR (TOTAL_EEPROM_BYTE_COUNT - 1 - ARGOS_EEPROM_SIZE_CALC)