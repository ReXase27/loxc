#pragma once

#include "chunk.h"

#define DEBUG_TRACE_EXECUTION
#define DEBUG_PRINT_CODE

void disassemble_chunk(chunk_t *chunk, const char *name);
int disassemble_instruction(chunk_t *chunk, int offset);
