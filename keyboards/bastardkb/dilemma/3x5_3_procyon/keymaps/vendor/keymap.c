/**
 * Copyright 2022 Charly Delay <charly@codesink.dev> (@0xcharly)
 * Copyright 2023 casuanoob <casuanoob@hotmail.com> (@casuanoob)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"
#include QMK_KEYBOARD_H

enum dilemma_keymap_layers {
    LAYER_BASE = 0,
    LAYER_FUNCTION,
    LAYER_NAVIGATION,
    LAYER_MEDIA,
    LAYER_POINTER,
    LAYER_NUMERAL,
    LAYER_SYMBOLS,
    MAX_LAYERS,
};

// Automatically enable sniping-mode on the pointer layer.
// #define DILEMMA_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#define ESC_MED LT(LAYER_MEDIA, KC_ESC)
#define SPC_NAV LT(LAYER_NAVIGATION, KC_SPC)
#define TAB_FUN LT(LAYER_FUNCTION, KC_TAB)
#define ENT_SYM LT(LAYER_SYMBOLS, KC_ENT)
#define BSP_NUM LT(LAYER_NUMERAL, KC_BSPC)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// QP stuff
#include "qp.h"
#include "qp_comms.h"
#include "qp_st77xx_opcodes.h"
#include "gfx/POC.qgf.h"
#include "gfx/bar_blue.qgf.h"
#include "gfx/bar_gray.qgf.h"
#include "gfx/bar_green.qgf.h"
#include "gfx/fonts.qff.h"
#include "qp_surface.h"
#include "keymap.h"
#include "color.h"

painter_device_t        lcd;
static painter_device_t surface;
// Buffer required for a 240x280 16bpp surface:
static uint8_t surface_buffer[SURFACE_REQUIRED_BUFFER_BYTE_SIZE(LCD_WIDTH, LCD_HEIGHT, 16)];
// end QP stuff

// clang-format off
/** \brief QWERTY layout (3 rows, 10 columns). */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_split_3x5_3(
       KC_Q,    KC_W,    KC_E,    KC_CAPS,    DRG_TOG,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
       LGUI_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F), KC_G, KC_H, LSFT_T(KC_J), LCTL_T(KC_K), LALT_T(KC_L), LGUI_T(KC_QUOT),
       PT_Z,    RALT_T(KC_X),    KC_C,    KC_V,    KC_B,    KC_N,    KC_M, KC_COMM,  RALT_T(KC_DOT), PT_SLSH,
                      ESC_MED, TAB_FUN, SPC_NAV, ENT_SYM, BSP_NUM, KC_MUTE
  ),

/*
 * Layers used on the Dilemma.
 *
 * These layers started off heavily inspired by the Miryoku layout, but trimmed
 * down and tailored for a stock experience that is meant to be fundation for
 * further personalization.
 *
 * See https://github.com/manna-harbour/miryoku for the original layout.
 */

/**
 * \brief Function layer.
 *
 * Secondary right-hand layer has function keys mirroring the numerals on the
 * primary layer with extras on the pinkie column, plus system keys on the inner
 * column. App is on the tertiary thumb key and other thumb keys are duplicated
 * from the base layer to enable auto-repeat.
 */
  [LAYER_FUNCTION] = LAYOUT_split_3x5_3(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PSCR,   KC_F7,   KC_F8,   KC_F9,  KC_F12,
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_SCRL,   KC_F4,   KC_F5,   KC_F6,  KC_F11,
    XXXXXXX, KC_RALT, XXXXXXX, XXXXXXX, XXXXXXX, KC_PAUS,   KC_F1,   KC_F2,   KC_F3,  KC_F10,
                      XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
  ),

/**
 * \brief Navigation layer.
 *
 * Primary right-hand layer (left home thumb) is navigation and editing. Cursor
 * keys are on the home position, line and page movement below, clipboard above,
 * caps lock and insert on the inner column. Thumb keys are duplicated from the
 * base layer to avoid having to layer change mid edit and to enable auto-repeat.
 */
  [LAYER_NAVIGATION] = LAYOUT_split_3x5_3(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_CAPS, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT,
    XXXXXXX, KC_RALT, XXXXXXX, XXXXXXX, XXXXXXX,  KC_INS, KC_HOME, KC_PGDN, KC_PGUP,  KC_END,
                      XXXXXXX, XXXXXXX, _______,  KC_ENT, KC_BSPC, KC_DEL
  ),

/**
 * \brief Media layer.
 *
 * Tertiary left- and right-hand layer is media and RGB control.  This layer is
 * symmetrical to accomodate the left- and right-hand trackball.
 */
  [LAYER_MEDIA] = LAYOUT_split_3x5_3(
    XXXXXXX, RM_PREV, RM_TOGG, RM_NEXT, XXXXXXX, XXXXXXX, RM_PREV, RM_TOGG, RM_NEXT, XXXXXXX,
    KC_MPRV, KC_VOLD, KC_MUTE, KC_VOLU, KC_MNXT, KC_MPRV, KC_VOLD, KC_MUTE, KC_VOLU, KC_MNXT,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                      _______, KC_MPLY, KC_MSTP, KC_MSTP, KC_MPLY, KC_MUTE
  ),

/** \brief Mouse emulation and pointer functions. */
  [LAYER_POINTER] = LAYOUT_split_3x5_3(
    QK_BOOT,  EE_CLR, XXXXXXX, DPI_MOD, S_D_MOD, S_D_MOD, DPI_MOD, XXXXXXX,  EE_CLR, QK_BOOT,
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
    _______, DRGSCRL, SNIPING, MS_BTN3, XXXXXXX, XXXXXXX, MS_BTN3, SNIPING, DRGSCRL, _______,
                      MS_BTN3, MS_BTN2, MS_BTN1, MS_BTN1, MS_BTN2, MS_BTN3
  ),

/**
 * \brief Numeral layout.
 *
 * Primary left-hand layer (right home thumb) is numerals and symbols. Numerals
 * are in the standard numpad locations with symbols in the remaining positions.
 * `KC_DOT` is duplicated from the base layer.
 */
  [LAYER_NUMERAL] = LAYOUT_split_3x5_3(
    KC_LBRC,    KC_7,    KC_8,    KC_9, KC_RBRC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_SCLN,    KC_4,    KC_5,    KC_6,  KC_EQL, XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
     KC_DOT,    KC_1,    KC_2,    KC_3, KC_BSLS, XXXXXXX, XXXXXXX, XXXXXXX, KC_RALT, XXXXXXX,
                       KC_DOT, KC_MINS,    KC_0, XXXXXXX, _______, XXXXXXX
  ),

/**
 * \brief Symbols layer.
 *
 * Secondary left-hand layer has shifted symbols in the same locations to reduce
 * chording when using mods with shifted symbols. `KC_LPRN` is duplicated next to
 * `KC_RPRN`.
 */
  [LAYER_SYMBOLS] = LAYOUT_split_3x5_3(
    KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_COLN,  KC_DLR, KC_PERC, KC_CIRC, KC_PLUS, XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
    KC_TILD, KC_EXLM,   KC_AT, KC_HASH, KC_PIPE, XXXXXXX, XXXXXXX, XXXXXXX, KC_RALT, XXXXXXX,
                      KC_RPRN,  KC_GRV, KC_UNDS, _______, XXXXXXX, XXXXXXX
  ),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef DILEMMA_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    dilemma_set_pointer_sniping_enabled(layer_state_cmp(state, DILEMMA_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // DILEMMA_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef ENCODER_MAP_ENABLE
// clang-format off
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [LAYER_BASE]       = {ENCODER_CCW_CW(MS_WHLD, MS_WHLU),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_FUNCTION]   = {ENCODER_CCW_CW(KC_DOWN, KC_UP),    ENCODER_CCW_CW(KC_LEFT, KC_RGHT)},
    [LAYER_NAVIGATION] = {ENCODER_CCW_CW(KC_PGDN, KC_PGUP),  ENCODER_CCW_CW(KC_VOLU, KC_VOLD)},
    [LAYER_MEDIA]      = {ENCODER_CCW_CW(KC_PGDN, KC_PGUP),  ENCODER_CCW_CW(KC_VOLU, KC_VOLD)},
    [LAYER_POINTER]    = {ENCODER_CCW_CW(RM_HUED, RM_HUEU),  ENCODER_CCW_CW(RM_SATD, RM_SATU)},
    [LAYER_NUMERAL]    = {ENCODER_CCW_CW(RM_VALD, RM_VALU),  ENCODER_CCW_CW(RM_SPDD, RM_SPDU)},
    [LAYER_SYMBOLS]    = {ENCODER_CCW_CW(RM_PREV, RM_NEXT),  ENCODER_CCW_CW(KC_LEFT, KC_RGHT)},
};

// clang-format on
#endif // ENCODER_MAP_ENABLE

void keyboard_post_init_user(void) {
    // if (is_keyboard_left()) {
    // Display timeout
    wait_ms(LCD_WAIT_TIME);

    lcd = qp_st7789_make_spi_device(LCD_WIDTH, LCD_HEIGHT, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, LCD_SPI_DIVISOR, SPI_MODE);
    qp_init(lcd, LCD_ROTATION);

    surface = qp_make_rgb565_surface(LCD_WIDTH, LCD_HEIGHT, surface_buffer);
    qp_init(surface, LCD_ROTATION);

    // Display offset
    qp_set_viewport_offsets(lcd, LCD_OFFSET_X, LCD_OFFSET_Y);

    // load fonts
    bk_font_layer    = qp_load_font_mem(font_gridlitepbslayer);
    bk_font_menu     = qp_load_font_mem(font_gridlitepbsmenu);
    bk_font_menu_off = qp_load_font_mem(font_gridlitepbsmenuoff);

    // Power on display, fill with black
    qp_power(lcd, 1);
    qp_rect(lcd, 0, 0, 300, 300, HSV_BLACK, 1);
    qp_flush(lcd);

    prev_layer = 99;
    // last_mods  = UINT8_MAX;
    // bk_display_layer_number();
    // keyboard_post_init_user();
    // }
}

void housekeeping_task_user(void) {
    static uint32_t anim_timer = 0;
    if (is_keyboard_left()) {
        if (timer_elapsed32(anim_timer) > 200) {
            const uint8_t layer = get_highest_layer(layer_state);
            // const uint8_t mods  = get_mods();
            anim_timer          = timer_read32();
            if (prev_layer != layer) {
                qp_clear(surface);
                bk_display_layer_name(BKS_LAYER_X, BKS_LAYER_Y, layer, bk_font_layer);
                bk_display_layer_info(BKS_LAYER_X, BKS_LAYER_Y + bk_font_layer->line_height + 5, layer, bk_font_menu, TRUE);
                qp_surface_draw(surface, lcd, 0, 0, false); 
            } else {
                bk_display_layer_info(BKS_LAYER_X, BKS_LAYER_Y + bk_font_layer->line_height + 5, layer, bk_font_menu, TRUE);
                qp_surface_draw(surface, lcd, 0, 0, false); 
            }

            prev_layer = layer;
        }
    }

    qp_drawtext(lcd, 0, 0, bk_font_layer, "TEST JERO");
}

void bk_display_layer_name(int x, int y, int layer, painter_font_handle_t font) {
    hsv_t color = _get_hsv_for_layer_index(layer);
    qp_rect(surface, 0, 0, BKS_LAYER_BAR_W, LCD_HEIGHT, color.h, color.s, color.v, true);
    qp_drawtext(surface, x, y, font, bk_layer_str(layer));
}

int bk_display_layer_info(int x, int y, int layer, painter_font_handle_t font, bool rewrite_all) {
    int current_y = y;
    switch (layer) {
        case LAYER_FUNCTION:
            current_y = bk_display_info_base(x, current_y, bk_font_menu, bk_font_menu_off, rewrite_all);
            break;
        case LAYER_NAVIGATION:
            current_y = bk_display_info_base(x, current_y, bk_font_menu, bk_font_menu_off, rewrite_all);
            break;
        case LAYER_MEDIA:
            current_y = bk_display_info_media(x, current_y, bk_font_menu, bk_font_menu_off, rewrite_all);
            break;
        case LAYER_POINTER:
            current_y = bk_display_info_pointer(x, current_y, bk_font_menu, bk_font_menu_off, rewrite_all);
            break;
        case LAYER_NUMERAL:
            current_y = bk_display_info_base(x, current_y, bk_font_menu, bk_font_menu_off, rewrite_all);
            break;
        case LAYER_SYMBOLS:
            current_y = bk_display_info_base(x, current_y, bk_font_menu, bk_font_menu_off, rewrite_all);
            break;
        case LAYER_BASE:
        default:
            current_y = bk_display_info_base(x, current_y, bk_font_menu, bk_font_menu_off, rewrite_all);
            break;
    }
    return current_y;
}

int bk_display_info_media(uint16_t x, uint16_t y, painter_font_handle_t font_on, painter_font_handle_t font_off, bool render_all) {
    int     current_y = y;
    uint8_t rgb       = rgb_matrix_is_enabled();
    int     mods_x    = 0;

    qp_drawtext(surface, x, current_y, font_on, "RGB");
    mods_x = qp_textwidth(font_on, "RGB ") + x;

    if (rgb) {
        qp_drawtext(surface, mods_x, current_y, font_on, "ON ");
    } else {
        qp_drawtext(surface, mods_x, current_y, font_off, "OFF");
    }

    current_y += font_on->line_height + 5;

    return current_y;
}

int bk_display_info_pointer(uint16_t x, uint16_t y, painter_font_handle_t font_on, painter_font_handle_t font_off, bool render_all) {
    int current_y = y;

    // Mods
    qp_drawtext(surface, x, current_y, font_on, "MODS");
    int mods_x = qp_textwidth(font_on, "MODS ") + x;

    qp_drawtext(surface, mods_x, current_y, (dilemma_get_pointer_sniping_enabled()) ? font_on : font_off, "SNIPE");
    qp_drawtext(surface, mods_x, current_y + font_on->line_height + 5, (dilemma_get_pointer_dragscroll_enabled()) ? font_on : font_off, "SCROLL");
    current_y += font_on->line_height * 2 + 10;
    // End Mods

    // DPI info
    char dpi[50];
    sprintf(dpi, "%u", dilemma_get_pointer_default_dpi());
    qp_drawtext(surface, x, current_y, font_on, "DPI");
    mods_x = qp_textwidth(font_on, "DPI ") + x;
    qp_drawtext(surface, mods_x, current_y, font_off, dpi);
    current_y += font_on->line_height + 5;

    char s_dpi[50];
    sprintf(s_dpi, "%u", dilemma_get_pointer_sniping_dpi());
    qp_drawtext(surface, x, current_y, font_on, "SP. DPI");
    mods_x = qp_textwidth(font_on, "SP. DPI ") + x;
    qp_drawtext(surface, mods_x, current_y, font_off, s_dpi);
    current_y += font_on->line_height + 5;

    // End DPI info

    return current_y;
}

int bk_display_info_base(uint16_t x, uint16_t y, painter_font_handle_t font_on, painter_font_handle_t font_off, bool render_all) {
    int     current_y       = y;
    int     mod_column_size = 0;
    uint8_t mods            = get_mods();

    // Mods info
    qp_drawtext(surface, x, current_y, font_on, "MODS");
    int mods_x = qp_textwidth(font_on, "MODS ") + x;
    // current_y += bk_layer_base_mods(qp_textwidth(font, "MODS ") + x, current_y, bk_font_menu, bk_font_menu_off, rewrite_all);
    mod_column_size = qp_textwidth(font_on, "XXXXX");

    // if (((mods & MOD_MASK_GUI) != (last_mods & MOD_MASK_GUI)) || render_all) {
        qp_drawtext(surface, mods_x, current_y, (mods & MOD_MASK_GUI) ? font_on : font_off, "GUI");
    // }
    // if (((mods & MOD_MASK_ALT) != (last_mods & MOD_MASK_ALT)) || render_all) {
        qp_drawtext(surface, mods_x + mod_column_size, current_y, (mods & MOD_MASK_ALT) ? font_on : font_off, "ALT");
    // }

    // if (((mods & MOD_MASK_CTRL) != (last_mods & MOD_MASK_CTRL)) || render_all) {
        qp_drawtext(surface, mods_x, current_y + font_on->line_height + 5, (mods & MOD_MASK_CTRL) ? font_on : font_off, "CTRL");
    // }

    // if (((mods & MOD_MASK_SHIFT) != (last_mods & MOD_MASK_SHIFT)) || render_all) {
        qp_drawtext(surface, mods_x + mod_column_size, current_y + font_on->line_height + 5, (mods & MOD_MASK_SHIFT) ? font_on : font_off, "SHFT");
    // }

    current_y += font_on->line_height * 2 + 5;
    // End Mods section

    // Lock info
    qp_drawtext(surface, x, current_y, font_on, "LOCK");
    // current_y += bk_layer_base_lock(qp_textwidth(font, "LOCK ") + x, current_y, bk_font_menu, bk_font_menu_off, rewrite_all);

    mods_x          = qp_textwidth(font_on, "LOCK ") + x;
    mod_column_size = qp_textwidth(font_on, "XXXXX");

    qp_drawtext(surface, mods_x, current_y, (host_keyboard_led_state().caps_lock) ? font_on : font_off, "CAPS");
    qp_drawtext(surface, mods_x + mod_column_size, current_y, (dilemma_get_pointer_dragscroll_enabled()) ? font_on : font_off, "SCRL");

    current_y += font_on->line_height;

    return current_y;
    // End Lock info
}

const char *bk_layer_str(enum dilemma_keymap_layers layer) {
    if (layer > MAX_LAYERS) {
        layer = 0;
    }
    return layer_strings[layer];
}
