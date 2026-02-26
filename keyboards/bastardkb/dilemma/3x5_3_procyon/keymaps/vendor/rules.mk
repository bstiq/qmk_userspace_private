VIA_ENABLE = yes
ENCODER_MAP_ENABLE = yes
MAXTOUCH_DEBUG = no

QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += st7789_spi
QUANTUM_PAINTER_DRIVERS += surface
QUANTUM_PAINTER_LVGL_INTEGRATION = yes
ST7789 = true

SRC += gfx/fonts.qff.c
SRC += gfx/regular20.qff.c
SRC += gfx/regular20grey.qff.c
SRC += gfx/regular36.qff.c
SRC += gfx/semibold36.qff.c
SRC += gfx/jostmedium20.qff.c
SRC += gfx/jostlight20.qff.c
SRC += gfx/jostlight22.qff.c
SRC += gfx/jostlight20grey.qff.c
SRC += gfx/jostlight24.qff.c
SRC += gfx/jostbold36.qff.c
SRC += gfx/mods.qgf.c
SRC += display.c

# LVGL fonts
SRC += montserrat.c
SRC += montserrat12.c

BACKLIGHT_ENABLE = yes
BACKLIGHT_DRIVER = pwm