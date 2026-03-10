/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
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
#pragma once

#define DYNAMIC_KEYMAP_LAYER_COUNT 8

#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE

#define ENCODER_RESOLUTION 4

// QP stuff
#define SPI_SCK_PIN GP22  // as per vik connector
#define SPI_MOSI_PIN GP23 // as per vik connector
#define SPI_MISO_PIN GP18 // Unused
#define LCD_RST_PIN GP16  // Unused, TODO connect to Vcc for now, test with SCL later
#define LCD_DC_PIN GP12   // vik_gp1
#define LCD_CS_PIN GP11   // vik_gp2
#define LCD_BLK_PIN GP4   // not used
#ifdef BACKLIGHT_LEVELS
#    undef BACKLIGHT_LEVELS
#endif
#define BACKLIGHT_LEVELS 16
#define BACKLIGHT_PIN GP20 // backlight pin for lcd
#define BACKLIGHT_PWM_DRIVER PWMD2
#define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_A

#define SPI_DRIVER SPID0
#define LCD_SPI_DIVISOR 2
#define LCD_WAIT_TIME 150
#define LCD_ROTATION QP_ROTATION_0
#define LCD_OFFSET_X 0
#define LCD_OFFSET_Y 15
#define LCD_WIDTH 240  // Set according to your display specs
#define LCD_HEIGHT 280 // Set according to your display specs
#define SPI_MODE 3     // Set according to your display specs
#define ST7789         // Set according to your display specs GC_9A01 or ST7789
#ifdef QUANTUM_PAINTER_SUPPORTS_NATIVE_COLORS
#    undef QUANTUM_PAINTER_SUPPORTS_NATIVE_COLORS
#endif
#define QUANTUM_PAINTER_SUPPORTS_NATIVE_COLORS TRUE
// Timeout configuration, default 30000 (30 sek). 0 = No timeout. Beware of image retention.
#ifdef QUANTUM_PAINTER_DISPLAY_TIMEOUT
#    undef QUANTUM_PAINTER_DISPLAY_TIMEOUT
#endif
#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 0
// #define QUANTUM_PAINTER_PIXDATA_BUFFER_SIZE 4096
#ifdef QUANTUM_PAINTER_TASK_THROTTLE
#    undef QUANTUM_PAINTER_TASK_THROTTLE
#endif
#define QUANTUM_PAINTER_TASK_THROTTLE 16 // 60 FPS ish
#define RGB_MATRIX_MODE_NAME_ENABLE
#ifdef QP_LVGL_TASK_PERIOD
#    undef QP_LVGL_TASK_PERIOD
#endif
#define QP_LVGL_TASK_PERIOD 33 // throttle lvgl for keyboard responsiveness

// #define QUANTUM_PAINTER_DISPLAY_TIMEOUT 5000

// Custom display stuff
#define BK_PALETTE LV_PALETTE_CYAN