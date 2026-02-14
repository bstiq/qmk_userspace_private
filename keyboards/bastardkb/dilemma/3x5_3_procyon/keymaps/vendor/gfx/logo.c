#include "logo.h"

void logo_show(painter_device_t surface) {
    qp_drawpixels(
        surface,
        0, 0,
        LOGO_WIDTH,
        LOGO_HEIGHT,
        logo_bitmap
    );
}
