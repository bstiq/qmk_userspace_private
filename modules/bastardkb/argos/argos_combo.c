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

#include "argos_combo.h"

static argos_combo_t argos_combos[ARGOS_COMBO_ENTRIES];

bool listening_for_combo_key = false;
uint8_t listening_keycode_index = 0;
uint8_t listening_combo_index = 0;

// TODO deal with disabled combos?
// TODO deal with NULL combos?... right now we set everything to zero
// or... just communicate them to the webapp and let it handle it
void argos_load_combos_eeprom(){     
    // initialize all combos
    memset(argos_combos, 0, sizeof(argos_combos));
    for(int i = 0; i < ARGOS_COMBO_ENTRIES; i++){
        argos_combo_t* combo = malloc(sizeof(argos_combo_t));
        argos_read_combo_eeprom(i, combo);
        if(combo != NULL){
            printf("loaded combo %d:", i);
            printf(" > enabled=%d", combo->enabled);
            printf(" > output=%d", combo->output);
            printf(" > keys=[");
            for(int j = 0; j < ARGOS_KEYS_PER_COMBO; j++){
                printf("%d ", combo->input[j]);
            }
            printf("]\n");
            // argos_combos[i] = combo;
            memcpy(&argos_combos[i], combo, sizeof(argos_combo_t));
        }
        // ... continue
            // argos_combos[i] = combo;
        // memcpy(&argos_combos[i], &combo, sizeof(argos_combo_t));

        // if (combo != NULL) {
        //     argos_combos[i] = *combo;
        // } else {
        //     // if read fails, initialize to default
        //     memset(&argos_combos[i], 0, sizeof(argos_combo_t));
        // }
    }
}

void argos_combo_copy_from_QMK(void){
    // TODO for now we will load all combos even if they're set to zero.
    // Later we need to find a way on how to stop when we reach the end.
    for(int i = 0; i < ARGOS_COMBO_ENTRIES; i++){
        combo_t* combo = combo_get_raw(i);
        if (combo == NULL) {
            // TODO test some kind of zero assignment to the keycode result maybe?
            break; // no more combos
        }

        argos_combo_t* argos_combo = malloc(sizeof(argos_combo_t));
        argos_combo->enabled = !(combo->disabled); // fix an interesting design choice
        argos_combo->output = combo->keycode;
        for (int j = 0; j < ARGOS_KEYS_PER_COMBO; j++) {
            argos_combo->input[j] = combo->keys[j];
        }
        argos_combo->custom_combo_term = 0;

        argos_set_combo_eeprom(i, argos_combo);
        free(argos_combo);
    }
}

void argos_combo_listen_for_key(uint8_t* data){
    listening_combo_index = data[0];
    // 0 for result, 1.... x for combo input
    listening_keycode_index = data[1];
    listening_for_combo_key = true;
}

bool process_record_argos_combo(uint16_t keycode, keyrecord_t *record){
    if (listening_for_combo_key && record->event.pressed) {
        // we will send the captured keycode in the next response to the capture_combo_key command
        uint8_t data[32] = {0};
        data[0] = ARGOS_CMD_PREFIX;
        data[1] = argos_id_capture_combo_key;
        data[2] = listening_combo_index;
        data[3] = listening_keycode_index;
        data[4] = keycode & 0xFF;
        data[5] = (keycode >> 8) & 0xFF;

        // Fill between 4... to listening_for_key_command_length with zeros:
        // TODO is this really necessary?
        for(int i = 6; i < 32; i++){
            data[i] = 0;
        }
        raw_hid_send(data, sizeof(data));
        listening_for_combo_key = false;
        printf("Captured combo key: %d\n", keycode);

        // Now we need to assign the key to the correct combo.
        // We need: combo index, key index in the combo, and the captured keycode
        // Key index: 0 (result), 1... x (input)

        printf("Assigning captured key to combo index %d, key index %d\n", data[2], data[3] - 1);
        printf("Keycode: %d\n", keycode);
        // We already have the combos loaded up, so we just need to
        // update the correct keycode and save it back to EEPROM.
        // argos_combo_t combo = argos_get_combo(listening_combo_index);
        if (listening_keycode_index == 0) {
            // This is the output keycode
            argos_combos[listening_combo_index].output = keycode;
            // printf("> Is output: %d\n", combo.output);
        } else if (listening_keycode_index - 1 < ARGOS_KEYS_PER_COMBO) {
            // This is one of the input keycodes 
            argos_combos[listening_combo_index].input[listening_keycode_index - 1] = keycode;
            // printf("> Is input %d / %d\n", listening_keycode_index - 1, combo.input[listening_keycode_index - 1]);
        } else {
            printf("Invalid key index in combo capture: %d\n", data[3]);
            return false;
        }

        // save combo in memory
        argos_set_combo_eeprom(listening_combo_index, &argos_combos[listening_combo_index]);

        return false; // this was a capture, do not process keycodes further
    }
    return true;
}

// TODO sanity check on index?
argos_combo_t argos_get_combo(uint8_t index){
    return argos_combos[index];
}

argos_combo_t* argos_read_combo_eeprom(uint8_t index, argos_combo_t* combo){
    if (index >= ARGOS_COMBO_ENTRIES) return NULL;
    argos_read_eeprom(ARGOS_OFFSET_COMBO + index * sizeof(argos_combo_t),
                       combo, sizeof(argos_combo_t));
    return combo;
}

void argos_set_combo_eeprom(uint8_t index, argos_combo_t* combo){
    if (index >= ARGOS_COMBO_ENTRIES) return;
    argos_write_eeprom(ARGOS_OFFSET_COMBO + index * sizeof(argos_combo_t),
                       combo, sizeof(argos_combo_t));
}