#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================
// ESTRUTURAS DE DADOS
// ==========================

// Estrutura que representa uma sala (nó da árvore binária)
typedef struct Sala {
    char nome[50];
    char pista[100]; // nova informação: pista da sala
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Estrutura que representa um nó da árvore BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// ==========================
// FUNÇÕES DE CRIAÇÃO
// ==========================

/**
 * @brief Cria uma nova sala com nome e pista fornecidos.
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro: falha na alocação de memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, ""); // sala sem pista
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/**
 * @brief Cria um novo nó de pista.
 */
PistaNode* criarPistaNode(const char* pista) {
    PistaNode* nova = (PistaNode*)malloc(sizeof(PistaNode));
    if (nova == NULL) {
        printf("Erro: falha na alocação de memória da pista!\n");
        exit(1);
    }
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ==========================
// FUNÇÕES DE BST (Pistas)
// ==========================

/**
 * @brief Insere uma pista na árvore BST em ordem alfabética.
 */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    // se for igual, ignora (evita duplicadas)
    return raiz;
}

/**
 * @brief Exibe as pistas em ordem alfabética (in-order traversal).
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// ==========================
// FUNÇÃO DE EXPLORAÇÃO
// ==========================

/**
 * @brief Explora as salas da mansão e coleta pistas.
 */
void explorarSalasComPistas(Sala* atual, PistaNode** arvorePistas) {
    char escolha;
    
    while (1) {
        printf("\nVocê está na: %s\n", atual->nome);
        
        // Se houver uma pista neste cômodo, coleta
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
            // marca a pista como coletada (opcional)
            strcpy(atual->pista, "");
        } else {
            printf("Nenhuma pista encontrada aqui.\n");
        }

        // Exibe opções de navegação
        printf("\nEscolha o caminho:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Esquerda -> %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Direita  -> %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Sua escolha: ");
        
        scanf(" %c", &escolha);

        if (escolha == 's' || escolha == 'S') {
            printf("\nExploração encerrada!\n");
            break;
        } else if ((escolha == 'e' || escolha == 'E') && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL) {
            atual = atual->direita;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// ==========================
// FUNÇÃO PRINCIPAL
// ==========================

int main() {
    // Criação das salas com pistas
    Sala* hall = criarSala("Hall de entrada", "Pegadas misteriosas no tapete");
    Sala* salaEstar = criarSala("Sala de estar", "Um relógio parado às 3h15");
    Sala* cozinha = criarSala("Cozinha", "Uma xícara de café ainda quente");
    Sala* quarto = criarSala("Quarto principal", "Uma carta rasgada sobre a cama");
    Sala* banheiro = criarSala("Banheiro", "");
    Sala* jardim = criarSala("Jardim", "Marcas de pneu no chão molhado");
    Sala* biblioteca = criarSala("Biblioteca", "Um livro antigo com anotações suspeitas");
    Sala* escritorio = criarSala("Escritório", "Um cofre trancado");

    // Montagem da árvore da mansão
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = quarto;
    salaEstar->direita = banheiro;

    cozinha->esquerda = jardim;
    cozinha->direita = biblioteca;

    jardim->esquerda = escritorio;

    // Inicializa a árvore de pistas (vazia)
    PistaNode* arvorePistas = NULL;

    printf("=== BEM-VINDO AO DETECTIVE QUEST ===\n");
    printf("Explore a mansão e colete todas as pistas!\n");
    printf("Use 'e' para esquerda, 'd' para direita, 's' para sair.\n");

    // Inicia a exploração
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibe pistas coletadas
    printf("\n=== PISTAS COLETADAS ===\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}