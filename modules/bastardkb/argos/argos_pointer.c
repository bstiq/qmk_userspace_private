#include QMK_KEYBOARD_H

#include "argos_pointer.h"
#include <stdint.h>
#include "argos.h"
#include "pointer_config_hardcoded.h"

#ifdef POINTING_DEVICE_DRIVER_pmw3360
#include "charybdis.h"
#endif
#ifdef POINTING_DEVICE_DRIVER_digitizer // TODO dilemma v2 / cirque
#include "dilemma.h"
#endif

void build_pointing_device_info_command_data(uint8_t **command_data) {
    (*command_data)[0] = pointing_device_type_unknown;
// TODO dilemma v2, TODO check charybdis working
#ifdef POINTING_DEVICE_DRIVER_pmw3360 // TODO test
    (*command_data)[0] = pointing_device_type_trackball;
    // pointing dpi is up to 400+16*200 = 3600, 2 bytes
    (*command_data)[1] = charybdis_get_pointer_default_dpi() & 0xFF;
    (*command_data)[2] = (charybdis_get_pointer_default_dpi() >> 8) & 0xFF;
    // minimum default DPI is 400, 2 bytes
    (*command_data)[3] = ARGOS_CHARYBDIS_MINIMUM_DEFAULT_DPI & 0xFF;
    (*command_data)[4] = (ARGOS_CHARYBDIS_MINIMUM_DEFAULT_DPI >> 8) & 0xFF;
    // default DPI config step is 200, so one byte, but we use 2 just in
    // case
    (*command_data)[5] = ARGOS_CHARYBDIS_DEFAULT_DPI_CONFIG_STEP & 0xFF;
    (*command_data)[6] = (ARGOS_CHARYBDIS_DEFAULT_DPI_CONFIG_STEP >> 8) & 0xFF;
    // sniping DPI is up to 200+4*100 = 600, 2 bytes
    (*command_data)[7] = charybdis_get_pointer_sniping_dpi() & 0xFF;
    (*command_data)[8] = (charybdis_get_pointer_sniping_dpi() >> 8) & 0xFF;
    // mininmum sniping dpi is 200, but ue use 2 bytes ju) in case
    (*command_data)[9] = ARGOS_CHARYBDIS_MINIMUM_SNIPING_DPI & 0xFF;
    (*command_data)[10] = (ARGOS_CHARYBDIS_MINIMUM_SNIPING_DPI >> 8) & 0xFF;
    // sniping DPI config step is 100, so one byte, but we use 2 just in
    // case
    (*command_data)[11] = ARGOS_CHARYBDIS_SNIPING_DPI_CONFIG_STEP & 0xFF;
    (*command_data)[12] = (ARGOS_CHARYBDIS_SNIPING_DPI_CONFIG_STEP >> 8) & 0xFF;
    // pointing DPI max steps is 16, so one byte is plenty
    // this is hardcoded here as we can't read it from charybdis.c (private
    // config structure)
    (*command_data)[13] = 16;
    // sniping DPI max steps is 4, so one byte is plenty
    // this is hardcoded here as we can't read it from charybdis.c
    (*command_data)[14] = 4;

#endif
#ifdef POINTING_DEVICE_DRIVER_digitizer // Dilemma v3, todo test procyon?
    // TODO move this to other file?...
    (*command_data)[0] = pointing_device_type_trackpad;
    // pointing dpi is up to 400+16*200 = 3600, 2 bytes
    (*command_data)[1] = dilemma_get_pointer_default_dpi() & 0xFF;
    (*command_data)[2] = (dilemma_get_pointer_default_dpi() >> 8) & 0xFF;
    // minimum default DPI is 400, 2 bytes
    (*command_data)[3] = ARGOS_DILEMMA_MINIMUM_DEFAULT_DPI & 0xFF;
    (*command_data)[4] = (ARGOS_DILEMMA_MINIMUM_DEFAULT_DPI >> 8) & 0xFF;
    // default DPI config step is 200, so one byte, but we use 2 just in
    // case
    (*command_data)[5] = ARGOS_DILEMMA_DEFAULT_DPI_CONFIG_STEP & 0xFF;
    (*command_data)[6] = (ARGOS_DILEMMA_DEFAULT_DPI_CONFIG_STEP >> 8) & 0xFF;
    // sniping DPI is up to 200+4*100 = 600, 2 bytes
    (*command_data)[7] = dilemma_get_pointer_sniping_dpi() & 0xFF;
    (*command_data)[8] = (dilemma_get_pointer_sniping_dpi() >> 8) & 0xFF;
    // mininmum sniping dpi is 200, but ue use 2 bytes ju) in case
    (*command_data)[9] = ARGOS_DILEMMA_MINIMUM_SNIPING_DPI & 0xFF;
    (*command_data)[10] = (ARGOS_DILEMMA_MINIMUM_SNIPING_DPI >> 8) & 0xFF;
    // sniping DPI config step is 100, so one byte, but we use 2 just in
    // case
    (*command_data)[11] = ARGOS_DILEMMA_SNIPING_DPI_CONFIG_STEP & 0xFF;
    (*command_data)[12] = (ARGOS_DILEMMA_SNIPING_DPI_CONFIG_STEP >> 8) & 0xFF;
    // pointing DPI max steps is 16, so one byte is plenty
    // this is hardcoded here as we can't read it from dilemma.c (private
    // config structure dilemma_config_t)
    (*command_data)[13] = 16;
    // sniping DPI max steps is 4, so one byte is plenty
    // this is hardcoded here as we can't read it from dilemma.c (private
    // config structure dilemma_config_t)
    (*command_data)[14] = 4;
#endif
}