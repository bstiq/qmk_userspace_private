QUANTUM_PAINTER_ENABLE = yes
# QUANTUM_PAINTER_DRIVERS += st7789_spi // TODO enable after moving it out of the keyboard rules.mk
# QUANTUM_PAINTER_DRIVERS += surface // TODO enable
QUANTUM_PAINTER_LVGL_INTEGRATION = yes
ST7789 = true

SRC += theme.c

# LVGL fonts
SRC += montserratbold14.c
SRC += montserratbold13.c
SRC += dmsans13.c
SRC += dmsans14.c
SRC += dmsans14bold.c
SRC += jetbrainsmono13.c
SRC += jetbrainsmono14bold.c

BACKLIGHT_ENABLE = yes
BACKLIGHT_DRIVER = pwm