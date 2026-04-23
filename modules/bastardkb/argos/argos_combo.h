// Copyright 2026 Quentin LEBASTARD <bstkbd@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "action.h"

void argos_load_combos_eeprom(void);
void argos_combo_copy_from_QMK(void);
argos_combo_t* argos_read_combo_eeprom(uint8_t index, argos_combo_t* combo);
void argos_set_combo_eeprom(uint8_t index, argos_combo_t* combo);
argos_combo_t argos_get_combo(uint8_t index);
void argos_combo_listen_for_key(uint8_t* data);
bool process_record_argos_combo(uint16_t keycode, keyrecord_t *record);