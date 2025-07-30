#pragma once

#include "chunk.h"
#include "scanner.h"
#include "value.h"

#include <stdint.h>

typedef struct
{
    token_t current;
    token_t previous;
    bool had_error;
    bool panic_mode;
} parser_t;

typedef enum
{
    PREC_NONE,
    PREC_ASSIGNMENT,
    PREC_OR,
    PREC_AND,
    PREC_EQUALITY,
    PREC_COMPARISON,
    PREC_TERM,
    PREC_FACTOR,
    PREC_UNARY,
    PREC_CALL,
    PREC_PRIMARY
} precedence_t;

typedef void (*parse_fn)();

typedef struct
{
    parse_fn prefix;
    parse_fn infix;
    precedence_t precedence;
} parse_rule_t;

bool compile(const char *source, chunk_t *chunk);
chunk_t *current_chunk();
void emit_return();
void emit_byte(uint8_t byte);
void emit_bytes(uint8_t byte1, uint8_t byte2);
void emit_constant(value_t value);
uint8_t make_constant(value_t value);
void end_compiler();

void consume(token_kind_t kind, const char *message);
void advance();

void error_at_current(const char *errmsg);
void error(const char *errmsg);
void error_at(token_t *token, const char *errmsg);

void expression();
void number();
void grouping();
void unary();
void binary();

void parse_precedence(precedence_t precedence);

const parse_rule_t *get_rule(token_kind_t operator_kind);
