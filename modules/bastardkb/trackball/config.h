#pragma once


// TODO those are different between the dillemma and charybdis
// maybe isolate the common code into a module, and then create 2 modules charybdis_trackball and dilemma_trackball?
// for now we only include the dilemma pins

/* SPI & PMW3360 settings. */
#define SPI_DRIVER SPID0
#define SPI_SCK_PIN GP22
#define SPI_MISO_PIN GP20
#define SPI_MOSI_PIN GP23
#define POINTING_DEVICE_CS_PIN GP21

// this is also dilemma specific. On the charybdis it's -25
#define ROTATIONAL_TRANSFORM_ANGLE 5
#define POINTING_DEVICE_INVERT_Y
#define POINTING_DEVICE_INVERT_X