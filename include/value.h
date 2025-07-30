#pragma once

typedef enum
{
    VALUE_BOOL,
    VALUE_NIL,
    VALUE_NUMBER,
} value_type_t;

typedef struct
{
    value_type_t type;
    union {
        bool boolean;
        double number;
    } as;
} value_t;

#define IS_BOOL(value) ((value).type == VALUE_BOOL)
#define IS_NIL(value) ((value).type == VALUE_NIL)
#define IS_NUMBER(value) ((value).type == VALUE_NUMBER)

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)

#define BOOL_VAL(value) ((value_t){VALUE_BOOL, {.boolean = value}})
#define NIL_VAL ((value_t){VALUE_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((value_t){VALUE_NUMBER, {.number = value}})

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
