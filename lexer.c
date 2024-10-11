#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "tokens.h"

// Função para ignorar espaços em branco e comentários
void skipWhitespaceAndComments(FILE *source) {
    char c = fgetc(source);

    while (c != EOF) {
        // Ignorar espaços em branco
        if (c == ' ' || c == '\t' || c == '\n') {
            c = fgetc(source);
            continue;
        }

        // Ignorar comentários /*...*/
        if (c == '/') {
            char next = fgetc(source);
            if (next == '*') {
                // Ignorar tudo até encontrar */
                while (1) {
                    c = fgetc(source);
                    if (c == EOF) return; // Para evitar loop infinito no final do arquivo
                    if (c == '*' && (next = fgetc(source)) == '/') break; // Encerra o comentário
                }
                c = fgetc(source); // Lê o próximo caractere após o comentário
                continue;
            } else {
                ungetc(next, source); // Coloca o caractere de volta
            }
        }

        // Se não for espaço ou comentário, break
        break;
    }

    if (c != EOF) {
        ungetc(c, source); // Coloca o caractere lido de volta se não for EOF
    }
}

// Função para obter o próximo token
Token nextToken(FILE *source) {
    char c;
    Token token;

    skipWhitespaceAndComments(source);
    c = fgetc(source);

    // Verifica se o arquivo chegou ao final
    if (c == EOF) {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }

    // Reconhecimento de palavras-chave e identificadores
    if (isalpha(c)) {
        char buffer[100];
        int i = 0;
        do {
            buffer[i++] = c;
            c = fgetc(source);
        } while (isalnum(c) || c == '_');
        buffer[i] = '\0';
        ungetc(c, source);

        // Comparar buffer com palavras-chave
        if (strcmp(buffer, "inteiro") == 0) {
            token.type = TOKEN_INT;
        } else if (strcmp(buffer, "real") == 0) {
            token.type = TOKEN_REAL;
        } else if (strcmp(buffer, "se") == 0) {
            token.type = TOKEN_IF;
        } else if (strcmp(buffer, "entao") == 0) {
            token.type = TOKEN_THEN;
        } else if (strcmp(buffer, "senao") == 0) {
            token.type = TOKEN_ELSE;
        } else if (strcmp(buffer, "enquanto") == 0) {
            token.type = TOKEN_WHILE;
        } else if (strcmp(buffer, "repita") == 0) {
            token.type = TOKEN_REPEAT;
        } else if (strcmp(buffer, "ate") == 0) {
            token.type = TOKEN_UNTIL;
        } else if (strcmp(buffer, "ler") == 0) {
            token.type = TOKEN_READ;
        } else if (strcmp(buffer, "mostrar") == 0) {
            token.type = TOKEN_WRITE;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        strcpy(token.lexeme, buffer);
    }
    // Reconhecimento de números (inteiros ou reais)
    else if (isdigit(c)) {
        char buffer[100];
        int i = 0;
        int isReal = 0;

        do {
            buffer[i++] = c;
            c = fgetc(source);
            if (c == '.') {
                isReal = 1;
                buffer[i++] = c;
                c = fgetc(source);
            }
        } while (isdigit(c));
        buffer[i] = '\0';
        ungetc(c, source);

        token.type = isReal ? TOKEN_NUMBER_REAL : TOKEN_NUMBER_INT;
        strcpy(token.lexeme, buffer);
    }
    // Reconhecimento de operadores e pontuação
    else if (c == '+') {
        token.type = TOKEN_PLUS;
        strcpy(token.lexeme, "+");
    } else if (c == '-') {
        token.type = TOKEN_MINUS;
        strcpy(token.lexeme, "-");
    } else if (c == '*') {
        token.type = TOKEN_MULTIPLY;
        strcpy(token.lexeme, "*");
    } else if (c == '/') {
        token.type = TOKEN_DIVIDE;
        strcpy(token.lexeme, "/");
    } else if (c == '=') {
        char next = fgetc(source);
        if (next == '=') {
            token.type = TOKEN_EQ;
            strcpy(token.lexeme, "==");
        } else {
            ungetc(next, source);
            token.type = TOKEN_ASSIGN;
            strcpy(token.lexeme, "=");
        }
    } else if (c == '<') {
        char next = fgetc(source);
        if (next == '=') {
            token.type = TOKEN_LTE;
            strcpy(token.lexeme, "<=");
        } else {
            ungetc(next, source);
            token.type = TOKEN_LT;
            strcpy(token.lexeme, "<");
        }
    } else if (c == '>') {
        char next = fgetc(source);
        if (next == '=') {
            token.type = TOKEN_GTE;
            strcpy(token.lexeme, ">=");
        } else {
            ungetc(next, source);
            token.type = TOKEN_GT;
            strcpy(token.lexeme, ">");
        }
    } else if (c == '&') {
        char next = fgetc(source);
        if (next == '&') {
            token.type = TOKEN_AND;
            strcpy(token.lexeme, "&&");
        } else {
            printf("Erro léxico: caractere inesperado '&'.\n");
            exit(1);
        }
    } else if (c == '|') {
        char next = fgetc(source);
        if (next == '|') {
            token.type = TOKEN_OR;
            strcpy(token.lexeme, "||");
        } else {
            printf("Erro léxico: caractere inesperado '|'.\n");
            exit(1);
        }
    }
    // Reconhecimento de pontuação
    else if (c == ';') {
        token.type = TOKEN_SEMICOLON;
        strcpy(token.lexeme, ";");
    } else if (c == ',') {
        token.type = TOKEN_COMMA;
        strcpy(token.lexeme, ",");
    } else if (c == '(') {
        token.type = TOKEN_LPAREN;
        strcpy(token.lexeme, "(");
    } else if (c == ')') {
        token.type = TOKEN_RPAREN;
        strcpy(token.lexeme, ")");
    } else if (c == '{') {
        token.type = TOKEN_LBRACE;
        strcpy(token.lexeme, "{");
    } else if (c == '}') {
        token.type = TOKEN_RBRACE;
        strcpy(token.lexeme, "}");
    } else {
        printf("Erro léxico: caractere inesperado '%c'.\n", c);
        exit(1);
    }

    return token;
}

// Função para obter a descrição do tipo de token
const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TOKEN_INT: return "Tipo inteiro";
        case TOKEN_REAL: return "Tipo real";
        case TOKEN_IF: return "Início de estrutura condicional (se)";
        case TOKEN_THEN: return "Bloco então";
        case TOKEN_ELSE: return "Bloco senão";
        case TOKEN_WHILE: return "Estrutura de repetição (enquanto)";
        case TOKEN_REPEAT: return "Início de repetição (repita)";
        case TOKEN_UNTIL: return "Fim de repetição (até)";
        case TOKEN_READ: return "Comando de entrada (ler)";
        case TOKEN_WRITE: return "Comando de saída (mostrar)";
        case TOKEN_PLUS: return "Operador de soma (+)";
        case TOKEN_MINUS: return "Operador de subtração (-)";
        case TOKEN_MULTIPLY: return "Operador de multiplicação (*)";
        case TOKEN_DIVIDE: return "Operador de divisão (/)";
        case TOKEN_AND: return "Operador lógico E (&&)";
        case TOKEN_OR: return "Operador lógico OU (||)";
        case TOKEN_LT: return "Operador menor que (<)";
        case TOKEN_LTE: return "Operador menor ou igual (<=)";
        case TOKEN_GT: return "Operador maior que (>)";
        case TOKEN_GTE: return "Operador maior ou igual (>=)";
        case TOKEN_EQ: return "Operador de igualdade (==)";
        case TOKEN_NEQ: return "Operador diferente (!=)";
        case TOKEN_ASSIGN: return "Operador de atribuição (=)";
        case TOKEN_SEMICOLON: return "Separador de comandos (;)";
        case TOKEN_COMMA: return "Separador de identificadores (,)";
        case TOKEN_LPAREN: return "Parêntese de abertura (()";
        case TOKEN_RPAREN: return "Parêntese de fechamento ())";
        case TOKEN_LBRACE: return "Chave de abertura ({)";
        case TOKEN_RBRACE: return "Chave de fechamento (})";
        case TOKEN_IDENTIFIER: return "Identificador";
        case TOKEN_NUMBER_INT: return "Número inteiro";
        case TOKEN_NUMBER_REAL: return "Número real";
        case TOKEN_EOF: return "Fim do arquivo";
        default: return "Tipo desconhecido";
    }
}