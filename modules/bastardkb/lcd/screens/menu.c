#include "menu.h"

bool process_record_menu(uint16_t keycode, keyrecord_t *record, obj_event_array_t menus, uint8_t *menu_index) {
    switch (keycode) {
        case LCD_MENU_NEXT:
            if (record->event.pressed) {
                lv_event_send(menus.array[*menu_index].obj, LV_EVENT_RELEASED, NULL);
                *menu_index = (*menu_index + 1) % menus.amount_elements;
                lv_event_send(menus.array[*menu_index].obj, LV_EVENT_PRESSED, NULL);
            }
            break;
        case LCD_MENU_PREV:
            if (record->event.pressed) {
                lv_event_send(menus.array[*menu_index].obj, LV_EVENT_RELEASED, NULL);
                *menu_index = (*menu_index - 1 + menus.amount_elements) % menus.amount_elements;
                lv_event_send(menus.array[*menu_index].obj, LV_EVENT_PRESSED, NULL);
            }
            break;
    }
    return true;
}