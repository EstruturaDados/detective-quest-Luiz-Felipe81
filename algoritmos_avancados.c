#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala (nó da árvore binária)
typedef struct Sala {
    char nome[50];           // Nome da sala (ex: "Hall de entrada")
    struct Sala* esquerda;   // Ponteiro para sala à esquerda
    struct Sala* direita;    // Ponteiro para sala à direita
} Sala;

/**
 * @brief Cria uma nova sala com o nome fornecido.
 * 
 * Aloca memória dinamicamente para uma nova sala e inicializa seus campos.
 * 
 * @param nome Nome da sala (string)
 * @return Sala* Ponteiro para a nova sala criada
 */
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro: falha na alocação de memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/**
 * @brief Permite ao jogador explorar a mansão de forma interativa.
 * 
 * Navega pela árvore binária a partir do nó atual, exibindo o nome da sala
 * e permitindo escolher entre esquerda (e), direita (d) ou sair (s).
 * A exploração termina ao chegar em uma folha (sem filhos).
 * 
 * @param atual Ponteiro para a sala atual
 */
void explorarSalas(Sala* atual) {
    char escolha;
    
    while (1) {
        printf("\nVocê está na: %s\n", atual->nome);
        
        // Verifica se é uma folha (sem caminhos)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim da exploração! Você chegou a uma sala sem saída.\n");
            break;
        }
        
        // Exibe opções disponíveis
        printf("Escolha o caminho:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Esquerda -> %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Direita  -> %s\n", atual->direita->nome);
        printf("  (s) Sair do jogo\n");
        printf("Sua escolha: ");
        
        scanf(" %c", &escolha);
        
        if (escolha == 's' || escolha == 'S') {
            printf("Exploração encerrada pelo jogador.\n");
            break;
        } else if ((escolha == 'e' || escolha == 'E') && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL) {
            atual = atual->direita;
        } else {
            printf("Opção inválida ou caminho inexistente! Tente novamente.\n");
        }
    }
}

/**
 * @brief Função principal: monta o mapa da mansão e inicia a exploração.
 * 
 * Cria a árvore binária representando a mansão de forma manual e automática
 * usando a função criarSala(). Inicia a exploração a partir do Hall.
 */
int main() {
    // Criação das salas (nós da árvore)
    Sala* hall = criarSala("Hall de entrada");
    Sala* salaEstar = criarSala("Sala de estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* quarto = criarSala("Quarto principal");
    Sala* banheiro = criarSala("Banheiro");
    Sala* jardim = criarSala("Jardim");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* escritorio = criarSala("Escritório");

    // Montagem da árvore binária (estrutura da mansão)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = quarto;
    salaEstar->direita = banheiro;

    cozinha->esquerda = jardim;
    cozinha->direita = biblioteca;

    quarto->esquerda = NULL;       // Folha
    quarto->direita = NULL;

    banheiro->esquerda = NULL;     // Folha
    banheiro->direita = NULL;

    jardim->esquerda = escritorio;
    jardim->direita = NULL;

    biblioteca->esquerda = NULL;   // Folha
    biblioteca->direita = NULL;

    escritorio->esquerda = NULL;   // Folha
    escritorio->direita = NULL;

    printf("=== BEM-VINDO AO DETECTIVE QUEST ===\n");
    printf("Explore a mansão para encontrar pistas!\n");
    printf("Use 'e' para esquerda, 'd' para direita, 's' para sair.\n\n");

    // Inicia a exploração a partir do Hall
    explorarSalas(hall);

    printf("\nObrigado por jogar!\n");
    return 0;
}