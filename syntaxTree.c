#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntaxTree.h"

#define MAX_DEPTH 100 // Limite de profundidade para evitar recursão infinita

Node* createNode(const char* label) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Erro: Falha na alocacao de memoria para node.\n");
        exit(1);
    }
    strncpy(node->label, label, LABEL_MAX_LENGTH - 1); // Segurança para evitar estouro
    node->label[LABEL_MAX_LENGTH - 1] = '\0'; // Assegura o término da string
    node->left = node->right = NULL;
    return node;
}

void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void printSpaces(FILE* file, int depth) {
    for (int i = 0; i < depth; ++i) {
        fprintf(file, "|   ");
    }
}

void printASTVisualToFile(Node *node, FILE *file, int depth) {
    if (!node) return;

    // Imprime indentação com "   " para níveis de profundidade.
    for (int i = 0; i < depth; ++i) {
        fprintf(file, "   ");
    }

    // Imprime o nó atual, com um formato de ramificação
    fprintf(file, "+-- %s\n", node->label);

    // Imprime os filhos, adicionando profundidade recursiva
    if (node->left) {
        printASTVisualToFile(node->left, file, depth + 1);
    }
    if (node->right) {
        printASTVisualToFile(node->right, file, depth + 1);
    }
}

void printTree(Node *root, FILE *file) {
    fprintf(file, "Árvore Sintática Abstrata:\n");
    printASTVisualToFile(root, file, 0);
}
