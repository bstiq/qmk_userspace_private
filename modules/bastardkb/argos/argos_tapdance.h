
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_MORE_TAPS
} argos_td_state_t;

void argos_reload_tap_dances(void);
bool argos_tap_dance_read_eeprom(uint8_t index, argos_td_entry_t *entry);
bool argos_tap_dance_write_eeprom(uint8_t index, const argos_td_entry_t *entry);
void argos_tap_dance_listen_for_key(uint8_t *data);
bool process_record_argos_tap_dance(uint16_t keycode, keyrecord_t *record);
void argos_tap_dance_set_keycode(uint8_t tap_dance_index, uint16_t keycode, uint8_t key_index);
void argos_tap_dance_reset_capturing_tap_dance_key_index(uint8_t index);