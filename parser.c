#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "syntaxTree.h"
#include "tokens.h"

// Declaração das funções que serão definidas posteriormente
Node* parseBloco(FILE *source);
Node* parseComando(FILE *source);
Node* parseExpressaoAritmetica(FILE *source);
Node* parseExpressaoRelacional(FILE *source);
Node* parseTermoAritmetico(FILE *source);
Node* parseFator(FILE *source);
Node* parseAtribuicao(FILE *source);
Node* parseSe(FILE *source);  // Função para 'se'

// Variável global para o token atual
Token currentToken;

// Função para avançar ao próximo token
void advanceToken(FILE *source) {
    currentToken = nextToken(source);
}

// Função para inicializar o analisador léxico e pegar o primeiro token
void initializeLexer(FILE *source) {
    advanceToken(source);
}

// Função para verificar e avançar o token
int match(TokenType type, FILE *source) {
    if (currentToken.type == type) {
        advanceToken(source);
        return 1;
    }
    return 0;
}

// Função de análise para `Programa`
Node* parsePrograma(FILE *source) {
    Node* root = createNode("Programa");
    root->left = parseDeclaracoes(source); // Chamada para análise de declarações
    root->right = parseCorpo(source);      // Chamada para o corpo do programa
    return root;
}

// Função de análise para `Declarações`
Node* parseDeclaracoes(FILE *source) {
    Node* node = createNode("Declaracoes");
    Node* lastVar = NULL;

    while (currentToken.type == TOKEN_INT || currentToken.type == TOKEN_REAL) {
        Node* tipoNode = createNode(currentToken.type == TOKEN_INT ? "inteiro" : "real");
        advanceToken(source);

        while (currentToken.type == TOKEN_IDENTIFIER) {
            Node* varNode = createNode(currentToken.lexeme);
            if (!tipoNode->left) {
                tipoNode->left = varNode;
            } else {
                lastVar->right = varNode;
            }
            lastVar = varNode;
            advanceToken(source);

            if (currentToken.type == TOKEN_COMMA) {
                advanceToken(source);
            } else {
                break;
            }
        }

        if (currentToken.type == TOKEN_SEMICOLON) {
            advanceToken(source);
            if (!node->left) {
                node->left = tipoNode;
            } else {
                lastVar->right = tipoNode;
            }
        } else {
            printf("Erro: ';' esperado após a declaração.\n");
            exit(1);
        }
    }
    return node;
}

// Função para processar o corpo do programa
Node* parseCorpo(FILE *source) {
    Node* node = createNode("Corpo");
    Node* lastNode = NULL;

    while (currentToken.type != TOKEN_EOF) {
        Node* comandoNode = NULL;

        // Processa a atribuição ou comandos
        if (currentToken.type == TOKEN_IDENTIFIER) {
            comandoNode = parseAtribuicao(source);
        } else if (currentToken.type == TOKEN_WRITE) {
            comandoNode = parseComando(source);
        } else if (currentToken.type == TOKEN_READ) {
            comandoNode = parseComando(source);
        } else if (currentToken.type == TOKEN_IF) {
            comandoNode = parseSe(source);
        } else if (currentToken.type == TOKEN_WHILE) {
            comandoNode = parseBloco(source);
        } else {
            // printf("Erro: Comando invalido ou bloco esperado.\n");
            break;  // Se nenhum comando é válido, saia do loop
        }

        // Adiciona o comando ao corpo
        if (comandoNode) {
            if (!node->left) {
                node->left = comandoNode; // Primeiro comando no corpo
            } else {
                lastNode->right = comandoNode; // Adiciona ao final da lista
            }
            lastNode = comandoNode; // Atualiza o último comando
        }

        advanceToken(source); // Avança para o próximo token
    }

    return node; // Retorna o nó do corpo
}

Node* parseComando(FILE *source) {
    Node* node = NULL;

    if (currentToken.type == TOKEN_IDENTIFIER) {
        // Atribuicao
        node = createNode("Atribuicao");

        Node* varNode = createNode(currentToken.lexeme);
        node->left = varNode;
        advanceToken(source);

        if (match(TOKEN_ASSIGN, source)) {
            node->right = parseExpressaoAritmetica(source);
            if (!match(TOKEN_SEMICOLON, source)) {
                printf("Erro: ';' esperado apos a atribuicao.\n");
                exit(1);
            }
        } else {
            printf("Erro: '=' esperado na atribuicao.\n");
            exit(1);
        }
    } else if (currentToken.type == TOKEN_WRITE) {  // Comando 'mostrar'
        node = createNode("Mostrar");
        advanceToken(source);

        if (match(TOKEN_LPAREN, source)) {
            if (currentToken.type == TOKEN_IDENTIFIER) {
                Node* varNode = createNode(currentToken.lexeme);
                node->left = varNode;
                advanceToken(source);

                if (!match(TOKEN_RPAREN, source)) {
                    printf("Erro: ')' esperado apos identificador no comando 'mostrar'.\n");
                    exit(1);
                }
                if (!match(TOKEN_SEMICOLON, source)) {
                    printf("Erro: ';' esperado apos comando 'mostrar'.\n");
                    exit(1);
                }
            } else {
                printf("Erro: identificador esperado apos 'mostrar('.\n");
                exit(1);
            }
        } else {
            printf("Erro: '(' esperado apos 'mostrar'.\n");
            exit(1);
        }
    } else if (currentToken.type == TOKEN_READ) {  // Comando 'ler'
        node = createNode("Ler");
        advanceToken(source);

        if (match(TOKEN_LPAREN, source)) {
            if (currentToken.type == TOKEN_IDENTIFIER) {
                Node* varNode = createNode(currentToken.lexeme);
                node->left = varNode;
                advanceToken(source);

                if (!match(TOKEN_RPAREN, source)) {
                    printf("Erro: ')' esperado apos identificador no comando 'ler'.\n");
                    exit(1);
                }
                if (!match(TOKEN_SEMICOLON, source)) {
                    printf("Erro: ';' esperado apos comando 'ler'.\n");
                    exit(1);
                }
            } else {
                printf("Erro: identificador esperado apos 'ler('.\n");
                exit(1);
            }
        } else {
            printf("Erro: '(' esperado apos 'ler'.\n");
            exit(1);
        }
    } else if (currentToken.type == TOKEN_IF) { // Comando 'se'
        node = parseSe(source);
    } else if (currentToken.type == TOKEN_WHILE) { // Comando 'enquanto'
        node = parseBloco(source);
    } else {
        // printf("Erro: Comando invalido ou bloco esperado.\n");
        exit(1);
    }

    return node;
}

Node* parseBloco(FILE *source) {
    Node* node = createNode("Enquanto");

    if (match(TOKEN_WHILE, source)) {
        if (match(TOKEN_LPAREN, source)) {
            node->left = parseExpressaoRelacional(source);
            if (match(TOKEN_RPAREN, source)) {
                if (match(TOKEN_LBRACE, source)) {  // Aguarda o '{'
                    node->right = parseCorpo(source); // Aqui, o corpo do enquanto
                    if (!match(TOKEN_RBRACE, source)) {
                        printf("Erro: '}' esperado no fim do bloco.\n");
                        exit(1);
                    }
                } else {
                    printf("Erro: '{' esperado no inicio do bloco.\n");
                    exit(1);
                }
            } else {
                printf("Erro: ')' esperado apos condicao do 'enquanto'.\n");
                exit(1);
            }
        } else {
            printf("Erro: '(' esperado apos 'enquanto'.\n");
            exit(1);
        }
    }
    return node;
}

Node* parseAtribuicao(FILE *source) {
    Node* node = createNode("Atribuicao");

    if (currentToken.type == TOKEN_IDENTIFIER) {
        Node* varNode = createNode(currentToken.lexeme);  // Identificador do lado esquerdo
        node->left = varNode;
        advanceToken(source);

        if (match(TOKEN_ASSIGN, source)) {
            node->right = parseExpressaoAritmetica(source);  // Processa a expressão do lado direito
        } else {
            printf("Erro: '=' esperado na atribuicao.\n");
            exit(1);
        }
    } else {
        printf("Erro: Identificador esperado para atribuicao.\n");
        exit(1);
    }

    return node;
}

Node* parseExpressaoAritmetica(FILE *source) {
    Node* node = parseTermoAritmetico(source);
    while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS) {
        Node* opNode = createNode(currentToken.type == TOKEN_PLUS ? "+" : "-");
        advanceToken(source);

        opNode->left = node;
        opNode->right = parseTermoAritmetico(source);

        node = opNode;
    }
    return node;
}

Node* parseExpressaoRelacional(FILE *source) {
    Node* node = parseExpressaoAritmetica(source);

    if (currentToken.type == TOKEN_LT || currentToken.type == TOKEN_LTE ||
        currentToken.type == TOKEN_GT || currentToken.type == TOKEN_GTE ||
        currentToken.type == TOKEN_EQ || currentToken.type == TOKEN_NEQ) {

        Node* opNode = createNode(tokenTypeToString(currentToken.type));
        advanceToken(source);

        opNode->left = node;
        opNode->right = parseExpressaoAritmetica(source);
        node = opNode;
    }

    return node;
}

Node* parseTermoAritmetico(FILE *source) {
    Node* node = parseFator(source);
    while (currentToken.type == TOKEN_MULTIPLY || currentToken.type == TOKEN_DIVIDE) {
        Node* opNode = createNode(currentToken.type == TOKEN_MULTIPLY ? "*" : "/");
        advanceToken(source);

        opNode->left = node;
        opNode->right = parseFator(source);

        node = opNode;
    }
    return node;
}

Node* parseFator(FILE *source) {
    Node* node = NULL;
    if (currentToken.type == TOKEN_LPAREN) {
        advanceToken(source);
        node = parseExpressaoAritmetica(source);

        if (!match(TOKEN_RPAREN, source)) {
            printf("Erro: ')' esperado.\n");
            exit(1);
        }
    } else if (currentToken.type == TOKEN_NUMBER_INT || currentToken.type == TOKEN_NUMBER_REAL) {
        node = createNode(currentToken.lexeme);
        advanceToken(source);
    } else if (currentToken.type == TOKEN_IDENTIFIER) {
        node = createNode(currentToken.lexeme);
        advanceToken(source);
    } else {
        printf("Erro: fator invalido.\n");
        exit(1);
    }
    return node;
}

Node* parseSe(FILE *source) {
    Node* node = createNode("Se");
    advanceToken(source);  // Avança para o próximo token após 'se'

    if (match(TOKEN_LPAREN, source)) {
        node->left = parseExpressaoRelacional(source);
        if (!match(TOKEN_RPAREN, source)) {
            printf("Erro: ')' esperado apos condicao do 'se'.\n");
            exit(1);
        }

        if (match(TOKEN_THEN, source)) {
            node->right = parseCorpo(source); // Processa o corpo do 'se'

            if (match(TOKEN_ELSE, source)) { // Verifica se tem um bloco 'senao'
                Node* elseNode = createNode("Senao");
                elseNode->right = parseCorpo(source); // Processa o corpo do 'senao'
                node->right = elseNode; // Adiciona o bloco 'senao' ao nó 'se'
            }
            return node;
        } else {
            printf("Erro: 'entao' esperado apos condicao do 'se'.\n");
            exit(1);
        }
    }

    return NULL; // Retorna NULL se não houver condição válida
}