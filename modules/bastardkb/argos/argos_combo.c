// Copyright 2026 Quentin LEBASTARD <bstkbd@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "argos.h"
#include "quantum.h"
#include "via.h"
#include "raw_hid.h"
#include "eeprom.h"
#include "version.h"
#include "eeconfig.h"
#include "nvm_eeprom_eeconfig_internal.h"
#include "nvm_eeprom_via_internal.h"
#include "keymap_introspection.h"
#include <time.h>
#include <stdlib.h>

#include "process_combo.h"
#include "argos_combo.h"

/*
    argos_combos is used to store the combos
    in the same way that QMK does.
    It is loaded from eeprom, and read from eeprom.
    The limitation is that we can't modify the input keys (const),
    So instead we use a custom data storage of definite size.
*/
static combo_t argos_combos[ARGOS_COMBO_ENTRIES];
// +1 for COMBO_END terminator
static uint16_t argos_combo_keys[ARGOS_COMBO_ENTRIES][ARGOS_KEYS_PER_COMBO + 1];

bool listening_for_combo_key = false;
uint8_t listening_keycode_index = 0;
uint8_t listening_combo_index = 0;

// TODO deal with disabled combos?
// TODO deal with NULL combos?... right now we set everything to zero
// or... just communicate them to the webapp and let it handle it
void argos_combos_load_eeprom()
{
    // initialize all combos
    memset(argos_combo_keys, 0, sizeof(argos_combo_keys));
    memset(argos_combos, 0, sizeof(argos_combos));

    for (int i = 0; i < ARGOS_COMBO_ENTRIES; i++)
    {
        argos_combo_load_eeprom(i);
    }
}

void argos_combo_load_eeprom(uint8_t index)
{
    uint16_t *keys = argos_combo_keys[index];
    argos_combos[index].keys = keys;

    argos_combo_t entry;
    if (argos_combo_read_eeprom(index, &entry))
    {
        memcpy(keys, entry.keys, sizeof(entry.keys));
        // Ensure null termination
        keys[ARGOS_KEYS_PER_COMBO] = COMBO_END;
        argos_combos[index].keycode = entry.keycode;
        // TODO manage disabled combos... with a custom array probably
    }
}

// Override
uint16_t combo_count(void)
{
    return ARGOS_COMBO_ENTRIES;
}

/*
    Since we have custom combos loaded, we want QMK to process those instead of the ones set during compilation.
    To do this, QMK provides a handy weak function: combo_get
    We can override it, and return our custom combos
*/
combo_t *combo_get(uint16_t combo_idx)
{
    if (combo_idx >= ARGOS_COMBO_ENTRIES)
    {
        return NULL;
    }
    // we need to convert out
    return &argos_combos[combo_idx];
}

/*
We use regular combos -->
    typedef struct combo_t {
        const uint16_t *keys;
        uint16_t        keycode;
        bool     disabled;
        bool     active;
        uint8_t state;
    }
*/
// Copies QMK's combos into eeprom, so that we can modify them later.
// Done only once ever
void argos_combos_copy_from_QMK(void)
{
    // TODO for now we will load all combos even if they're set to zero.
    // Later we need to find a way on how to stop when we reach the end.
    for (int i = 0; i < ARGOS_COMBO_ENTRIES; i++)
    {
        combo_t *combo = combo_get_raw(i);
        if (combo == NULL)
        {
            // TODO test some kind of zero assignment to the keycode result maybe?
            return; // no more combos
        }

        // we need to convert from combo_t to argos_combo_t before saving to eeprom
        argos_combo_t comboNew = {
            .keys = {0},
            .keycode = combo->keycode,
            .disabled = combo->disabled,
            .active = combo->active,
            .state = combo->state};
        for (int j = 0; j < ARGOS_KEYS_PER_COMBO; j++)
        {
            comboNew.keys[j] = combo->keys[j];
        }

        argos_combo_write_eeprom(i, &comboNew);
    }
}

void argos_combo_listen_for_key(uint8_t *data)
{
    listening_combo_index = data[0];
    // 0 for result, 1.... x for combo input
    listening_keycode_index = data[1];
    listening_for_combo_key = true;
}

void argos_combo_reset_capturing_combo_key_index(uint8_t index)
{
    argos_combo_set_keycode(listening_combo_index, 0, listening_keycode_index);
    // TODO: compact the combo so we don't have any "Zeros" in there.
}

void argos_combo_set_keycode(uint8_t combo_index, uint16_t keycode, uint8_t key_index)
{
    // Send back the data to the GUI so it knows we received the command
    uint8_t data[32] = {0};
    data[0] = ARGOS_CMD_PREFIX;
    raw_hid_send(data, sizeof(data));

    printf("Setting combo %d key index %d to keycode %d\n", combo_index, key_index, keycode);

    argos_combo_t combo;
    argos_combo_read_eeprom(listening_combo_index, &combo);

    // key result
    if (listening_keycode_index == 0)
    {
        combo.keycode = keycode;
    }
    // key input
    else if ((listening_keycode_index - 1 < ARGOS_KEYS_PER_COMBO) && listening_keycode_index - 1 >= 0)
    {
        combo.keys[listening_keycode_index - 1] = keycode;
    }

    // TODO: reorganize keys to make sure there's no 0x0000 in the middle...

    // Save the newly created combo in memory
    argos_combo_write_eeprom(listening_combo_index, &combo);

    // Reload combo
    uint16_t *keys = argos_combo_keys[combo_index];
    argos_combos[combo_index].keys = keys;

    memcpy(keys, combo.keys, sizeof(combo.keys));
    // Ensure null termination
    keys[ARGOS_KEYS_PER_COMBO] = COMBO_END;
    argos_combos[combo_index].keycode = combo.keycode;

    listening_for_combo_key = false;
}

bool process_record_argos_combo(uint16_t keycode, keyrecord_t *record)
{
    if (listening_for_combo_key && record->event.pressed)
    {
        argos_combo_set_keycode(listening_combo_index, keycode, listening_keycode_index);
    }
    return true;
}

// TODO sanity check on index?
combo_t argos_combo_get(uint8_t index)
{
    if (index < ARGOS_COMBO_ENTRIES)
    {
        return argos_combos[index];
    }
    return (combo_t){0};
}

bool argos_combo_read_eeprom(uint8_t index, argos_combo_t *combo)
{
    if (index >= ARGOS_COMBO_ENTRIES)
        return false;
    argos_read_eeprom(ARGOS_OFFSET_COMBO + index * sizeof(combo_t),
                      combo, sizeof(combo_t));
    return true;
}

void argos_combo_write_eeprom(uint8_t index, argos_combo_t *combo)
{
    if (index >= ARGOS_COMBO_ENTRIES)
        return;
    argos_write_eeprom(ARGOS_OFFSET_COMBO + index * sizeof(combo_t),
                       combo, sizeof(combo_t));
}