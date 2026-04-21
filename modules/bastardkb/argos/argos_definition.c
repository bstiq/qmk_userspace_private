// Copyright 2025 Ira Cooper <ira@wakeful.net>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "argos.h"
#include "argos_definition_data.h"
#include <string.h>

// Get the size of the compressed keyboard definition
uint32_t argos_get_definition_size(void) {
    return ARGOS_DEFINITION_SIZE;
}

// Get a chunk of the compressed keyboard definition
// Returns the number of bytes copied (up to 28)
uint8_t argos_get_definition_chunk(uint16_t offset, uint8_t *buffer) {
    if (offset >= ARGOS_DEFINITION_SIZE) {
        return 0;
    }

    uint16_t remaining = ARGOS_DEFINITION_SIZE - offset;
    uint8_t chunk_size = remaining < ARGOS_DEFINITION_CHUNK_SIZE ? remaining : ARGOS_DEFINITION_CHUNK_SIZE;

    // Copy from PROGMEM
    memcpy_P(buffer, &argos_definition_data[offset], chunk_size);

    return chunk_size;
}
