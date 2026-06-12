#include "argos_rgb.h"
#include "argos.h"

static argos_rgb_t argos_rgb_entries[ARGOS_RGB_MATRIX_ENTRIES];

void argos_rgb_init(void) {
    for(int i = 0; i < ARGOS_RGB_MATRIX_ENTRIES; i++) {
        argos_rgb_entries[i] = (argos_rgb_t){0, 0, 0, false, false, false};
    }
    // save zeros in eeprom
    argos_write_eeprom(ARGOS_OFFSET_RGB_MATRIX, argos_rgb_entries, sizeof(argos_rgb_entries));
}

void argos_rgb_load_from_eeprom(void) {
    argos_read_eeprom(ARGOS_OFFSET_RGB_MATRIX, argos_rgb_entries, sizeof(argos_rgb_entries));
}

// static HSV _get_hsv_for_layer_index(uint8_t layer) {
//     switch (layer) {
//         case 1:
//             return (HSV){HSV_BLUE};
//         case 2:
//             return (HSV){HSV_BLUE};
//         case 3:
//             return (HSV){HSV_ORANGE};
//         case 4:
//             return (HSV){HSV_ORANGE};
//         case 5:
//             return (HSV){HSV_TEAL};
//         case 6:
//             return (HSV){HSV_TEAL};
//         case 7:
//         default:
//             return (hsv_t){HSV_RED};
//             break;
//     };
// }

// Layer state indicator
// for now... just a test
bool rgb_matrix_indicators_advanced_argos(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }

    // Set all LEDs to a solid color for highest active layer apart from the base layer.
    // const uint8_t layer = get_highest_layer(layer_state);
    // if (layer > 0) {
    //     HSV hsv = _get_hsv_for_layer_index(layer);

    //     // Set brightness to the configured interval brighter than current brightness, clamped to 255 (ie. uint8_t max value). This compensates for the dimmer appearance of the underglow LEDs.
    //     hsv.v         = MIN(rgb_matrix_get_val(), 255);
    //     const RGB rgb = hsv_to_rgb(hsv);

    //     for (int i = led_min; i < led_max; i++) {
    //         rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    //     }
    // }

    // // Set underglow LEDs to red if caps lock is enabled
    // if (host_keyboard_led_state().caps_lock) {
    //     for (int i = led_min; i <= led_max; i++) {
    //         if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
    //             // set modifier-flagged LEDs to a pure a configured interval brighter than the current brightness, clamped to 255 (ie. uint8_t max value).
    //             rgb_matrix_set_color(i, MIN(rgb_matrix_get_val(), 255), 0, 0);
    //         }
    //     }
    // }

    // we want to go through all the keys of the current layer, and isolate those that are set (custom = false)
    const uint8_t layer = get_highest_layer(layer_state);
    uint16_t min_index = layer * MATRIX_COLS * MATRIX_ROWS;
    for(int i = min_index; i < min_index + MATRIX_COLS * MATRIX_ROWS; i++) {
        if(argos_rgb_entries[i].custom) {
            if(argos_rgb_entries[i].on) {
                if(argos_rgb_entries[i].transparent == false) {
                    rgb_matrix_set_color(i, argos_rgb_entries[i].r, argos_rgb_entries[i].g, argos_rgb_entries[i].b);
                }
            } else {
                rgb_matrix_set_color(i, 0, 0, 0);
            }
        }
    }

    return true;
};

void argos_rgb_set_led_at_position(uint8_t layer, uint8_t row, uint8_t col, uint8_t r, uint8_t g, uint8_t b, bool transparent, bool on, bool custom) {
    uint16_t index = layer * MATRIX_COLS * MATRIX_ROWS + row * MATRIX_COLS + col;
    argos_rgb_entries[index] = (argos_rgb_t){r, g, b, transparent, on, custom};
    argos_write_eeprom(ARGOS_OFFSET_RGB_MATRIX + index * sizeof(argos_rgb_t), &argos_rgb_entries[index], sizeof(argos_rgb_t));
}

// TODO function that reads the whole LED "keymap", just like the regular keymap. It means buffering etc :( my favourite

void argos_rgb_get_led_at_position(uint8_t layer, uint8_t row, uint8_t col, uint8_t *r, uint8_t *g, uint8_t *b, bool *transparent, bool *on, bool *custom) {
    uint16_t index = layer * MATRIX_COLS * MATRIX_ROWS + row * MATRIX_COLS + col;
    argos_rgb_t entry = argos_rgb_entries[index];
    *r = entry.r;
    *g = entry.g;
    *b = entry.b;
    *transparent = entry.transparent;
    *on = entry.on;
    *custom = entry.custom;
}