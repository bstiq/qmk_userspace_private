#include "logo_data.h"

// Bitmap: rectángulo blanco sobre fondo negro
const uint16_t logo_bitmap[LOGO_WIDTH * LOGO_HEIGHT] = {
    [0 ... (LOGO_WIDTH*LOGO_HEIGHT-1)] = 0xFFFF // Color blanco 16bpp
};
