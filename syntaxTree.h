// syntaxTree.h
#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#define LABEL_MAX_LENGTH 200

// Definição da estrutura Node
typedef struct Node {
    char label[LABEL_MAX_LENGTH];
    struct Node *left;
    struct Node *middle; // Certifique-se de que este campo exista
    struct Node *right;
} Node;

// Funções para manipular a árvore
Node* createNode(const char* label);
void freeTree(Node* root);
void printASTVisualToFile(Node *node, FILE *file, int depth);
void printTree(Node *root, FILE *file);

#endif // SYNTAX_TREE_H
