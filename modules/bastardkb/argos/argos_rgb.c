#include "argos_rgb.h"
#include "argos.h"
#include "transactions.h"

static argos_rgb_t argos_rgb_entries[ARGOS_RGB_MATRIX_ENTRIES];

void argos_rgb_init(void) {
    // layer 0 is transparent
    for(int i = 0; i < RGB_ENTRIES_PER_LAYER; i++) {
        argos_rgb_entries[i] = (argos_rgb_t){0, 0, 0, false, false, false};
    }
    // default: per-layer rgb
    for(int layer = 1; layer < 10; layer++) {
        // pick 10 different colors, easier to do in HSV
        HSV hsv = (HSV){layer * 360 / 10, 255, 255};
        RGB rgb = hsv_to_rgb(hsv);
        for(int i = 0; i < RGB_ENTRIES_PER_LAYER; i++) {
            argos_rgb_entries[layer * RGB_ENTRIES_PER_LAYER + i] = (argos_rgb_t){rgb.r, rgb.g, rgb.b, false, true, true};
        }
    }
    // save in eeprom
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

    const uint8_t layer = get_highest_layer(layer_state);
    const uint16_t min_index = layer * RGB_ENTRIES_PER_LAYER;

    // printf("min: %d, max: %d\n", led_min, led_max);
    
    for(int i = led_min; i < led_max; i++) {
        const uint16_t index = min_index + i;
        // printf("Testing rgb entry index %d\n", index);
        if(argos_rgb_entries[index].custom) {
            if(argos_rgb_entries[index].on) {
                if(argos_rgb_entries[index].passthrough == false) {
                    rgb_matrix_set_color(i, argos_rgb_entries[index].r, argos_rgb_entries[index].g, argos_rgb_entries[index].b);
                }
            } else {
                rgb_matrix_set_color(i, 0, 0, 0);
            }
        }
    }
    
    return true;
}

// The rgb module code is called BEFORE the KB code, so we need to override the KB code.
// We can't do that, so instead we override the user code, so that the keyboard code detects it and does not execute.
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    return false;
}

/*
    TODO: we don't need row and col here.
*/
void argos_rgb_set_led_at_position(uint8_t layer, uint8_t row, uint8_t col, uint8_t r, uint8_t g, uint8_t b, bool passthrough, bool on, bool custom, uint8_t offset, uint8_t index) {
    if (is_keyboard_master()) {
        uint16_t baseIndex = layer * RGB_ENTRIES_PER_LAYER;
        uint16_t keyIndex = baseIndex + index + offset;
        argos_rgb_handle_set_led_at_position(keyIndex, r, g, b, passthrough, on, custom);
        // we need to send over: keyindex, r, g, b, passthrough, on, custom. uint16 + uint8, we'll send everything as uint16t to make it more simple.
        uint16_t data[] = {keyIndex, r, g, b, passthrough, on, custom};

        transaction_rpc_send(RPC_ID_RGB_SYNC, sizeof(data), data);
    }
}

void argos_rgb_handle_set_led_at_position(uint16_t keyIndex, uint8_t r, uint8_t g, uint8_t b, bool passthrough, bool on, bool custom) {
    // printf("Setting RGB matrix LED at position %d\n", keyIndex);
    // printf("Rows: %d, Cols: %d, Offset: %d, Index: %d\n", MATRIX_ROWS, MATRIX_COLS, offset, index);
    argos_rgb_entries[keyIndex] = (argos_rgb_t){r, g, b, passthrough, on, custom};
    // TODO this is a lot of writes potentially
    argos_write_eeprom(ARGOS_OFFSET_RGB_MATRIX + keyIndex * sizeof(argos_rgb_t), &argos_rgb_entries[keyIndex], sizeof(argos_rgb_t));
}

// secondary side
void rgb_sync_handler(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    // if (!is_keyboard_master()) {
    //     if (initiator2target_buffer_size == sizeof(dilemma_status_t)) {
    //         dilemma_status = *(const dilemma_status_t*)initiator2target_buffer;
    //     }
    // }
    if(!is_keyboard_master()) {
        // TODO check data size
        uint16_t *data = (uint16_t*)initiator2target_buffer;
        uint16_t keyIndex = data[0];
        uint8_t r = data[1];
        uint8_t g = data[2];
        uint8_t b = data[3];
        bool passthrough = data[4];
        bool on = data[5];
        bool custom = data[6];
        argos_rgb_handle_set_led_at_position(keyIndex, r, g, b, passthrough, on, custom);
    }
}

// TODO function that reads the whole LED "keymap", just like the regular keymap. It means buffering etc :( my favourite

void argos_rgb_get_led_at_position(argos_rgb_t *entry, uint8_t layer, uint8_t index, uint8_t offset) {
    uint16_t baseIndex = layer * RGB_ENTRIES_PER_LAYER;
    *entry = argos_rgb_entries[baseIndex + index + offset];
}