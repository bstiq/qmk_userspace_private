
#include "quantum.h"
#include "argos.h"
#include "process_tap_dance.h"
#include "argos_tapdance.h"
#include "raw_hid.h"

static uint8_t dance_state[ARGOS_TAP_DANCE_ENTRIES];
// Storage for Argos tap dances
static tap_dance_action_t argos_td_tap_actions[ARGOS_TAP_DANCE_ENTRIES];

// TODO have this also as an array of entries?
static argos_td_entry_t td_entry;

// Check if tap dance entry is enabled (bit 15 of custom_tapping_term)
#define TD_ENABLED(entry) ((entry).custom_tapping_term & 0x8000)


bool listening_for_tap_dance_key = false;
uint8_t listening_tap_dance_keycode_index = 0;
uint8_t listening_tap_dance_index = 0;

argos_td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

static void on_dance(tap_dance_state_t *state, void *user_data) {
    uint8_t index = (uintptr_t)user_data;
    // TODO replace this with loading the array from memory, instead of an EEPROM read...
    if (argos_tap_dance_read_eeprom(index, &td_entry) != 0) // TODO
        return;
    if (!TD_ENABLED(td_entry))
        return;
    uint16_t kc = td_entry.on_tap;
    if (kc) {
        if (state->count == 3) {
            argos_keycode_tap(kc);
            argos_keycode_tap(kc);
            argos_keycode_tap(kc);
        } else if (state->count > 3) {
            argos_keycode_tap(kc);
        }
    }
}

// TODO: dance reset and finished are the same??...
void on_dance_finished(tap_dance_state_t *state, void *user_data) {
    uint8_t index = (uintptr_t)user_data;
    // TODO replace this with loading the array from memory, instead of an EEPROM read...
    if (argos_tap_dance_read_eeprom(index, &td_entry) != 0) // TODO
        return;
    if (!TD_ENABLED(td_entry))
        return;
    dance_state[index] = cur_dance(state);
    switch (dance_state[index]) {
        case TD_SINGLE_TAP: {
            if (td_entry.on_tap)
                argos_keycode_down(td_entry.on_tap);
            break;
        }
        case TD_SINGLE_HOLD: {
            if (td_entry.on_hold)
                argos_keycode_down(td_entry.on_hold);
            else if (td_entry.on_tap)
                argos_keycode_down(td_entry.on_tap);
            break;
        }
        case TD_DOUBLE_TAP: {
            if (td_entry.on_double_tap) {
                argos_keycode_down(td_entry.on_double_tap);
            } else if (td_entry.on_tap) {
                argos_keycode_tap(td_entry.on_tap);
                argos_keycode_down(td_entry.on_tap);
            }
            break;
        }
        case TD_DOUBLE_HOLD: {
            if (td_entry.on_tap_hold) {
                argos_keycode_down(td_entry.on_tap_hold);
            } else {
                if (td_entry.on_tap) {
                    argos_keycode_tap(td_entry.on_tap);
                    if (td_entry.on_hold)
                        argos_keycode_down(td_entry.on_hold);
                    else
                        argos_keycode_down(td_entry.on_tap);
                } else if (td_entry.on_hold) {
                    argos_keycode_down(td_entry.on_hold);
                }
            }
            break;
        }
        case TD_DOUBLE_SINGLE_TAP: {
            if (td_entry.on_tap) {
                argos_keycode_tap(td_entry.on_tap);
                argos_keycode_down(td_entry.on_tap);
            }
            break;
        }
    }
}

void on_dance_reset(tap_dance_state_t *state, void *user_data) {
    uint8_t index = (uintptr_t)user_data;
    if (argos_tap_dance_read_eeprom(index, &td_entry) != 0)
        return;
    if (!TD_ENABLED(td_entry))
        return;
    wait_ms(TAP_CODE_DELAY);
    uint8_t st = dance_state[index];
    state->count = 0;
    dance_state[index] = 0;
    switch (st) {
        case TD_SINGLE_TAP: {
            if (td_entry.on_tap)
                argos_keycode_up(td_entry.on_tap);
            break;
        }
        case TD_SINGLE_HOLD: {
            if (td_entry.on_hold)
                argos_keycode_up(td_entry.on_hold);
            else if (td_entry.on_tap)
                argos_keycode_up(td_entry.on_tap);
            break;
        }
        case TD_DOUBLE_TAP: {
            if (td_entry.on_double_tap) {
                argos_keycode_up(td_entry.on_double_tap);
            } else if (td_entry.on_tap) {
                argos_keycode_up(td_entry.on_tap);
            }
            break;
        }
        case TD_DOUBLE_HOLD: {
            if (td_entry.on_tap_hold) {
                argos_keycode_up(td_entry.on_tap_hold);
            } else {
                if (td_entry.on_tap) {
                    if (td_entry.on_hold)
                        argos_keycode_up(td_entry.on_hold);
                    else
                        argos_keycode_up(td_entry.on_tap);
                } else if (td_entry.on_hold) {
                    argos_keycode_up(td_entry.on_hold);
                }
            }
            break;
        }
        case TD_DOUBLE_SINGLE_TAP: {
            if (td_entry.on_tap) {
                argos_keycode_up(td_entry.on_tap);
            }
            break;
        }
    }
}

// TODO function to reload one specific tap dance
void argos_reload_tap_dances(void) {
    for (size_t i = 0; i < ARGOS_TAP_DANCE_ENTRIES; ++i) {
        argos_td_tap_actions[i].fn.on_each_tap = on_dance;
        argos_td_tap_actions[i].fn.on_dance_finished = on_dance_finished;
        argos_td_tap_actions[i].fn.on_reset = on_dance_reset;
        argos_td_tap_actions[i].fn.on_each_release = NULL;
        argos_td_tap_actions[i].user_data = (void*)(uintptr_t)i;
    }
}

// Override the introspection function
uint16_t tap_dance_count(void) {
    return ARGOS_TAP_DANCE_ENTRIES;
}

// Override the introspection function
tap_dance_action_t* tap_dance_get(uint16_t index) {
    if (index >= ARGOS_TAP_DANCE_ENTRIES) {
        return NULL;
    }
    return &argos_td_tap_actions[index];
}

bool argos_tap_dance_read_eeprom(uint8_t index, argos_td_entry_t *entry) {
    if (index >= ARGOS_TAP_DANCE_ENTRIES) return false;
    printf("Reading tap dance %d from eeprom\n", index);
    argos_read_eeprom(ARGOS_OFFSET_TAP_DANCE + index * sizeof(argos_td_entry_t),
                       entry, sizeof(argos_td_entry_t));
    printf("Data: %d, %d, %d, %d, %d\n", entry->on_tap, entry->on_hold, entry->on_double_tap, entry->on_tap_hold, entry->custom_tapping_term);
    return true;
}

bool argos_tap_dance_write_eeprom(uint8_t index, const argos_td_entry_t *entry) {
    if (index >= ARGOS_TAP_DANCE_ENTRIES) return false;
    printf("Writing tap dance %d to eeprom\n", index);
    printf("Data: %d, %d, %d, %d, %d\n", entry->on_tap, entry->on_hold, entry->on_double_tap, entry->on_tap_hold, entry->custom_tapping_term);
    argos_write_eeprom(ARGOS_OFFSET_TAP_DANCE + index * sizeof(argos_td_entry_t),
                       entry, sizeof(argos_td_entry_t));
    return true;
}

// TODO move the data out of here
void argos_tap_dance_listen_for_key(uint8_t *data) {
    last_activity_time = timer_read32();
    listening_tap_dance_index = data[0];
    listening_tap_dance_keycode_index = data[1]; // 0... 3 
    listening_for_tap_dance_key = true;
    printf("Listening for tap dance key %d\n", listening_tap_dance_index);
}

// TODO code duplication with argos_combo.c
bool process_record_argos_tap_dance(uint16_t keycode, keyrecord_t *record) {
    if (listening_for_tap_dance_key) {
        // Disable listening after 3.5 seconds of inactivity
        if (timer_read32() - last_activity_time > 3500)
            listening_for_tap_dance_key = false;
        else{
            printf("Setting tap dance key %d to keycode %d\n", listening_tap_dance_index, keycode);
            printf("For tap dance index %d\n", listening_tap_dance_index);
            argos_tap_dance_set_keycode(listening_tap_dance_index, keycode,
                                    listening_tap_dance_keycode_index);
            listening_for_tap_dance_key = false;
            return false; // do not process further
        }
    }
    return true;
}

// TODO resets (zero key)
void argos_tap_dance_set_keycode(uint8_t tap_dance_index, uint16_t keycode,
                             uint8_t key_index) {
    // Send back the data to the GUI so it knows we received the command
    uint8_t data[32] = {0};
    data[0] = ARGOS_CMD_PREFIX;
    raw_hid_send(data, sizeof(data));

    printf("Setting tap dance %d key %d to keycode %d\n", tap_dance_index, key_index, keycode);

    if (tap_dance_index >= ARGOS_TAP_DANCE_ENTRIES) return;
    // TODO move this to a table directly instead of reading/writing every time
    argos_td_entry_t entry = {0};
    argos_tap_dance_read_eeprom(tap_dance_index, &entry);
    
    switch(key_index) {
        case 0: {
            entry.on_tap = keycode;
            break;
        }
        case 1: {
            entry.on_hold = keycode;
            break;
        }
        case 2: {
            entry.on_double_tap = keycode;
            break;
        }
        case 3: {
            entry.on_tap_hold = keycode;
            break;
        }
    }
    
    argos_tap_dance_write_eeprom(tap_dance_index, &entry);
    // TODO reload only one tap dance
    // TODO why is this needed?
    argos_reload_tap_dances();
}