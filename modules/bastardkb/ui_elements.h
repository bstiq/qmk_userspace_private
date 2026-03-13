
void              ui_init_layer_name(lv_obj_t *label);
void              ui_init_button_mod_indicator(lv_obj_t *label);
mod_button_pair_t ui_create_mod_button(lv_obj_t *cont, const char *text, bool force_new_track, uint8_t mod_mask);
lv_obj_t *ui_create_secondary_text(lv_obj_t *cont, const char *text, bool new_track, uint8_t flex);
lv_obj_t *ui_create_progress_bar(lv_obj_t *cont, uint8_t flex);
lv_obj_t *ui_create_number_label(lv_obj_t *cont, uint8_t flex);
lv_obj_t *ui_create_line_separator(lv_obj_t *cont, uint8_t flex, uint8_t height);