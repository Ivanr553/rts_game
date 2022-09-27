#include <stdlib.h>

#include "map.h"
#include "../../engine/util/util.h"
#include <glad/glad.h>
#include <string.h>
#include <stdlib.h>

int collisions = 0;

Map *create_map(int size, int use_long_key)
{
    Array *keys = create_array(size, sizeof(Map_Value));
    Map *map = (Map *)calloc(1, sizeof(Map));
    map->size = size;
    map->map_values = keys;
    map->use_long_key = use_long_key;

    return map;
};

size_t get_hash(Map *map, void *key)
{
    if (map->use_long_key == 1)
    {
        long _key = (long)key;
        return _key % map->size;
    }

    char *_key = key;
    size_t keyNum = 0;

    for (int i = 0; i < strlen(_key); i++)
    {
        keyNum += _key[i];
        keyNum *= _key[i];
    }

    return keyNum % map->size;
};

void *get_value_from_map(Map *map, char *key)
{
    size_t index = get_hash(map, key);

    Map_Value *map_value = get_item_from_array(map->map_values, index);
    if (!map_value->value)
    {
        // printf("Null pointer in map\n");
        return NULL;
    }

    void *value = traverse_map_linked_list(map_value, key, map->use_long_key);

    return value;
};

void *set_value_in_map(Map *map, void *key, void *value)
{
    size_t index = get_hash(map, key);

    Map_Value *map_value = get_item_from_array(map->map_values, index);

    if (map_value->value)
    {
        collisions++;
        while (map_value->next)
        {
            map_value = map_value->next;
        }

        Map_Value *new_map_value = calloc(1, sizeof(Map_Value));
        new_map_value->key = key;
        new_map_value->value = value;
        new_map_value->next = NULL;

        map_value->next = new_map_value;
    }
    else
    {
        Map_Value *new_map_value = calloc(1, sizeof(Map_Value));
        new_map_value->key = key;
        new_map_value->value = value;
        new_map_value->next = NULL;

        update_item(map->map_values, index, new_map_value);
    }

    return value;
};

void remove_value_in_map(Map *map, char *key)
{
    size_t index = get_hash(map, key);

    Map_Value *map_value = get_item_from_array(map->map_values, index);

    if (map_value)
    {
        Map_Value *prev_map_value = NULL;

        if (map->use_long_key)
        {
            long _key = (long)key;
            long map_value_key = (long)map_value->key;

            while (map_value_key != _key)
            {
                prev_map_value = map_value;
                map_value = map_value->next;
            }
        }
        else
        {
            while (strcmp(map_value->key, key) != 0)
            {
                prev_map_value = map_value;
                map_value = map_value->next;
            }
        }

        if (map_value->next)
        {
            map_value = map_value->next;
        }
        else if (!prev_map_value)
        {
            remove_item_from_array(map->map_values, index);
        }
        else
        {
            prev_map_value->next = map_value->next;
        }
    }
    else
    {
        remove_item_from_array(map->map_values, index);
    }
};

Map_Value *traverse_map_linked_list(Map_Value *map_value, char *key, int use_long_key)
{
    if (use_long_key)
    {
        Map_Value *next_map_value = map_value;
        long _key = key;
        long value_key = next_map_value->key;

        while (_key != value_key)
        {
            if (!next_map_value)
            {
                return NULL;
            }

            next_map_value = next_map_value->next;
        }

        return next_map_value->value;
    }

    char *value_key = key;
    Map_Value *next_map_value = map_value;

    while (strcmp(value_key, next_map_value->key) != 0)
    {
        if (!next_map_value->next)
        {
            return NULL;
        }

        next_map_value = next_map_value->next;
    }

    return next_map_value->value;
};