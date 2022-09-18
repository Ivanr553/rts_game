#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "array.h"
#include "../../engine/util/util.h"

Array *create_array(size_t capacity, size_t item_size)
{
    Array *array = (Array *)calloc(1, sizeof(Array));
    array->item_size = item_size;
    array->capacity = capacity;
    array->len = 0;
    array->list = calloc(capacity, item_size);

    for (size_t i = 0; i < capacity; i++)
    {
        memset((u8 *)array->list + i * array->item_size, 0, item_size);
    }

    return array;
};

void *append_array(Array *array, void *item)
{
    size_t index = array->len++;

    if (array->len == array->capacity)
    {
        array->list = realocate_array(array);
    }

    // return memset((u8 *)array->list + index * array->item_size, item, array->item_size);
    memcpy((u8 *)array->list + index * array->item_size, item, array->item_size);

    return get_item_from_array(array, index);
};

void *get_item_from_array(Array *array, size_t index)
{
    if (index >= array->capacity)
    {
        ERROR_RETURN(NULL, "Index out of bounds\n");
    }

    void *value = (u8 *)array->list + index * array->item_size;

    return value;
};

void *remove_item_from_array(Array *array, size_t index)
{
    return memset((u8 *)array->list + index * array->item_size, 0, array->item_size);
};

void *remove_items_from_array_and_shift(Array *array, size_t index, int count)
{
    for (int r = index; r < index + count; r++)
    {
        remove_item_from_array(array, r);
    }

    int i = index;
    while ((i + count) < array->len)
    {
        void *item_to_move = get_item_from_array(array, i + count);
        memcpy((u8 *)array->list + i * array->item_size, item_to_move, array->item_size);
        i++;
    }

    array->len -= count;
    return array;
}

void *update_item(Array *array, size_t index, void *item)
{
    return memcpy((u8 *)array->list + index * array->item_size, item, array->item_size);
};

void *realocate_array(Array *array)
{
    array->capacity *= 2;
    printf("Reallocating array to size: %d with capacity: %d\n", array->capacity * array->item_size, array->capacity);
    Array *new_array = realloc(array->list, array->capacity * array->item_size);

    if (!new_array)
    {
        printf("Unable to allocate space for array\n");
        return NULL;
    }

    return new_array;
}