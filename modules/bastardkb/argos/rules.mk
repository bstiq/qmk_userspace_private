SRC += argos_combo.c

# Enable required QMK features if not already enabled
COMBO_ENABLE ?= yes
EXTRA_EXTRA_LONG_COMBOS ?= no
EXTRA_LONG_COMBOS ?= no
EXTRA_SHORT_COMBOS ?= no

# TODO tapping term per key