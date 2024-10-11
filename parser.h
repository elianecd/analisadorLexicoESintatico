#ifndef PARSER_H
#define PARSER_H

#include "syntaxTree.h"

// Inicializa o analisador léxico
void initializeLexer(FILE *source);

// Funções para a análise sintática
Node* parsePrograma(FILE *source);
Node* parseDeclaracoes(FILE *source);
Node* parseDeclaracao(FILE *source);
Node* parseCorpo(FILE *source);
Node* parseComando(FILE *source);
Node* parseExpressaoAritmetica(FILE *source);
Node* parseExpressaoRelacional(FILE *source);
Node* parseTermoAritmetico(FILE *source);
Node* parseFator(FILE *source);

#endif
