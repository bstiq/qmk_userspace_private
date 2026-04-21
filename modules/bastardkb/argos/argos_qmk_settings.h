// Copyright 2025 Ira Cooper <ira@wakeful.net>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>

// Initialize QMK settings (load from EEPROM and apply)
void argos_qmk_settings_init(void);

// Query supported QSIDs greater than qsid_gt
void argos_qmk_settings_query(uint16_t qsid_gt, uint8_t *buffer, uint8_t length);

// Get a setting value (returns 0 on success)
int argos_qmk_settings_get(uint16_t qsid, uint8_t *buffer, uint8_t length);

// Set a setting value (returns 0 on success)
int argos_qmk_settings_set(uint16_t qsid, const uint8_t *data, uint8_t length);

// Reset all settings to defaults
void argos_qmk_settings_reset(void);

// Getters for global settings (used by other argos modules as fallback)
uint16_t argos_get_tapping_term(void);
uint16_t argos_get_combo_term(void);
