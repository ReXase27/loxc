#include "compiler.h"

#include "chunk.h"

parser_t parser;
chunk_t *compiling_chunk;

bool compile(const char *source, chunk_t *chunk)
{
    init_scanner(source);
    compiling_chunk = chunk;

    parser.had_error = false;
    parser.panic_mode = false;

    advance();
    expression();
    consume(TOKEN_EOF, "Expected end of expression.");

    end_compiler();

    return !parser.had_error;
}

static void advance()
{
    parser.previous = parser.current;

    while (true)
    {
        parser.current = scan_token();
        if (parser.current.kind != TOKEN_ERROR)
            break;

        error_at_current(parser.current.start);
    }
}

static void error_at_current(const char *errmsg)
{
    error_at(&parser.previous, errmsg);
}

static void error(const char *errmsg)
{
    error_at(&parser.previous, errmsg);
}

static void error_at(token_t *token, const char *errmsg)
{
    if (parser.panic_mode)
        return;

    parser.panic_mode = true;

    fprintf(stderr, "[line: %d] Error", token->line);

    if (token->kind == TOKEN_EOF)
    {
        fprintf(stderr, " at end");
    }
    else if (token->kind == TOKEN_ERROR)
    {
    }
    else
    {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", errmsg);
    parser.had_error = true;
}

static void consume(token_kind_t kind, const char *message)
{
    if (parser.current.kind == kind)
    {
        advance();
        return;
    }

    error_at_current(message);
}

static void emit_byte(uint8_t byte)
{
    write_chunk(current_chunk(), byte, parser.previous.line);
}

static chunk_t *current_chunk()
{
    return compiling_chunk;
}

static void end_compiler()
{
    emit_return();
#ifdef DEBUG_PRINT_CODE
#include "debug.h"
    if (!parser.had_error)
        disassemble_chunk(current_chunk(), "code");
#endif
}

static void emit_return()
{
    emit_byte(TOKEN_RETURN);
}

static void emit_bytes(uint8_t byte1, uint8_t byte2)
{
    emit_byte(byte1);
    emit_byte(byte2);
}

static void emit_constant(value_t value)
{
    emit_bytes(OP_CONSTANT, make_constant(value));
}

static uint8_t make_constant(value_t value)
{
    int constant = add_constant(current_chunk(), value);
    if (constant > UINT8_MAX)
    {
        error("Too many constants in one chunk");
        return 0;
    }

    return (uint8_t)constant;
}

static void expression()
{
    parse_precedence(PREC_ASSIGNMENT);
}

static void number()
{
    double value = strtod(parser.previous.start, NULL);
    emit_constant(value);
}

static void grouping()
{
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void unary()
{
    token_kind_t operator_kind = parser.previous.kind;

    // FIX: possible bad prec parse here !!
    parse_precedence(PREC_UNARY);

    expression();

    switch (operator_kind)
    {
    case TOKEN_MINUS:
        emit_byte(OP_NEGATE);
        break;
    default:
        return; // Unreachable.
    }
}

static const parse_rule_t rules[] = {
    [TOKEN_LEFT_PAREN] = {grouping, NULL, PREC_NONE},
    [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},
    [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},
    [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},
    [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
    [TOKEN_DOT] = {NULL, NULL, PREC_NONE},
    [TOKEN_MINUS] = {unary, binary, PREC_TERM},
    [TOKEN_PLUS] = {NULL, binary, PREC_TERM},
    [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},
    [TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},
    [TOKEN_STAR] = {NULL, binary, PREC_FACTOR},
    [TOKEN_BANG] = {NULL, NULL, PREC_NONE},
    [TOKEN_BANG_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_EQUAL_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_GREATER] = {NULL, NULL, PREC_NONE},
    [TOKEN_GREATER_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_LESS] = {NULL, NULL, PREC_NONE},
    [TOKEN_LESS_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_IDENTIFIER] = {NULL, NULL, PREC_NONE},
    [TOKEN_STRING] = {NULL, NULL, PREC_NONE},
    [TOKEN_NUMBER] = {number, NULL, PREC_NONE},
    [TOKEN_AND] = {NULL, NULL, PREC_NONE},
    [TOKEN_CLASS] = {NULL, NULL, PREC_NONE},
    [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
    [TOKEN_FALSE] = {NULL, NULL, PREC_NONE},
    [TOKEN_FOR] = {NULL, NULL, PREC_NONE},
    [TOKEN_FUN] = {NULL, NULL, PREC_NONE},
    [TOKEN_IF] = {NULL, NULL, PREC_NONE},
    [TOKEN_NIL] = {NULL, NULL, PREC_NONE},
    [TOKEN_OR] = {NULL, NULL, PREC_NONE},
    [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
    [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
    [TOKEN_SUPER] = {NULL, NULL, PREC_NONE},
    [TOKEN_THIS] = {NULL, NULL, PREC_NONE},
    [TOKEN_TRUE] = {NULL, NULL, PREC_NONE},
    [TOKEN_VAR] = {NULL, NULL, PREC_NONE},
    [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},
    [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},
    [TOKEN_EOF] = {NULL, NULL, PREC_NONE},
};

static void binary()
{
    token_kind_t operator_kind = parser.previous.kind;

    const parse_rule_t *rule = get_rule(operator_kind);

    parse_precedence((precedence_t)(rule->precedence + 1));

    switch (operator_kind)
    {
    case TOKEN_PLUS:
        emit_byte(OP_ADD);
        break;
    case TOKEN_MINUS:
        emit_byte(OP_SUBTRACT);
        break;
    case TOKEN_STAR:
        emit_byte(OP_MULTIPLY);
        break;
    case TOKEN_SLASH:
        emit_byte(OP_DIVIDE);
        break;
    default:
        return; // Unreachable.
    }
}

static void parse_precedence(precedence_t precedence)
{
    advance();
    parse_fn prefix_rule = get_rule(parser.previous.kind)->prefix;

    if (prefix_rule == NULL)
    {
        error("Expected expression.");
        return;
    }

    prefix_rule();

    while (precedence <= get_rule(parser.current.kind)->precedence)
    {
        advance();
        parse_fn infix_rule = get_rule(parser.previous.kind)->infix;
        infix_rule();
    }
}

static const parse_rule_t *get_rule(token_kind_t operator_kind)
{
    return &rules[operator_kind];
}
