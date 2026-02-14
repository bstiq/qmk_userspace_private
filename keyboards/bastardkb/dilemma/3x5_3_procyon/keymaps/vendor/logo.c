#include "logo.h"
#include "logo_data.h"

// Mostrar el logo en la pantalla usando QMK Painter
void logo_show(painter_device_t surface) {
    qp_drawpixels(
        surface,
        0, 0,                // Coordenadas de inicio (x, y)
        LOGO_WIDTH, LOGO_HEIGHT, // Tamaño del bitmap
        logo_bitmap          // Píxeles
    );
}
