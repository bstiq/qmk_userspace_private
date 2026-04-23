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

/*
    To make migrating from QMK as easy as possible, on first load we copy
    over the combos.
    QMK does not store combos in eeprom, so we have to load them using combo_get_raw
    and then manually copy each one into eeprom through our custom data structure
*/
void argos_copy_combos_from_QMK(void){
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

// void argos_copy_combos_QMK(){

// }

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