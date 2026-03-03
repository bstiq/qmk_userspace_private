VIA_ENABLE = yes
ENCODER_MAP_ENABLE = yes

QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += st7789_spi
QUANTUM_PAINTER_DRIVERS += surface
QUANTUM_PAINTER_LVGL_INTEGRATION = yes
ST7789 = true

SRC += display.c

# LVGL fonts
SRC += montserratbold14.c
SRC += montserratbold13.c
SRC += spacemono_bold_12.c
SRC += spacemono_bold_13.c
SRC += spacemono_bold_15.c

BACKLIGHT_ENABLE = yes
BACKLIGHT_DRIVER = pwm