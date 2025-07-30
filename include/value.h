#pragma once

typedef double value_t;

typedef struct
{
    size_t capacity;
    int count;
    value_t *values;

} value_array_t;

void init_value_array(value_array_t *varray);
void write_value_array(value_array_t *varray, value_t value);
void free_value_array(value_array_t *varray);
void print_value(value_t value);
