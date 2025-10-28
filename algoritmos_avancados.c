#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ==========================
// DEFINIÇÕES E CONSTANTES
// ==========================
#define TAM_HASH 53          // Tamanho da tabela hash (primo)
#define MAX_SUSPEITOS 10
#define MAX_PILHA 50         // Profundidade máxima da pilha de navegação

// ==========================
// ESTRUTURAS DE DADOS
// ==========================

// Sala da mansão (árvore binária)
typedef struct Sala {
    char nome[50];
    char pista[100];          // pista associada (vazia = "")
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Nó da BST de pistas coletadas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Entrada da tabela hash (pista → suspeito)
typedef struct HashEntry {
    char pista[100];
    char suspeito[50];
    struct HashEntry* proximo;
} HashEntry;

// Tabela hash
typedef struct {
    HashEntry* tabela[TAM_HASH];
} HashTable;

// ==========================
// FUNÇÕES DE CRIAÇÃO
// ==========================

/**
 * @brief Cria uma nova sala com nome e pista.
 * @param nome Nome da sala.
 * @param pista Pista associada (pode ser vazia).
 * @return Ponteiro para a sala alocada.
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro: falha na alocação de memória para sala!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista != NULL && strlen(pista) > 0)
        strcpy(nova->pista, pista);
    else
        nova->pista[0] = '\0';
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/**
 * @brief Cria um nó para a BST de pistas.
 */
PistaNode* criarPistaNode(const char* pista) {
    PistaNode* nova = (PistaNode*)malloc(sizeof(PistaNode));
    if (nova == NULL) {
        printf("Erro: falha na alocação de memória para pista!\n");
        exit(1);
    }
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ==========================
// FUNÇÕES DA TABELA HASH
// ==========================

/**
 * @brief Função de hash djb2 adaptada.
 */
unsigned int hash(const char* str) {
    unsigned int h = 5381;
    int c;
    while ((c = *str++))
        h = ((h << 5) + h) + c;
    return h % TAM_HASH;
}

/**
 * @brief Inicializa a tabela hash.
 */
void inicializarHash(HashTable* ht) {
    for (int i = 0; i < TAM_HASH; i++) {
        ht->tabela[i] = NULL;
    }
}

/**
 * @brief Insere associação pista → suspeito na hash.
 */
void inserirNaHash(HashTable* ht, const char* pista, const char* suspeito) {
    unsigned int idx = hash(pista);
    HashEntry* nova = (HashEntry*)malloc(sizeof(HashEntry));
    if (nova == NULL) {
        printf("Erro: falha na alocação de memória para hash!\n");
        exit(1);
    }
    strcpy(nova->pista, pista);
    strcpy(nova->suspeito, suspeito);
    nova->proximo = ht->tabela[idx];
    ht->tabela[idx] = nova;
}

/**
 * @brief Busca suspeito por pista.
 * @return Nome do suspeito ou NULL.
 */
const char* encontrarSuspeito(HashTable* ht, const char* pista) {
    unsigned int idx = hash(pista);
    HashEntry* atual = ht->tabela[idx];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->proximo;
    }
    return NULL;
}

// ==========================
// FUNÇÕES DA BST DE PISTAS
// ==========================

/**
 * @brief Insere pista na BST (ordem alfabética).
 */
PistaNode* adicionarPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) return criarPistaNode(pista);
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = adicionarPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = adicionarPista(raiz->direita, pista);
    return raiz;
}

/**
 * @brief Exibe pistas em ordem (in-order).
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("  • %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/**
 * @brief Conta quantas pistas apontam para um suspeito.
 */
int contarPistasDoSuspeito(PistaNode* raiz, HashTable* ht, const char* alvo) {
    if (raiz == NULL) return 0;
    int count = 0;
    const char* sus = encontrarSuspeito(ht, raiz->pista);
    if (sus != NULL && strcmp(sus, alvo) == 0) count = 1;
    return count +
           contarPistasDoSuspeito(raiz->esquerda, ht, alvo) +
           contarPistasDoSuspeito(raiz->direita, ht, alvo);
}

// ==========================
// FUNÇÃO DE EXPLORAÇÃO (COM VOLTAR)
// ==========================

/**
 * @brief Navega pela mansão com suporte a voltar (v).
 * @param raiz Sala inicial.
 * @param arvorePistas BST de pistas coletadas.
 * @param ht Tabela hash.
 */
void explorarSalas(Sala* raiz, PistaNode** arvorePistas, HashTable* ht) {
    if (raiz == NULL) return;

    Sala* atual = raiz;
    Sala* pilha[MAX_PILHA];   // Histórico de salas
    int topo = -1;            // Índice do topo
    char buffer[100];

    printf("\nExploração iniciada no Hall de Entrada.\n");

    while (1) {
        printf("\n\033[1mVocê está na: %s\033[0m\n", atual->nome);

        // === Coleta pista ===
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"\033[33m%s\033[0m\"\n", atual->pista);
            *arvorePistas = adicionarPista(*arvorePistas, atual->pista);
            atual->pista[0] = '\0';  // Marca como coletada
        } else {
            printf("Nenhuma pista nova aqui.\n");
        }

        // === Opções de movimento ===
        printf("\nEscolha o próximo movimento:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Esquerda → %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Direita → %s\n", atual->direita->nome);
        if (topo >= 0)
            printf("  (v) Voltar → %s\n", pilha[topo]->nome);
        printf("  (s) Sair da exploração\n");
        printf("Sua escolha: ");

        // === Entrada segura ===
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro de leitura. Saindo...\n");
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove \n
        if (strlen(buffer) == 0) {
            printf("Nenhuma opção. Tente novamente.\n");
            continue;
        }

        char escolha = tolower(buffer[0]);

        // === Processa comando ===
        if (escolha == 's') {
            printf("\nExploração finalizada. Hora do julgamento!\n");
            break;
        }
        else if (escolha == 'v' && topo >= 0) {
            atual = pilha[topo--];
            printf("Voltando para: %s\n", atual->nome);
            continue;
        }
        else if (escolha == 'e' && atual->esquerda != NULL) {
            pilha[++topo] = atual;
            atual = atual->esquerda;
        }
        else if (escolha == 'd' && atual->direita != NULL) {
            pilha[++topo] = atual;
            atual = atual->direita;
        }
        else {
            if (escolha == 'e' || escolha == 'd')
                printf("Esse caminho não existe!\n");
            else if (escolha == 'v')
                printf("Você já está no ponto de partida.\n");
            else
                printf("Comando inválido! Use 'e', 'd', 'v' ou 's'.\n");
        }
    }
}

// ==========================
// JULGAMENTO FINAL
// ==========================

/**
 * @brief Fase final: acusação e validação.
 */
void verificarSuspeitoFinal(PistaNode* arvorePistas, HashTable* ht) {
    if (arvorePistas == NULL) {
        printf("\nNenhuma pista coletada. Impossível acusar.\n");
        return;
    }

    char acusacao[50];
    printf("\n=== JULGAMENTO FINAL ===\n");
    printf("Pistas coletadas:\n");
    exibirPistas(arvorePistas);

    printf("\nQuem você acusa? ");
    if (fgets(acusacao, sizeof(acusacao), stdin) == NULL) {
        printf("Erro de entrada.\n");
        return;
    }
    acusacao[strcspn(acusacao, "\n")] = '\0';
    if (strlen(acusacao) == 0) {
        printf("Nenhum nome digitado.\n");
        return;
    }

    int count = contarPistasDoSuspeito(arvorePistas, ht, acusacao);

    printf("\n\033[1mResultado:\033[0m\n");
    if (count >= 2) {
        printf("SUCESSO! %d pista(s) contra \033[1m%s\033[0m.\n", count, acusacao);
        printf("Acusação \033[1;32mVÁLIDA\033[0m! Culpado identificado!\n");
    } else {
        printf("FALHA. Apenas %d pista(s) contra %s.\n", count, acusacao);
        printf("Necessário \033[1mPELO MENOS 2\033[0m para acusar.\n");
    }
}

// ==========================
// MAIN
// ==========================
int main() {
    // === Salas ===
    Sala* hall       = criarSala("Hall de entrada",      "Pegadas misteriosas no tapete");
    Sala* salaEstar  = criarSala("Sala de estar",        "Um relógio parado às 3h15");
    Sala* cozinha    = criarSala("Cozinha",              "Uma xícara de café ainda quente");
    Sala* quarto     = criarSala("Quarto principal",     "Uma carta rasgada sobre a cama");
    Sala* banheiro   = criarSala("Banheiro",             "");
    Sala* jardim     = criarSala("Jardim",               "Marcas de pneu no chão molhado");
    Sala* biblioteca = criarSala("Biblioteca",           "Um livro antigo com anotações suspeitas");
    Sala* escritorio = criarSala("Escritório",           "Um cofre trancado");

    // === Árvore da mansão ===
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = quarto;
    salaEstar->direita = banheiro;
    cozinha->esquerda = jardim;
    cozinha->direita = biblioteca;
    jardim->esquerda = escritorio;

    // === Tabela hash: pista → suspeito ===
    HashTable ht;
    inicializarHash(&ht);
    inserirNaHash(&ht, "Pegadas misteriosas no tapete",          "Jardineiro");
    inserirNaHash(&ht, "Um relógio parado às 3h15",              "Mordomo");
    inserirNaHash(&ht, "Uma xícara de café ainda quente",        "Empregada");
    inserirNaHash(&ht, "Uma carta rasgada sobre a cama",         "Esposa");
    inserirNaHash(&ht, "Marcas de pneu no chão molhado",         "Motorista");
    inserirNaHash(&ht, "Um livro antigo com anotações suspeitas","Bibliotecário");
    inserirNaHash(&ht, "Um cofre trancado",                      "Sócio");

    // === BST de pistas ===
    PistaNode* arvorePistas = NULL;

    // === Início ===
    printf("=== BEM-VINDO AO DETECTIVE QUEST ===\n");
    printf("Explore a mansão, colete pistas e descubra o culpado!\n");
    printf("Comandos: (e) esquerda, (d) direita, (v) voltar, (s) sair\n\n");

    explorarSalas(hall, &arvorePistas, &ht);
    verificarSuspeitoFinal(arvorePistas, &ht);

    printf("\nObrigado por jogar!\n");
    return 0;
}