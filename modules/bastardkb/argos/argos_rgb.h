#include "quantum.h"

// bool rgb_matrix_indicators_advanced_module(uint8_t led_min, uint8_t led_max);
void argos_rgb_init(void);
void argos_rgb_load_from_eeprom(void);
void argos_rgb_set_led_at_position(uint8_t layer, uint8_t row, uint8_t col, uint8_t r, uint8_t g, uint8_t b, bool transparent, bool on, bool custom, uint8_t offset);
void argos_rgb_get_led_at_position(uint8_t layer, uint8_t row, uint8_t col, uint8_t *r, uint8_t *g, uint8_t *b, bool *transparent, bool *on, bool *custom);

// for override purposes
// bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max);