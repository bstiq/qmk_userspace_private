/*
 * Copyright 2020 Christopher Courtney <drashna@live.com> (@drashna)
 * Copyright 2021 Quentin LEBASTARD <qlebastard@gmail.com>
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Publicw License as published by
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

 /*
    Generic pointing device configuration and features.
 */

#include QMK_KEYBOARD_H
 #include "bk_pointing_device.h"
 #include "transactions.h"
 #include <string.h>
 #include "math.h"
 
 #ifdef CONSOLE_ENABLE
 #    include "print.h"
 #endif // CONSOLE_ENABLE

 #ifdef COMMUNITY_MODULE_ARGOS_ENABLE
 #include "argos.h"
 #endif

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 0, 0);
 
// ........................... imported from charybdis.c
 
 #define BK_POINTING_DEVICE_MINIMUM_DEFAULT_DPI 400
 #define BK_POINTING_DEVICE_DEFAULT_DPI_CONFIG_STEP 200
 #define BK_POINTING_DEVICE_MINIMUM_SNIPING_DPI 200
 #define BK_POINTING_DEVICE_SNIPING_DPI_CONFIG_STEP 100
 #define BK_POINTING_DEVICE_BK_POINTING_DEVICE_DRAGSCROLL_DPI 100
 #define BK_POINTING_DEVICE_BK_POINTING_DEVICE_DRAGSCROLL_BUFFER_SIZE 6
 
 typedef union {
     uint8_t raw;
     struct {
         uint8_t pointer_default_dpi : 4; // 16 steps available.
         uint8_t pointer_sniping_dpi : 2; // 4 steps available.
         bool    is_dragscroll_enabled : 1;
         bool    is_sniping_enabled : 1;
     } __attribute__((packed));
 } bk_pointing_device_config_t;
 
 static bk_pointing_device_config_t g_bk_pointing_device_config = {0};
 
 /**
  * \brief Set the value of `config` from EEPROM.
  *
  * Note that `is_dragscroll_enabled` and `is_sniping_enabled` are purposefully
  * ignored since we do not want to persist this state to memory.  In practice,
  * this state is always written to maximize write-performances.  Therefore, we
  * explicitly set them to `false` in this function.
  */
static void read_bk_pointing_device_config_from_eeprom(bk_pointing_device_config_t* config) {
    // TODO: replace with per-module memory management
#ifdef COMMUNITY_MODULE_ARGOS_ENABLE
    argos_read_eeprom(ARGOS_OFFSET_POINTER_CONFIG, config, sizeof(bk_pointing_device_config_t));
#else
     config->raw                   = eeconfig_read_kb() & 0xff;
#endif
     config->is_dragscroll_enabled = false;
     config->is_sniping_enabled    = false;
     printf("Read DPI: %d\n", config->pointer_default_dpi);
     printf("ReadSNIPING DPI: %d\n", config->pointer_sniping_dpi);
 }
 
 /**
  * \brief Save the value of `config` to eeprom.
  *
  * Note that all values are written verbatim, including whether drag-scroll
  * and/or sniper mode are enabled.  `read_bk_pointing_device_config_from_eeprom(…)`
  * resets these 2 values to `false` since it does not make sense to persist
  * these across reboots of the board.
  */
 static void write_bk_pointing_device_config_to_eeprom(bk_pointing_device_config_t* config) {
    printf("Writing DPI: %d\n", config->pointer_default_dpi);
    printf("Writing SNIPING DPI: %d\n", config->pointer_sniping_dpi);
    // TODO: replace with per-module memory management
#ifdef ARGOS_OFFSET_POINTER_CONFIG
    argos_write_eeprom(ARGOS_OFFSET_POINTER_CONFIG, config, sizeof(bk_pointing_device_config_t));
#else
    eeconfig_update_kb(config->raw);
#endif
 }
 
 /** \brief Return the current value of the pointer's default DPI. */
 static uint16_t get_pointer_default_dpi(bk_pointing_device_config_t* config) {
     return (uint16_t)config->pointer_default_dpi * BK_POINTING_DEVICE_DEFAULT_DPI_CONFIG_STEP + BK_POINTING_DEVICE_MINIMUM_DEFAULT_DPI;
 }
 
 /** \brief Return the current value of the pointer's sniper-mode DPI. */
 static uint16_t get_pointer_sniping_dpi(bk_pointing_device_config_t* config) {
     return (uint16_t)config->pointer_sniping_dpi * BK_POINTING_DEVICE_SNIPING_DPI_CONFIG_STEP + BK_POINTING_DEVICE_MINIMUM_SNIPING_DPI;
 }
 
 /** \brief Set the appropriate DPI for the input config. */
 static void maybe_update_bk_pointing_device_cpi(bk_pointing_device_config_t* config) {
     if (config->is_dragscroll_enabled) {
        printf("Set Dragscroll DPI: %d\n", BK_POINTING_DEVICE_BK_POINTING_DEVICE_DRAGSCROLL_DPI);
         pointing_device_set_cpi(BK_POINTING_DEVICE_BK_POINTING_DEVICE_DRAGSCROLL_DPI);
     } else if (config->is_sniping_enabled) {
        printf("Set Sniping DPI: %d\n", get_pointer_sniping_dpi(config));
         pointing_device_set_cpi(get_pointer_sniping_dpi(config));
     } else {
         printf("Set Default DPI: %d\n", get_pointer_default_dpi(config));
         pointing_device_set_cpi(get_pointer_default_dpi(config));
     }
 }
 
 /**
  * \brief Update the pointer's default DPI to the next or previous step.
  *
  * Increases the DPI value if `forward` is `true`, decreases it otherwise.
  * The increment/decrement steps are equal to BK_BK_POINTING_DEVICE_DEFAULT_DPI_CONFIG_STEP.
  */
 static void step_pointer_default_dpi(bk_pointing_device_config_t* config, bool forward) {
     config->pointer_default_dpi += forward ? 1 : -1;
     printf("Set DPI: %d\n", config->pointer_default_dpi);
     maybe_update_bk_pointing_device_cpi(config);
 }
 
 /**
  * \brief Update the pointer's sniper-mode DPI to the next or previous step.
  *
  * Increases the DPI value if `forward` is `true`, decreases it otherwise.
  * The increment/decrement steps are equal to BK_POINTING_DEVICE_SNIPING_DPI_CONFIG_STEP.
  */
 static void step_pointer_sniping_dpi(bk_pointing_device_config_t* config, bool forward) {
     config->pointer_sniping_dpi += forward ? 1 : -1;
     maybe_update_bk_pointing_device_cpi(config);
 }
 
 uint16_t bk_pointing_device_get_pointer_default_dpi(void) {
     return get_pointer_default_dpi(&g_bk_pointing_device_config);
 }
 
 uint16_t bk_pointing_device_get_pointer_sniping_dpi(void) {
     return get_pointer_sniping_dpi(&g_bk_pointing_device_config);
 }
 
 void bk_pointing_device_cycle_pointer_default_dpi_noeeprom(bool forward) {
     step_pointer_default_dpi(&g_bk_pointing_device_config, forward);
 }
 
 void bk_pointing_device_cycle_pointer_default_dpi(bool forward) {
     step_pointer_default_dpi(&g_bk_pointing_device_config, forward);
     write_bk_pointing_device_config_to_eeprom(&g_bk_pointing_device_config);
 }
 
 void bk_pointing_device_cycle_pointer_sniping_dpi_noeeprom(bool forward) {
     step_pointer_sniping_dpi(&g_bk_pointing_device_config, forward);
 }
 
 void bk_pointing_device_cycle_pointer_sniping_dpi(bool forward) {
     step_pointer_sniping_dpi(&g_bk_pointing_device_config, forward);
     write_bk_pointing_device_config_to_eeprom(&g_bk_pointing_device_config);
 }
 
 bool bk_pointing_device_get_pointer_sniping_enabled(void) {
     return g_bk_pointing_device_config.is_sniping_enabled;
 }
 
 void bk_pointing_device_set_pointer_sniping_enabled(bool enable) {
     g_bk_pointing_device_config.is_sniping_enabled = enable;
     maybe_update_bk_pointing_device_cpi(&g_bk_pointing_device_config);
 }
 
 bool bk_pointing_device_get_pointer_dragscroll_enabled(void) {
     return g_bk_pointing_device_config.is_dragscroll_enabled;
 }
 
 void bk_pointing_device_set_pointer_dragscroll_enabled(bool enable) {
     g_bk_pointing_device_config.is_dragscroll_enabled = enable;
     maybe_update_bk_pointing_device_cpi(&g_bk_pointing_device_config);
 }

  /**
  * \brief Augment the pointing device behavior.
  *
  * Corrects for sensor angle on Dilemma
  */
//   #ifdef DILEMMA_TRACKBALL
  static void bk_pointing_device_task_pointing_device_dilemma(report_mouse_t* mouse_report) {
    // first: move the sensor to the back of the keyboard
    // x stays the same
    // static int16_t prev_x = 0;
    // static int16_t prev_y = 0;
    // if (prev_x != mouse_report->x || prev_y != mouse_report->y) {
    //     printf("mouse_report->x: %d, mouse_report->y: %d\n", mouse_report->x, mouse_report->y);
    // }

    // // float dy_world = 0.029345f * (float)(mouse_report->x) - 0.027695f * (float)(mouse_report->y);
    // // float dx_world = 0.111018f * (float)(mouse_report->x) + 0.027752f * (float)(mouse_report->y);
    // // // if (prev_x != mouse_report->x || prev_y != mouse_report->y) {
    // // // printf("NEW mouse_report->x: %f, mouse_report->y: %f\n", dx_world, dy_world);
    // // // }
    // float dx_world =  0.098246f * (float)(mouse_report->x) - 0.026167f * (float)(mouse_report->y);
    // float dy_world =  0.036666f * (float)(mouse_report->x) + 0.019220f * (float)(mouse_report->y);
    // mouse_report->x = (int16_t)dx_world * 3;
    // mouse_report->y = (int16_t)dy_world * 3;
    // prev_x = mouse_report->x;
    // prev_y = mouse_report->y;
}
// #endif

 /**
  * \brief Augment the pointing device behavior.
  *
  * Implement drag-scroll.
  */
 static void bk_pointing_device_task_pointing_device(report_mouse_t* mouse_report) {
     static int16_t scroll_buffer_x = 0;
     static int16_t scroll_buffer_y = 0;
     if (g_bk_pointing_device_config.is_dragscroll_enabled) {
 #    ifdef BK_POINTING_DEVICE_BK_POINTING_DEVICE_DRAGSCROLL_REVERSE_X
         scroll_buffer_x -= mouse_report->x;
 #    else
         scroll_buffer_x += mouse_report->x;
 #    endif // BK_POINTING_DEVICE_BK_POINTING_DEVICE_DRAGSCROLL_REVERSE_X
 #    ifdef BK_POINTING_DEVICE_BK_POINTING_DEVICE_DRAGSCROLL_REVERSE_Y
         scroll_buffer_y -= mouse_report->y;
 #    else
         scroll_buffer_y += mouse_report->y;
 #    endif // BK_POINTING_DEVICE_BK_POINTING_DEVICE_DRAGSCROLL_REVERSE_Y
         mouse_report->x = 0;
         mouse_report->y = 0;
         if (abs(scroll_buffer_x) > BK_POINTING_DEVICE_BK_POINTING_DEVICE_DRAGSCROLL_BUFFER_SIZE) {
             mouse_report->h = scroll_buffer_x > 0 ? 1 : -1;
             scroll_buffer_x = 0;
         }
         if (abs(scroll_buffer_y) > BK_POINTING_DEVICE_BK_POINTING_DEVICE_DRAGSCROLL_BUFFER_SIZE) {
             mouse_report->v = scroll_buffer_y > 0 ? 1 : -1;
             scroll_buffer_y = 0;
         }
     }
 }
 
 report_mouse_t pointing_device_task_bk_pointing_device(report_mouse_t mouse_report) {
     if (is_keyboard_master()) {
        // printf("mouse_report: x=%d, y=%d\n", mouse_report.x, mouse_report.y);
// #ifdef DILEMMA_TRACKBALL
        bk_pointing_device_task_pointing_device_dilemma(&mouse_report);
// #endif
         bk_pointing_device_task_pointing_device(&mouse_report);
         mouse_report = pointing_device_task_user(mouse_report);
     }
     return mouse_report;
 }
 
 // TODO missing && !NO_DILEMMA_KEYCODES?
//  #    if defined(BK_POINTING_DEVICE_ENABLE) && !defined(NO_BK_POINTING_DEVICE_KEYCODES)
 /** \brief Whether SHIFT mod is enabled. */
 static bool has_shift_mod(void) {
 #        ifdef NO_ACTION_ONESHOT
     return mod_config(get_mods()) & MOD_MASK_SHIFT;
 #        else
     return mod_config(get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
 #        endif // NO_ACTION_ONESHOT
 }
//  #    endif // BK_POINTING_DEVICE_ENABLE && !NO_BK_POINTING_DEVICE_KEYCODES
 
 /**
  * \brief Outputs the Pointing device configuration to console.
  *
  * Prints the in-memory configuration structure to console, for debugging.
  * Includes:
  *   - raw value
  *   - drag-scroll: on/off
  *   - sniping: on/off
  *   - default DPI: internal table index/actual DPI
  *   - sniping DPI: internal table index/actual DPI
  */
 static void debug_bk_pointing_device_config_to_console(bk_pointing_device_config_t* config) {
 #    ifdef CONSOLE_ENABLE
     dprintf("(pointing_device) process_record_kb: config = {\n"
             "\traw = 0x%X,\n"
             "\t{\n"
             "\t\tis_dragscroll_enabled=%u\n"
             "\t\tis_sniping_enabled=%u\n"
             "\t\tdefault_dpi=0x%X (%u)\n"
             "\t\tsniping_dpi=0x%X (%u)\n"
             "\t}\n"
             "}\n",
             config->raw, config->is_dragscroll_enabled, config->is_sniping_enabled, config->pointer_default_dpi, get_pointer_default_dpi(config), config->pointer_sniping_dpi, get_pointer_sniping_dpi(config));
 #    endif // CONSOLE_ENABLE
 }
 
 bool process_record_bk_pointing_device(uint16_t keycode, keyrecord_t* record) {
     if (!process_record_user(keycode, record)) {
         debug_bk_pointing_device_config_to_console(&g_bk_pointing_device_config);
         return false;
     }
//  #    ifdef BK_POINTING_DEVICE_ENABLE
//  #        ifndef NO_BK_POINTING_DEVICE_KEYCODES
     switch (keycode) {
         case DPI_MOD:
             if (record->event.pressed) {
                 // Step backward if shifted, forward otherwise.
                 bk_pointing_device_cycle_pointer_default_dpi(/* forward= */ !has_shift_mod());
             }
             break;
         case DPI_RMOD:
             if (record->event.pressed) {
                 // Step forward if shifted, backward otherwise.
                 bk_pointing_device_cycle_pointer_default_dpi(/* forward= */ has_shift_mod());
             }
             break;
         case S_D_MOD:
             if (record->event.pressed) {
                 // Step backward if shifted, forward otherwise.
                 bk_pointing_device_cycle_pointer_sniping_dpi(/* forward= */ !has_shift_mod());
             }
             break;
         case S_D_RMOD:
             if (record->event.pressed) {
                 // Step forward if shifted, backward otherwise.
                 bk_pointing_device_cycle_pointer_sniping_dpi(/* forward= */ has_shift_mod());
             }
             break;
         case SNIPING:
         printf("SNIPING\n");
             bk_pointing_device_set_pointer_sniping_enabled(record->event.pressed);
             break;
         case SNP_TOG:
             if (record->event.pressed) {
                 bk_pointing_device_set_pointer_sniping_enabled(!bk_pointing_device_get_pointer_sniping_enabled());
             }
             break;
         case DRGSCRL:
             bk_pointing_device_set_pointer_dragscroll_enabled(record->event.pressed);
             break;
         case DRG_TOG:
             if (record->event.pressed) {
                 bk_pointing_device_set_pointer_dragscroll_enabled(!bk_pointing_device_get_pointer_dragscroll_enabled());
             }
             break;
     }
//  #        endif // !NO_BK_POINTING_DEVICE_KEYCODES
//  #    endif     // BK_POINTING_DEVICE_ENABLE
     if (IS_QK_KB(keycode) || IS_MOUSEKEY(keycode)) {
         debug_bk_pointing_device_config_to_console(&g_bk_pointing_device_config);
     }
     return true;
 }
 
 void eeconfig_init_kb(void) {
     g_bk_pointing_device_config.raw = 0;
     write_bk_pointing_device_config_to_eeprom(&g_bk_pointing_device_config);
     maybe_update_bk_pointing_device_cpi(&g_bk_pointing_device_config);
     // TODO: replace with per-module memory management
#ifdef COMMUNITY_MODULE_ARGOS_ENABLE
#else
     eeconfig_init_user();
#endif
 }
 
 void keyboard_post_init_bk_pointing_device(void) {
     read_bk_pointing_device_config_from_eeprom(&g_bk_pointing_device_config);
    //  matrix_init_user(); // TODO we removed this
 } 

 // TODO: for dilemma, missing keyboard_pre_init_kb?  gpio_init?