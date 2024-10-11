#include <stdio.h>
#include <locale.h>
#include <time.h>

#include "parser.h"
#include "tokens.h"
#include "syntaxTree.h"

// Variável global para armazenar o token atual
extern Token currentToken;

int main() {
    setlocale(LC_ALL, "Portuguese");

    int continuar = 1;
    int counter = 1;

    while (continuar) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        char lex_output_filename[64];
        snprintf(lex_output_filename, sizeof(lex_output_filename),
                 "lex_output_%d-%02d-%02d_%02d-%02d-%02d_%d.txt",
                 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                 tm.tm_hour, tm.tm_min, tm.tm_sec, counter);

        char syntax_output_filename[64];
        snprintf(syntax_output_filename, sizeof(syntax_output_filename),
                 "syntax_output_%d-%02d-%02d_%02d-%02d-%02d_%d.txt",
                 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                 tm.tm_hour, tm.tm_min, tm.tm_sec, counter);

        int escolha;
        printf("Escolha um arquivo de entrada:\n1. input1.p-\n2. input2.p-\n3. input3.p-\nDigite sua escolha (1-3):\n");
        if (scanf("%d", &escolha) != 1) {
            fprintf(stderr, "Erro ao ler a escolha. Saindo.\n");
            return 1;
        }

        char *input_filename;
        switch (escolha) {
            case 1: input_filename = "input1.p-"; break;
            case 2: input_filename = "input2.p-"; break;
            case 3: input_filename = "input3.p-"; break;
            default: input_filename = "input1.p-"; break;
        }

        FILE *source = fopen(input_filename, "r");
        if (!source) {
            fprintf(stderr, "Erro ao abrir o arquivo de entrada.\n");
            return 1;
        }

        FILE *lex_output = fopen(lex_output_filename, "w");
        if (!lex_output) {
            fprintf(stderr, "Erro ao criar o arquivo de saida lexico.\n");
            fclose(source);
            return 1;
        }

        initializeLexer(source);
        // Imprimir tokens no arquivo de saída léxica
        while (currentToken.type != TOKEN_EOF) {
            fprintf(lex_output, "Token: %d, Lexema: %s, Descricao: %s\n",
                    currentToken.type, currentToken.lexeme, tokenTypeToString(currentToken.type));
            advanceToken(source); // Avança para o próximo token
        }
        fclose(lex_output);

        fseek(source, 0, SEEK_SET);
        initializeLexer(source);  // Reinicializa para a análise sintática

        Node *syntaxTree = parsePrograma(source);

        FILE *syntax_output = fopen(syntax_output_filename, "w");
        if (!syntax_output) {
            fprintf(stderr, "Erro ao criar o arquivo de saida sintatica.\n");
            fclose(source);
            freeTree(syntaxTree);
            return 1;
        }

        printTree(syntaxTree, syntax_output);
        fclose(syntax_output);

        freeTree(syntaxTree);
        fclose(source);

        printf("Analise concluida. Verifique '%s' e '%s' para detalhes.\n", lex_output_filename, syntax_output_filename);

        printf("Deseja analisar outro arquivo? (1 - Sim, 0 - Nao):\n");
        if (scanf("%d", &continuar) != 1) {
            fprintf(stderr, "Erro ao ler a escolha. Saindo.\n");
            break;
        }

        counter++;
    }

    return 0;
}
