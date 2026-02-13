#pragma once
#include <stdint.h>
#include "logo_data.h"

// Función para dibujar el logo en un framebuffer
void logo_draw(uint16_t *fb, int width, int height);