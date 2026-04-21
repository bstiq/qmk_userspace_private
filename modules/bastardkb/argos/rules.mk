# Argos - VIA3-based dynamic features module
# Provides: tap dance, combo, key override, keyboard definition

# Set USB serial number for GUI detection (unless keyboard overrides it)
SERIAL_NUMBER ?= argos:12345-00
OPT_DEFS += -DSERIAL_NUMBER=\"$(SERIAL_NUMBER)\"

# Enable required QMK features if not already enabled
TAP_DANCE_ENABLE ?= yes
COMBO_ENABLE ?= yes
KEY_OVERRIDE_ENABLE ?= yes

# Enable tapping term per key for tap dance custom timing
TAPPING_TERM_PER_KEY ?= yes

# Add argos source files
SRC += argos_tap_dance.c \
       argos_combo.c \
       argos_key_override.c \
       argos_alt_repeat_key.c \
       argos_definition.c \
       argos_qmk_settings.c

# Generate compressed keyboard definition header
# Search for argos.json in keymap directories (same search order as QMK)
ARGOS_JSON_PATH := $(firstword \
    $(wildcard $(MAIN_KEYMAP_PATH_1)/argos.json) \
    $(wildcard $(MAIN_KEYMAP_PATH_2)/argos.json) \
    $(wildcard $(MAIN_KEYMAP_PATH_3)/argos.json) \
    $(wildcard $(MAIN_KEYMAP_PATH_4)/argos.json) \
    $(wildcard $(MAIN_KEYMAP_PATH_5)/argos.json))

ARGOS_DEFINITION_HEADER := $(INTERMEDIATE_OUTPUT)/src/argos_definition_data.h

# Generate the header at the start of the build
$(shell mkdir -p "$(INTERMEDIATE_OUTPUT)/src" && \
    python3 "$(MODULE_PATH_ARGOS)/argos_compress.py" \
        "$(ARGOS_JSON_PATH)" \
        "$(ARGOS_DEFINITION_HEADER)" >/dev/null 2>&1)
