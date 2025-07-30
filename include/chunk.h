#pragma once

#include "value.h"

#include <stdint.h>

typedef enum : uint8_t
{
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN
} opcode_t;

typedef struct
{
    int count;
    size_t capacity;
    uint8_t *code;
    int *lines;
    value_array_t constants;
} chunk_t;

void init_chunk(chunk_t *chunk);
void free_chunk(chunk_t *chunk);
void write_chunk(chunk_t *chunk, uint8_t byte, int line);
int add_constant(chunk_t *chunk, value_t value);
