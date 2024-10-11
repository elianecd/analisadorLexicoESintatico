// tokens.h
#ifndef TOKENS_H
#define TOKENS_H

// Enum de tipos de tokens
typedef enum {
    TOKEN_INT,
    TOKEN_REAL,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_REPEAT,
    TOKEN_UNTIL,
    TOKEN_READ,
    TOKEN_WRITE,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_LT,
    TOKEN_LTE,
    TOKEN_GT,
    TOKEN_GTE,
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_ASSIGN,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER_INT,
    TOKEN_NUMBER_REAL,
    TOKEN_EOF
} TokenType;

// Estrutura de Token
typedef struct {
    TokenType type;
    char lexeme[100];
} Token;

// Função para obter a descrição do token
const char* tokenTypeToString(TokenType type);

// Declaração das funções
void skipWhitespaceAndComments(FILE *source);
Token nextToken(FILE *source);
void advanceToken(FILE *source);

#endif