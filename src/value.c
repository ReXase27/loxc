#include "value.h"

#include "memory.h"

#include <stdio.h>

void init_value_array(value_array_t *varray)
{
    varray->values = NULL;
    varray->capacity = 0;
    varray->count = 0;
}

void write_value_array(value_array_t *varray, value_t value)
{
    if (varray->capacity < varray->count + 1)
    {
        size_t old_capacity = varray->capacity;
        varray->capacity = GROW_CAPACITY(old_capacity);
        varray->values = GROW_ARRAY(value_t, varray->values, old_capacity, varray->capacity);
    }

    varray->values[varray->count] = value;
    varray->count++;
}

void free_value_array(value_array_t *varray)
{
    FREE_ARRAY(value_t, varray->values, varray->capacity);

    init_value_array(varray);
}

void print_value(value_t value)
{
    printf("%g", value);
}
