
#include "quantum.h"
#include "argos.h"
#include "process_tap_dance.h"

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} argos_td_state_t;

typedef struct {
    bool is_press_action;
    argos_td_state_t state;
} argos_td_tap_t;

static argos_td_tap_t td_state[ARGOS_TAP_DANCE_ENTRIES];
static argos_td_entry_t td_entry;

// Check if tap dance entry is enabled (bit 15 of custom_tapping_term)
#define TD_ENABLED(entry) ((entry).custom_tapping_term & 0x8000)


/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicative that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currently not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustrating to type.
 *
 * For the third point, there does exist the 'TD_DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
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

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static argos_td_tap_t xtap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static void on_dance(tap_dance_state_t *state, void *user_data) {
    uint8_t index = (uintptr_t)user_data;
    if (argos_get_tap_dance(index, &td_entry) != 0) // TODO
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
    if (argos_get_tap_dance(index, &td_entry) != 0) // TODO
        return;
    if (!TD_ENABLED(td_entry))
        return;
    dance_state[index] = dance_step(state);
    switch (dance_state[index]) {
        case SINGLE_TAP: {
            if (td_entry.on_tap)
                argos_keycode_down(td_entry.on_tap);
            break;
        }
        case SINGLE_HOLD: {
            if (td_entry.on_hold)
                argos_keycode_down(td_entry.on_hold);
            else if (td_entry.on_tap)
                argos_keycode_down(td_entry.on_tap);
            break;
        }
        case DOUBLE_TAP: {
            if (td_entry.on_double_tap) {
                argos_keycode_down(td_entry.on_double_tap);
            } else if (td_entry.on_tap) {
                argos_keycode_tap(td_entry.on_tap);
                argos_keycode_down(td_entry.on_tap);
            }
            break;
        }
        case DOUBLE_HOLD: {
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
        case DOUBLE_SINGLE_TAP: {
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
    if (viable_get_tap_dance(index, &td_entry) != 0)
        return;
    if (!TD_ENABLED(td_entry))
        return;
    wait_ms(TAP_CODE_DELAY);
    uint8_t st = dance_state[index];
    state->count = 0;
    dance_state[index] = 0;
    switch (st) {
        case SINGLE_TAP: {
            if (td_entry.on_tap)
                viable_keycode_up(td_entry.on_tap);
            break;
        }
        case SINGLE_HOLD: {
            if (td_entry.on_hold)
                viable_keycode_up(td_entry.on_hold);
            else if (td_entry.on_tap)
                viable_keycode_up(td_entry.on_tap);
            break;
        }
        case DOUBLE_TAP: {
            if (td_entry.on_double_tap) {
                viable_keycode_up(td_entry.on_double_tap);
            } else if (td_entry.on_tap) {
                viable_keycode_up(td_entry.on_tap);
            }
            break;
        }
        case DOUBLE_HOLD: {
            if (td_entry.on_tap_hold) {
                viable_keycode_up(td_entry.on_tap_hold);
            } else {
                if (td_entry.on_tap) {
                    if (td_entry.on_hold)
                        viable_keycode_up(td_entry.on_hold);
                    else
                        viable_keycode_up(td_entry.on_tap);
                } else if (td_entry.on_hold) {
                    viable_keycode_up(td_entry.on_hold);
                }
            }
            break;
        }
        case DOUBLE_SINGLE_TAP: {
            if (td_entry.on_tap) {
                viable_keycode_up(td_entry.on_tap);
            }
            break;
        }
    }
}

// Storage for Argos tap dances
static tap_dance_action_t argos_td_tap_actions[ARGOS_TAP_DANCE_ENTRIES];

void argos_init_tap_dances(void) {
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
    argos_read_eeprom(ARGOS_TAP_DANCE_OFFSET + index * sizeof(argos_td_entry_t),
                       entry, sizeof(argos_td_entry_t));
    return true;
}

bool argos_tap_dance_write_eeprom(uint8_t index, const argos_td_entry_t *entry) {
    if (index >= ARGOS_TAP_DANCE_ENTRIES) return false;
    argos_write_eeprom(ARGOS_TAP_DANCE_OFFSET + index * sizeof(argos_td_entry_t),
                       entry, sizeof(argos_td_entry_t));
    return true;
}