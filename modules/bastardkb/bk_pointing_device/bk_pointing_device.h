#pragma once

#include QMK_KEYBOARD_H
#include "quantum.h"

// NOTE: made to work on branch bkb-pointing-device

#ifdef POINTING_DEVICE_ENABLE
#    ifndef NO_BK_POINTING_DEVICE_KEYCODES
enum bk_pointing_device_keycodes {
    BK_POINTING_DEVICE_DEFAULT_DPI_FORWARD = QK_KB_0,
    BK_POINTING_DEVICE_DEFAULT_DPI_REVERSE,
    BK_POINTING_DEVICE_SNIPING_DPI_FORWARD,
    BK_POINTING_DEVICE_SNIPING_DPI_REVERSE,
    BK_POINTING_DEVICE_SNIPING,
    BK_POINTING_DEVICE_SNIPING_TOGGLE,
    BK_POINTING_DEVICE_DRAGSCROLL_MODE,
    BK_POINTING_DEVICE_DRAGSCROLL_MODE_TOGGLE,
};
#define DPI_MOD BK_POINTING_DEVICE_DEFAULT_DPI_FORWARD
#define DPI_RMOD BK_POINTING_DEVICE_DEFAULT_DPI_REVERSE
#define S_D_MOD BK_POINTING_DEVICE_SNIPING_DPI_FORWARD
#define S_D_RMOD BK_POINTING_DEVICE_SNIPING_DPI_REVERSE
#define SNIPING BK_POINTING_DEVICE_SNIPING
#define SNP_TOG BK_POINTING_DEVICE_SNIPING_TOGGLE
#define DRGSCRL BK_POINTING_DEVICE_DRAGSCROLL_MODE
#define DRG_TOG BK_POINTING_DEVICE_DRAGSCROLL_MODE_TOGGLE
#    endif // !NO_BK_POINTING_DEVICE_KEYCODES

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
#endif // BK_POINTING_DEVICE_ENABLE
