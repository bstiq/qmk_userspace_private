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
    We will use this to store our own custom combos,
    but also to send to QMK's combo processing functions.
    So it's a lot easier to follow the same data structure
*/
static combo_t argos_combos[ARGOS_COMBO_ENTRIES];

bool listening_for_combo_key = false;
uint8_t listening_keycode_index = 0;
uint8_t listening_combo_index = 0;

// TODO deal with disabled combos?
// TODO deal with NULL combos?... right now we set everything to zero
// or... just communicate them to the webapp and let it handle it
void argos_combos_load_eeprom()
{
    // initialize all combos
    memset(argos_combos, 0, sizeof(argos_combos));
    for (int i = 0; i < ARGOS_COMBO_ENTRIES; i++)
    {
        combo_t *combo = malloc(sizeof(combo_t));
        argos_read_combo_eeprom(i, combo);
        if (combo != NULL)
        {
            // printf("loaded combo %d:", i);
            // printf(" > enabled=%d", combo->enabled);
            // printf(" > output=%d", combo->output);
            // printf(" > keys=[");
            // for (int j = 0; j < ARGOS_KEYS_PER_COMBO; j++)
            // {
            //     printf("%d ", combo->input[j]);
            // }
            // printf("]\n");
            // argos_combos[i] = combo;
            memcpy(&argos_combos[i], combo, sizeof(combo_t));
        }
        // ... continue
        // argos_combos[i] = combo;
        // memcpy(&argos_combos[i], &combo, sizeof(combo_t));

        // if (combo != NULL) {
        //     argos_combos[i] = *combo;
        // } else {
        //     // if read fails, initialize to default
        //     memset(&argos_combos[i], 0, sizeof(combo_t));
        // }
    }
}

/*
    Since we have custom combos loaded, we want QMK to process those instead of the ones set during compilation.
    To do this, QMK provides a handy weak function: combo_get
    We can override it, and return our custom combos
*/
combo_t* combo_get(uint16_t combo_idx) {
    if (combo_idx >= ARGOS_COMBO_ENTRIES) {
        return NULL;
    }
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
    
        memcpy(&argos_combos[i], combo, sizeof(combo_t));

        argos_set_combo_eeprom(i, combo);
        free(combo);
    }
}

void argos_combo_listen_for_key(uint8_t *data)
{
    listening_combo_index = data[0];
    // 0 for result, 1.... x for combo input
    listening_keycode_index = data[1];
    listening_for_combo_key = true;
}

/*
    Intercept keycodes for combos
    Based on QMK's OG code, but using our own combos.
    We have to do this manually because QMK doesn't store combos in EEPROM / can't dynamically add/modify combos
    This is a bunch of extra work and a whole lot of code duplication, and hopefully in the future it won't be needed

    There are some limitations, chosen on purpose to simplify things.
    It makes the available customization more limited, but makes the implementation simpler.
    Managing those would mean additional work on both QMK Module side and WebApp side.
    Later, we might want to either implement those on both WebApp and QMK Module side, or WebApp side only if QMK manages it
    - 
*/

typedef enum { COMBO_KEY_NOT_PRESSED, COMBO_KEY_PRESSED, COMBO_KEY_REPRESSED } combo_key_action_t;

bool process_record_argos_combo(uint16_t keycode, keyrecord_t *record)
{
    // if (listening_for_combo_key && record->event.pressed)
    // {
    //     // we will send the captured keycode in the next response to the capture_combo_key command
    //     uint8_t data[32] = {0};
    //     data[0] = ARGOS_CMD_PREFIX;
    //     data[1] = argos_id_capture_combo_key;
    //     data[2] = listening_combo_index;
    //     data[3] = listening_keycode_index;
    //     data[4] = keycode & 0xFF;
    //     data[5] = (keycode >> 8) & 0xFF;

    //     // Fill between 4... to listening_for_key_command_length with zeros:
    //     // TODO is this really necessary?
    //     for (int i = 6; i < 32; i++)
    //     {
    //         data[i] = 0;
    //     }
    //     raw_hid_send(data, sizeof(data));
    //     listening_for_combo_key = false;
    //     printf("Captured combo key: %d\n", keycode);

    //     // Now we need to assign the key to the correct combo.
    //     // We need: combo index, key index in the combo, and the captured keycode
    //     // Key index: 0 (result), 1... x (input)

    //     printf("Assigning captured key to combo index %d, key index %d\n", data[2], data[3] - 1);
    //     printf("Keycode: %d\n", keycode);
    //     // We already have the combos loaded up, so we just need to
    //     // update the correct keycode and save it back to EEPROM.
    //     // combo_t combo = argos_get_combo(listening_combo_index);
    //     if (listening_keycode_index == 0)
    //     {
    //         // This is the output keycode
    //         argos_combos[listening_combo_index].output = keycode;
    //         // printf("> Is output: %d\n", combo.output);
    //     }
    //     else if (listening_keycode_index - 1 < ARGOS_KEYS_PER_COMBO)
    //     {
    //         // This is one of the input keycodes
    //         argos_combos[listening_combo_index].input[listening_keycode_index - 1] = keycode;
    //         // printf("> Is input %d / %d\n", listening_keycode_index - 1, combo.input[listening_keycode_index - 1]);
    //     }
    //     else
    //     {
    //         printf("Invalid key index in combo capture: %d\n", data[3]);
    //         return false;
    //     }

    //     // save combo in memory
    //     argos_set_combo_eeprom(listening_combo_index, &argos_combos[listening_combo_index]);

    //     return false; // this was a capture, do not process keycodes further
    // }
    return true;
}

// TODO sanity check on index?
combo_t argos_get_combo(uint8_t index)
{
    return argos_combos[index];
}

combo_t *argos_read_combo_eeprom(uint8_t index, combo_t *combo)
{
    if (index >= ARGOS_COMBO_ENTRIES)
        return NULL;
    argos_read_eeprom(ARGOS_OFFSET_COMBO + index * sizeof(combo_t),
                      combo, sizeof(combo_t));
    return combo;
}

void argos_set_combo_eeprom(uint8_t index, combo_t *combo)
{
    if (index >= ARGOS_COMBO_ENTRIES)
        return;
    argos_write_eeprom(ARGOS_OFFSET_COMBO + index * sizeof(combo_t),
                       combo, sizeof(combo_t));
}