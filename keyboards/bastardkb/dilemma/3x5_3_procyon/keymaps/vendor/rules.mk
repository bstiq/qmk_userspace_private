VIA_ENABLE = yes
ENCODER_MAP_ENABLE = yes
MAXTOUCH_DEBUG = no

QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += st7789_spi
QUANTUM_PAINTER_DRIVERS += surface
ST7789 = true
SRC += gfx/POC.qgf.c
SRC += gfx/bar_blue.qgf.c
SRC += gfx/bar_green.qgf.c
SRC += gfx/bar_gray.qgf.c
SRC += gfx/fonts.qff.c

SRC += jero.c


BACKLIGHT_ENABLE = yes
BACKLIGHT_DRIVER = pwm