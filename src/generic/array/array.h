#pragma once

#include "../../engine/types.h"

typedef struct array
{
    size_t len;
    size_t capacity;
    size_t item_size;
    void *list;
} Array;

Array *create_array(size_t capacity, size_t item_size);

void *append_array(Array *array, void *item);
void *get_item_from_array(Array *array, size_t index);
void *remove_item_from_array(Array *array, size_t index);
void *update_item(Array *array, size_t index, void *item);
void *remove_items_from_array_and_shift(Array *array, size_t index, int count);
void *realocate_array(Array *array);