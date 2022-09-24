#pragma once

#include "../../engine/types.h"
#include "../array/array.h"

int collisions;

typedef union map_key
{
    long key_long;
    char *key_char;
} Map_Key;


typedef struct map
{
    Array *map_values;
    int size;
    int use_long_key;
} Map;

typedef struct Map_Value Map_Value;

struct Map_Value
{
    void *value;
    char *key;
    Map_Value *next;
};

Map *create_map(int size, int use_long_key);
size_t get_hash(Map *map, char *key);
void *get_value_from_map(Map *map, void *key);
void *set_value_in_map(Map *map, void *key, void *value);
Map_Value *traverse_map_linked_list(Map_Value *map_value, char *key, int use_long_key);