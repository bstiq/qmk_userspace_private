#include "display.h"
#include "config.h"
#include "quantum.h"

lv_obj_t *ui_screen_base;
lv_obj_t *ui_screen_pointer;

lv_obj_t *ui_label_layer_name_base;
lv_obj_t *ui_label_mod_gui;
lv_obj_t *ui_button_mod_gui;
lv_obj_t *ui_label_mod_shift;
lv_obj_t *ui_button_mod_shift;
lv_obj_t *ui_label_mod_control;
lv_obj_t *ui_button_mod_control;
lv_obj_t *ui_label_mod_alt;
lv_obj_t *ui_button_mod_alt;
lv_obj_t *ui_label_dpi;
lv_obj_t *ui_bar_dpi;
lv_obj_t *ui_label_s_dpi;
lv_obj_t *ui_bar_s_dpi;
lv_obj_t *ui_label_sniping;
lv_obj_t *ui_button_sniping;
lv_obj_t *ui_button_scroll;
lv_obj_t *ui_label_scroll;
lv_obj_t *ui_label_rgb;
lv_obj_t *ui_bar_rgb;
lv_obj_t *ui_label_rgb_effect;

lv_style_t style_btn;
lv_style_t style_bar;
lv_style_t style_bar_background;
lv_style_t style_btn_pressed;
lv_style_t style_flex_container;

enum ui_user_events {
    EVENT_LAYER_CHANGE = 0,
    EVENT_MOD_CHANGE,
    EVENT_LAST_EVENT,
};

// todo define bits
typedef union {
    uint8_t raw;
    struct {
        uint8_t  mods;
        bool     sniping;
        bool     scrolling;
        uint8_t  rgb_enabled;
        uint8_t  rgb_effect_mode;
        uint16_t rgb_val;
        uint16_t dpi;
        uint16_t s_dpi;
        uint8_t  layer;
    } __attribute__((packed));
} dilemma_status_t;

static dilemma_status_t g_dilemma_status_prev = {0};
static dilemma_status_t g_dilemma_status      = {0};

const char *ui_layer_strings[] = {"BASE", "FUNCTION", "NAV", "MED/RGB", "POINTER", "NUM", "SYM"};

void display_init(void) {
    /*
        Base layer screen
    */
    ui_screen_base = lv_obj_create(NULL);
    style_init_mod_indicator();
    style_pressed_init_mod_indicator();
    style_bar_init();
    style_flex_container_init();

    lv_obj_t *cont = lv_obj_create(ui_screen_base);
    lv_obj_set_size(cont, LCD_WIDTH, LCD_HEIGHT);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_add_style(cont, &style_flex_container, 0);

    ui_label_layer_name_base = lv_label_create(cont);
    ui_init_layer_name(ui_label_layer_name_base, "Layer: Base");
    lv_obj_set_size(ui_label_layer_name_base, 200, 30);
    lv_obj_center(ui_label_layer_name_base);

    ui_button_mod_gui = lv_btn_create(cont);
    ui_init_button_mod_indicator(ui_button_mod_gui);
    lv_obj_add_flag(ui_button_mod_gui, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK); // force new line
    ui_label_mod_gui = lv_label_create(ui_button_mod_gui);
    lv_label_set_text(ui_label_mod_gui, "Gui");
    lv_obj_center(ui_label_mod_gui);

    ui_button_mod_alt = lv_btn_create(cont);
    ui_init_button_mod_indicator(ui_button_mod_alt);
    ui_label_mod_alt = lv_label_create(ui_button_mod_alt);
    lv_label_set_text(ui_label_mod_alt, "Alt");
    lv_obj_center(ui_label_mod_alt);

    ui_button_mod_control = lv_btn_create(cont);
    ui_init_button_mod_indicator(ui_button_mod_control);
    ui_label_mod_control = lv_label_create(ui_button_mod_control);
    lv_label_set_text(ui_label_mod_control, "Ctrl");
    lv_obj_center(ui_label_mod_control);

    ui_button_mod_shift = lv_btn_create(cont);
    ui_init_button_mod_indicator(ui_button_mod_shift);
    ui_label_mod_shift = lv_label_create(ui_button_mod_shift);
    lv_label_set_text(ui_label_mod_shift, "Shft");
    lv_obj_center(ui_label_mod_shift);

    // display base layer screen upon init
    lv_disp_load_scr(ui_screen_base);

    /*
        Pointer screen
    */
    // todo delete this line
    ui_screen_pointer = lv_obj_create(NULL);

    // cont      = lv_obj_create(ui_screen_pointer);
    // lv_obj_set_size(cont, 240, 280); // todo change to screen height
    // lv_obj_add_style(cont, &style_flex_container, 0);
    
    ui_button_sniping = lv_btn_create(cont);
    lv_obj_add_flag(ui_button_sniping, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK); // force new line
    ui_init_button_mod_indicator(ui_button_sniping);
    ui_label_sniping = lv_label_create(ui_button_sniping);
    lv_label_set_text(ui_label_sniping, "Snipe");
    lv_obj_center(ui_label_sniping);

    ui_button_scroll = lv_btn_create(cont);
    ui_init_button_mod_indicator(ui_button_scroll);
    ui_label_scroll = lv_label_create(ui_button_scroll);
    lv_label_set_text(ui_label_scroll, "Scroll");
    lv_obj_center(ui_label_scroll);

    ui_label_dpi = lv_label_create(cont);
    lv_label_set_text(ui_label_dpi, "DPI");
    lv_obj_add_flag(ui_label_dpi, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK); // force new line
    ui_bar_dpi = lv_bar_create(cont);
    lv_obj_set_height(ui_bar_dpi, 25);
    lv_obj_set_flex_grow(ui_bar_dpi, 1); // take all remaining space in line
    lv_obj_add_style(ui_bar_dpi, &style_bar, LV_PART_INDICATOR);
    lv_obj_add_style(ui_bar_dpi, &style_bar_background, 0);

    ui_label_s_dpi = lv_label_create(cont);
    lv_label_set_text(ui_label_s_dpi, "Snip. DPI");
    lv_obj_add_flag(ui_label_s_dpi, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK); // force new line
    ui_bar_s_dpi = lv_bar_create(cont);
    lv_obj_set_height(ui_bar_s_dpi, 25);
    lv_obj_set_flex_grow(ui_bar_s_dpi, 1); // take all remaining space in line
    lv_obj_add_style(ui_bar_s_dpi, &style_bar, LV_PART_INDICATOR);
    lv_obj_add_style(ui_bar_s_dpi, &style_bar_background, 0);

    /*
    Rgb info
    */
    ui_label_rgb = lv_label_create(cont);
    lv_label_set_text(ui_label_rgb, "RGB");
    lv_obj_add_flag(ui_label_rgb, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK); // force new line
    ui_bar_rgb = lv_bar_create(cont);
    lv_obj_set_height(ui_bar_rgb, 25);
    lv_obj_set_flex_grow(ui_bar_rgb, 1); // take all remaining space in line
    lv_obj_add_style(ui_bar_rgb, &style_bar, LV_PART_INDICATOR);
    lv_obj_add_style(ui_bar_rgb, &style_bar_background, 0);

    ui_label_rgb_effect = lv_label_create(cont);
    lv_label_set_text(ui_label_rgb_effect, "effect...");
    lv_obj_add_flag(ui_label_rgb_effect, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK); // force new line

    /*
        Theme
    */
    lv_disp_t  *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(BK_PALETTE), lv_palette_main(BK_PALETTE), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
}

void style_init_mod_indicator(void) {
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 3);
    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);

    lv_style_set_bg_color(&style_btn, lv_palette_darken(BK_PALETTE, 1));
    lv_style_set_bg_grad_color(&style_btn, lv_palette_darken(BK_PALETTE, 3));
    lv_style_set_bg_grad_dir(&style_btn, LV_GRAD_DIR_VER);

    lv_style_set_border_color(&style_btn, lv_palette_lighten(BK_PALETTE, 1));
    lv_style_set_border_opa(&style_btn, LV_OPA_20);
    lv_style_set_border_width(&style_btn, 2);

    lv_style_set_text_color(&style_btn, lv_color_white());
}

void style_pressed_init_mod_indicator(void) {
    lv_style_init(&style_btn_pressed);
    lv_style_set_radius(&style_btn_pressed, 2);
    lv_style_set_bg_opa(&style_btn_pressed, LV_OPA_COVER);

    lv_style_set_bg_color(&style_btn_pressed, lv_palette_lighten(BK_PALETTE, 3));
    lv_style_set_bg_grad_color(&style_btn_pressed, lv_palette_lighten(BK_PALETTE, 1));
    lv_style_set_bg_grad_dir(&style_btn_pressed, LV_GRAD_DIR_VER);

    lv_style_set_border_color(&style_btn_pressed, lv_palette_lighten(BK_PALETTE, 1));
    lv_style_set_border_opa(&style_btn_pressed, LV_OPA_20);
    lv_style_set_border_width(&style_btn_pressed, 2);

    lv_style_set_text_color(&style_btn_pressed, lv_color_black());
}

void style_bar_init(void) {
    // inner bar
    lv_style_set_radius(&style_bar, 0);
    lv_style_set_bg_color(&style_bar, lv_palette_darken(BK_PALETTE, 3));

    // bar background
    lv_style_set_radius(&style_bar_background, 3);
    lv_style_set_border_color(&style_bar_background, lv_palette_darken(BK_PALETTE, 4));
    lv_style_set_border_width(&style_bar_background, 1);
}

void ui_init_layer_name(lv_obj_t *label, const char *layer_name) {
    lv_obj_remove_style_all(label);
    lv_label_set_text(label, layer_name);
    lv_obj_set_width(label, LV_SIZE_CONTENT);
    lv_obj_set_x(label, 0);
    lv_obj_set_y(label, 20);
    lv_obj_set_align(label, LV_ALIGN_TOP_MID);
}

void style_flex_container_init(void) {
    lv_style_set_bg_color(&style_flex_container, lv_color_black());
    lv_style_set_pad_row(&style_flex_container, 10);
    lv_style_set_border_width(&style_flex_container, 0);
}

void ui_init_button_mod_indicator(lv_obj_t *button) {
    // styles
    lv_obj_add_style(button, &style_btn, 0);
    lv_obj_add_style(button, &style_btn_pressed, LV_STATE_PRESSED);

    // behaviour
    lv_obj_add_event_cb(button, event_screen_base_update_mods, LV_EVENT_ALL, NULL);

    // position and width
    // lv_obj_set_width(button, 45);
    lv_obj_set_height(button, 30);

    // automatically fill space
     lv_obj_set_flex_grow(button, 1);
}

void event_screen_pointer_sniping_toggle(lv_event_t *e) {}
void event_screen_pointer_scroll_toggle(lv_event_t *e) {}

// TODO what is this?
void event_screen_base_update_mods(lv_event_t *e) {}

void housekeeping_task_display(void) {
    update_dilemma_status();

    housekeeping_task_screen_layer_name();
    housekeeping_task_screen_base();
    housekeeping_task_screen_rgb();
    housekeeping_task_screen_pointer();
    housekeeping_task_screen_rgb();

    g_dilemma_status_prev = g_dilemma_status;
}

void housekeeping_task_screen_layer_name(void) {
    if (g_dilemma_status.layer != g_dilemma_status_prev.layer) {
        switch (g_dilemma_status.layer) {
            case 0:
            default:
                lv_label_set_text(ui_label_layer_name_base, "Layer: Base");
                break;
            case 1:
                lv_label_set_text(ui_label_layer_name_base, "Layer: Function");
                break;
            case 2:
                lv_label_set_text(ui_label_layer_name_base, "Layer: Navigation");
                break;
            case 3:
                lv_label_set_text(ui_label_layer_name_base, "Layer: Media/RGB");
                break;
            case 4:
                lv_label_set_text(ui_label_layer_name_base, "Layer: Pointer");
                break;
            case 5:
                lv_label_set_text(ui_label_layer_name_base, "Layer: Numeral");
                break;
            case 6:
                lv_label_set_text(ui_label_layer_name_base, "Layer: Symbols");
                break;
        }
    }
}

void update_dilemma_status(void) {
    g_dilemma_status.mods  = get_mods();
    g_dilemma_status.layer = get_highest_layer(layer_state);
    g_dilemma_status.sniping = dilemma_get_pointer_sniping_enabled();
    g_dilemma_status.dpi = dilemma_get_pointer_default_dpi();
    g_dilemma_status.s_dpi = dilemma_get_pointer_sniping_dpi();
    g_dilemma_status.scrolling = dilemma_get_pointer_dragscroll_enabled();
    g_dilemma_status.rgb_enabled     = rgb_matrix_is_enabled();
    g_dilemma_status.rgb_effect_mode = rgb_matrix_get_mode();
    g_dilemma_status.rgb_val         = rgb_matrix_get_val();
}

void housekeeping_task_screen_base(void) {
    update_mod_button(g_dilemma_status.mods, MOD_MASK_SHIFT, ui_button_mod_shift);
    update_mod_button(g_dilemma_status.mods, MOD_MASK_ALT, ui_button_mod_alt);
    update_mod_button(g_dilemma_status.mods, MOD_MASK_CTRL, ui_button_mod_control);
    update_mod_button(g_dilemma_status.mods, MOD_MASK_GUI, ui_button_mod_gui);
}

void update_mod_button(uint8_t mods_active, uint8_t MASK, lv_obj_t *ui_button_mod) {
    if ((mods_active & MASK) != (g_dilemma_status_prev.mods & MASK)) {
        if ((mods_active & MASK)) {
            lv_event_send(ui_button_mod, LV_EVENT_PRESSED, NULL);
        } else {
            lv_event_send(ui_button_mod, LV_EVENT_RELEASED, NULL);
        }
    }
}

void housekeeping_task_screen_rgb(void) {
    const bool     rgb_change      = (g_dilemma_status.rgb_enabled != g_dilemma_status_prev.rgb_enabled);

    if (!g_dilemma_status.rgb_enabled) {
        if (rgb_change) {
            lv_label_set_text(ui_label_rgb, "RGB: Off");
            lv_bar_set_value(ui_bar_rgb, 0, LV_ANIM_OFF);
            lv_label_set_text(ui_label_rgb_effect, "");
        }
    } else {
        if ((rgb_change) || (g_dilemma_status.rgb_val != g_dilemma_status_prev.rgb_val)) {
            char rgbval[50];
            sprintf(rgbval, "RGB: %u", g_dilemma_status.rgb_val);
            lv_label_set_text(ui_label_rgb, rgbval);
            float rel = (float)(g_dilemma_status.rgb_val) * 100 / 156;
            lv_bar_set_value(ui_bar_rgb, (uint16_t)rel, LV_ANIM_OFF);
        }
        if ((rgb_change) || (g_dilemma_status.rgb_effect_mode != g_dilemma_status_prev.rgb_effect_mode)) {
            const char *effect_name = rgb_matrix_get_effect_name();
            lv_label_set_text(ui_label_rgb_effect, effect_name);
        }
    }
}

void housekeeping_task_screen_pointer(void) {
    // TODO dynamically get max DPI, instead of using hardcoded values
    if (g_dilemma_status.dpi != g_dilemma_status_prev.dpi) {
        static const uint16_t rel_max_dpi = 200 * 16;
        const float           rel         = (float)((g_dilemma_status.dpi + 200 - 400)) * 100 / rel_max_dpi;
        lv_bar_set_value(ui_bar_dpi, (uint16_t)rel, LV_ANIM_OFF);

        char c_dpi[50];
        sprintf(c_dpi, "DPI: %u", (uint16_t)g_dilemma_status.dpi);
        lv_label_set_text(ui_label_dpi, c_dpi);
    }

    if (g_dilemma_status.s_dpi != g_dilemma_status_prev.s_dpi) {
        char                  c_s_dpi[50];
        static const uint16_t rel_max_s_dpi = 100 * 4;
        const float           rel           = (float)((g_dilemma_status.s_dpi + 100 - 200)) * 100 / rel_max_s_dpi;
        lv_bar_set_value(ui_bar_s_dpi, (uint16_t)rel, LV_ANIM_OFF);
        sprintf(c_s_dpi, "Sniper DPI: %u", (uint16_t)g_dilemma_status.s_dpi);
        lv_label_set_text(ui_label_s_dpi, c_s_dpi);
    }

    if (g_dilemma_status.sniping != g_dilemma_status_prev.sniping) {
        if (g_dilemma_status.sniping) {
            lv_event_send(ui_button_sniping, LV_EVENT_PRESSED, NULL);
        } else {
            lv_event_send(ui_button_sniping, LV_EVENT_RELEASED, NULL);
        }
    }

    if (g_dilemma_status.scrolling != g_dilemma_status_prev.scrolling) {
        if (g_dilemma_status.scrolling) {
            lv_event_send(ui_button_scroll, LV_EVENT_PRESSED, NULL);
        } else {
            lv_event_send(ui_button_scroll, LV_EVENT_RELEASED, NULL);
        }
    }
}

bool process_records_display(uint16_t keycode, keyrecord_t *record) {
    return true;
}

const char *rgb_matrix_get_effect_name(void) {
    // thank you drashna!
    static char    buf[32]     = {0};
    static uint8_t last_effect = 0;
    if (last_effect != rgb_matrix_get_mode()) {
        last_effect = rgb_matrix_get_mode();
        snprintf(buf, sizeof(buf), "%s", rgb_matrix_get_mode_name(rgb_matrix_get_mode()));
        for (uint8_t i = 1; i < sizeof(buf); ++i) {
            if (buf[i] == 0)
                break;
            else if (buf[i] == '_')
                buf[i] = ' ';
            else if (buf[i - 1] == ' ')
                buf[i] = toupper(buf[i]);
            else if (buf[i - 1] != ' ')
                buf[i] = tolower(buf[i]);
        }
    }
    return buf;
}