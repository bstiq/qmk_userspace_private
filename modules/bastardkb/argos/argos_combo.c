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
        // combo_t *combo = malloc(sizeof(combo_t));
        // argos_read_combo_eeprom(i, combo);
        // if (combo != NULL)
        // {
        //     memcpy(&argos_combos[i], combo, sizeof(combo_t));
        // }
        uint16_t *keys = argos_combo_keys[i];
        argos_combos[i].keys = keys;

        argos_combo_t entry;
        if(argos_read_combo_eeprom(i, &entry)){
            memcpy(keys, entry.keys, sizeof(entry.keys));
            // Ensure null termination
            keys[ARGOS_KEYS_PER_COMBO] = COMBO_END;
            argos_combos[i].keycode = entry.keycode;
            // TODO manage disabled combos... with a custom array probably
        }
            // // No combo stored in eeprom, set to default empty combo
            // keys[0] = COMBO_END;
            // argos_combos[i].keycode = KC_NO;
    }
}

// Override
uint16_t combo_count(void) {
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
        for (int j = 0; j < ARGOS_KEYS_PER_COMBO; j++) {
            comboNew.keys[j] = combo->keys[j];
        }

        // memcpy(&argos_combos[i], combo, sizeof(combo_t));
        argos_set_combo_eeprom(i, &comboNew);
        // we can't reload the combos here (input keys are a const),
        // it will be done later in argos_combos_load_eeprom
        // free(combo);
    }
}

void argos_combo_listen_for_key(uint8_t *data)
{
    listening_combo_index = data[0];
    // 0 for result, 1.... x for combo input
    listening_keycode_index = data[1];
    listening_for_combo_key = true;
}

typedef enum
{
    COMBO_KEY_NOT_PRESSED,
    COMBO_KEY_PRESSED,
    COMBO_KEY_REPRESSED
} combo_key_action_t;

bool process_record_argos_combo(uint16_t keycode, keyrecord_t *record)
{
    if (listening_for_combo_key && record->event.pressed)
    {
        /* ---- Step 1 : Send back the data to the GUI so it knows we received the command */
        uint8_t data[32] = {0};
        data[0] = ARGOS_CMD_PREFIX;
        data[1] = argos_id_capture_combo_key; // Not necessary, but nice
        raw_hid_send(data, sizeof(data));
        listening_for_combo_key = false;

        /* ---- Step 2: Modify the combo */
        argos_combo_t combo;
        argos_read_combo_eeprom(listening_combo_index, &combo);

        // key result
        if(listening_keycode_index == 0)
        {
            combo.keycode = keycode;
        } 
        // key input
        else if ((listening_keycode_index - 1 < ARGOS_KEYS_PER_COMBO)
            && listening_keycode_index - 1 >= 0)
        {
            combo.keys[listening_keycode_index - 1] = keycode;
        }

        /* ---- Step 3: Save the newly created combo in memory */
        argos_set_combo_eeprom(listening_combo_index, &combo);

        /* ---- Step 4: reload combos */
        // We do this because QMK's combo_t structure has a const on the keys, so we need to manually reload it from eeprom
        // TODO only reload the specific combo we just modified.
        // Otherwise it's a lot of eeprom reads...
        argos_combos_load_eeprom();

            //     if (listening_keycode_index == 0)
    //     {
    //         combo_t comboNew = {
    //             .keys = combo.keys,
    //             .keycode = keycode,
    //             .disabled = combo.disabled,
    //             .active = combo.active,
    //             .state = combo.state}; 
    //     }
    //     else if (listening_keycode_index - 1 < ARGOS_KEYS_PER_COMBO)
    //     {
    //         uint16_t key_array[ARGOS_KEYS_PER_COMBO];
    //         for(int i = 0; i < ARGOS_KEYS_PER_COMBO; i++){
    //             key_array[i] = combo.keys[i];
    //         }
    //         key_array[listening_keycode_index - 1] = keycode;
    //         combo_t comboNew = {
    //             .keys = key_array,
    //             .keycode = combo.keycode,
    //             .disabled = combo.disabled,
    //             .active = combo.active,
    //             .state = combo.state};
    //         /* ---- Step 3: Save the newly created combo in memory */
    //         argos_set_combo_eeprom(listening_combo_index, &comboNew);

    //         /* ---- Step 4: reload combos */
    //         // We do this because QMK's combo_t structure has a const on the keys, so we need to manually reload it from eeprom
    //         // TODO: separate into a argos_combo_load_eeprom(index) --> where we set the memory there?
    //         argos_combos_load_eeprom();
    //         // This is one of the input keycodes
    //         // combo.keys[listening_keycode_index - 1] = keycode;
    //         // printf("> Is input %d / %d\n", listening_keycode_index - 1, combo.input[listening_keycode_index - 1]);
    //     }

    }

    //     printf("Captured combo key: %d\n", keycode);

    //     /* ---- Step 2: Create a copy of the combo we want to modify */
    //     argos_combo_t combo;
    //     argos_read_combo_eeprom(listening_combo_index, &combo);

    //     // Now we need to assign the key to the correct combo.
    //     // We need: combo index, key index in the combo, and the captured keycode
    //     // Key index: 0 (result), 1... x (input)
    //     printf("Assigning captured key to combo index %d, key index %d\n", listening_combo_index, listening_keycode_index);
    //     printf("Keycode: %d\n", keycode);
    //     // We already have the combos loaded up, so we just need to
    //     // update the correct keycode and save it back to EEPROM.
    //     // combo_t combo = argos_get_combo(listening_combo_index);
    //     if (listening_keycode_index == 0)
    //     {
    //         combo_t comboNew = {
    //             .keys = combo.keys,
    //             .keycode = keycode,
    //             .disabled = combo.disabled,
    //             .active = combo.active,
    //             .state = combo.state}; 
    //         /* ---- Step 3: Save the newly created combo in memory */
    //         argos_set_combo_eeprom(listening_combo_index, &comboNew);

    //         /* ---- Step 4: reload combos */
    //         // We do this because QMK's combo_t structure has a const on the keys, so we need to manually reload it from eeprom
    //         // TODO: separate into a argos_combo_load_eeprom(index) --> where we set the memory there?
    //         argos_combos_load_eeprom();
    //     }
    //     else if (listening_keycode_index - 1 < ARGOS_KEYS_PER_COMBO)
    //     {
    //         uint16_t key_array[ARGOS_KEYS_PER_COMBO];
    //         for(int i = 0; i < ARGOS_KEYS_PER_COMBO; i++){
    //             key_array[i] = combo.keys[i];
    //         }
    //         key_array[listening_keycode_index - 1] = keycode;
    //         combo_t comboNew = {
    //             .keys = key_array,
    //             .keycode = combo.keycode,
    //             .disabled = combo.disabled,
    //             .active = combo.active,
    //             .state = combo.state};
    //         /* ---- Step 3: Save the newly created combo in memory */
    //         argos_set_combo_eeprom(listening_combo_index, &comboNew);

    //         /* ---- Step 4: reload combos */
    //         // We do this because QMK's combo_t structure has a const on the keys, so we need to manually reload it from eeprom
    //         // TODO: separate into a argos_combo_load_eeprom(index) --> where we set the memory there?
    //         argos_combos_load_eeprom();
    //         // This is one of the input keycodes
    //         // combo.keys[listening_keycode_index - 1] = keycode;
    //         // printf("> Is input %d / %d\n", listening_keycode_index - 1, combo.input[listening_keycode_index - 1]);
    //     }
    //     else
    //     {
    //         printf("Invalid key index in combo capture: %d\n", data[3]);
    //         return false;
    //     }

    //     return false; // this was a capture, do not process keycodes further
    // }
    return true;
}

// TODO sanity check on index?
// combo_t argos_get_combo(uint8_t index)
// {
//     if(index < ARGOS_COMBO_ENTRIES){
//         return argos_combos[index];
//     }
//     return NULL;
// }

bool argos_read_combo_eeprom(uint8_t index, argos_combo_t *combo)
{
    if (index >= ARGOS_COMBO_ENTRIES)
        return false;
    argos_read_eeprom(ARGOS_OFFSET_COMBO + index * sizeof(combo_t),
                      combo, sizeof(combo_t));
    return true;
}

void argos_set_combo_eeprom(uint8_t index, argos_combo_t *combo)
{
    if (index >= ARGOS_COMBO_ENTRIES)
        return;
    argos_write_eeprom(ARGOS_OFFSET_COMBO + index * sizeof(combo_t),
                       combo, sizeof(combo_t));
}