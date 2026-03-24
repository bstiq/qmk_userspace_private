#ifndef UTILITIES_H
#define UTILITIES_H

#include "lvgl.h"

typedef struct{
    lv_obj_t *obj;
    void (*update_function)(lv_obj_t*);
} obj_update_t;

typedef struct{
    obj_update_t *array;
    size_t used;
    size_t size;
    int amount_elements;
}obj_event_array_t;

#endif