#pragma once

#ifdef POINTING_DEVICE_DRIVER_digitizer
#include "digitizer.h"
#endif

uint16_t bk_pointing_device_get_pointer_default_dpi(void);
void bk_pointing_device_cycle_pointer_default_dpi(bool forward);
void bk_pointing_device_cycle_pointer_default_dpi_noeeprom(bool forward);
void bk_pointing_device_cycle_pointer_sniping_dpi(bool forward);
void bk_pointing_device_cycle_pointer_sniping_dpi_noeeprom(bool forward);
bool bk_pointing_device_get_pointer_sniping_enabled(void);
void bk_pointing_device_set_pointer_sniping_enabled(bool enable);
bool bk_pointing_device_get_pointer_dragscroll_enabled(void);
void bk_pointing_device_set_pointer_dragscroll_enabled(bool enable);
uint16_t bk_pointing_device_get_pointer_sniping_dpi(void);
void keyboard_post_init_bk_pointing_device(void);
void bk_pointing_device_set_auto_mouse_layer_enabled(bool enabled);
void bk_pointing_device_set_auto_precision_on_mouse_layer_enabled(bool enabled);
bool bk_pointing_device_get_auto_mouse_layer_enabled(void);
bool bk_pointing_device_get_auto_precision_on_mouse_layer_enabled(void);
void bk_pointing_device_set_dragscroll_axis_invert_x(bool invert);
void bk_pointing_device_set_dragscroll_axis_invert_y(bool invert);
void bk_pointing_device_set_dragscroll_dpi(uint16_t dpi);
bool bk_pointing_device_get_dragscroll_axis_invert_x(void);
bool bk_pointing_device_get_dragscroll_axis_invert_y(void);
uint16_t bk_pointing_device_get_dragscroll_dpi(void);

#ifdef POINTING_DEVICE_DRIVER_digitizer
bool digitizer_task_kb(digitizer_t *const digitizer_state);
#endif

// NOTE: made to work on branch bkb-pointing-device
