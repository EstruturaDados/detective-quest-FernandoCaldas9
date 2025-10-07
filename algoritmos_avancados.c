#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 100 // tamanho da tabela hash para associar pistas a suspeitos

// ============================
// ESTRUTURAS DE DADOS
// ============================

// Nó da árvore de salas da mansão
typedef struct Sala {
    char nome[50];           // Nome do cômodo
    char pista[100];         // Pista associada ao cômodo
    struct Sala* esquerda;   // Ponteiro para sala à esquerda
    struct Sala* direita;    // Ponteiro para sala à direita
} Sala;

// Nó da BST de pistas coletadas
typedef struct PistaNode {
    char pista[100];              // Conteúdo da pista
    struct PistaNode* esquerda;   // Filho esquerdo na BST
    struct PistaNode* direita;    // Filho direito na BST
} PistaNode;

// Nó da tabela hash associando pista a suspeito
typedef struct HashNode {
    char pista[100];          // Chave: pista
    char suspeito[50];        // Valor: suspeito associado
    struct HashNode* proximo; // Ponteiro para próximo nó em caso de colisão
} HashNode;

// ============================
// FUNÇÕES DE CRIAÇÃO
// ============================

// Cria dinamicamente uma sala da mansão
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala)); // aloca memória
    if (!nova) { printf("Erro de alocacao!\n"); exit(1); }
    strcpy(nova->nome, nome);   // copia o nome da sala
    strcpy(nova->pista, pista); // copia a pista associada
    nova->esquerda = NULL;      // inicializa filhos como NULL
    nova->direita = NULL;
    return nova;
}

// Cria nó de pista para BST
PistaNode* criarPistaNode(const char* pista) {
    PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
    strcpy(novo->pista, pista); // copia o conteúdo da pista
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// ============================
// FUNÇÕES BST
// ============================

// Insere uma pista na BST (ordem alfabética)
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (!raiz) return criarPistaNode(pista); // se árvore vazia, cria nó
    if (strcmp(pista, raiz->pista) < 0)      // menor que raiz -> esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0) // maior que raiz -> direita
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz; // duplicatas ignoradas
}

// Exibe pistas em ordem alfabética (em-ordem)
void exibirPistas(PistaNode* raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda); // filho esquerdo
    printf("- %s\n", raiz->pista); // raiz
    exibirPistas(raiz->direita); // filho direito
}

// ============================
// FUNÇÕES HASH
// ============================

// Função hash simples baseada em string
unsigned int hash(const char* str) {
    unsigned int h = 0;
    while (*str) h = (h * 31) + *str++;
    return h % HASH_SIZE;
}

// Insere associação pista -> suspeito na tabela hash
void inserirNaHash(HashNode* tabela[], const char* pista, const char* suspeito) {
    unsigned int idx = hash(pista); // calcula índice
    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabela[idx]; // insere no início da lista encadeada
    tabela[idx] = novo;
}

// Busca suspeito de uma pista na hash
const char* encontrarSuspeito(HashNode* tabela[], const char* pista) {
    unsigned int idx = hash(pista);
    HashNode* atual = tabela[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito; // retorna suspeito encontrado
        atual = atual->proximo;
    }
    return NULL; // não encontrado
}

// ============================
// EXPLORAÇÃO DA MANSÃO
// ============================

// Função recursiva para explorar salas e coletar pistas
void explorarSalas(Sala* atual, PistaNode** arvorePistas, HashNode* tabelaHash[]) {
    if (!atual) return;

    printf("\nVoce esta na sala: %s\n", atual->nome);

    // Coleta pista, se existir
    if (strlen(atual->pista) > 0) {
        printf("Voce encontrou uma pista: %s\n", atual->pista);
        *arvorePistas = inserirPista(*arvorePistas, atual->pista);
    }

    // Se a sala é folha (sem caminhos)
    if (!atual->esquerda && !atual->direita) {
        printf("Nao ha mais caminhos a seguir.\n");
        return;
    }

    char escolha;
    do {
        printf("Escolha o caminho: esquerda (e), direita (d) ou sair (s): ");
        scanf(" %c", &escolha);
        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda) explorarSalas(atual->esquerda, arvorePistas, tabelaHash);
            else printf("Nao ha sala a esquerda!\n");
            break;
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita) explorarSalas(atual->direita, arvorePistas, tabelaHash);
            else printf("Nao ha sala a direita!\n");
            break;
        } else if (escolha == 's' || escolha == 'S') {
            printf("Exploracao encerrada.\n");
            break;
        } else {
            printf("Opcao invalida! Tente novamente.\n");
        }
    } while(1);
}

// ============================
// FASE DE JULGAMENTO
// ============================

void verificarSuspeitoFinal(PistaNode* arvorePistas, HashNode* tabelaHash[], const char* suspeitoAcusado) {
    int contador = 0;

    // Função recursiva interna para percorrer BST e contar pistas do suspeito
    void contar(PistaNode* raiz) {
        if (!raiz) return;
        const char* suspeito = encontrarSuspeito(tabelaHash, raiz->pista);
        if (suspeito && strcmp(suspeito, suspeitoAcusado) == 0)
            contador++;
        contar(raiz->esquerda);
        contar(raiz->direita);
    }

    contar(arvorePistas);

    printf("\n%d pistas coletadas apontam para %s.\n", contador, suspeitoAcusado);

    if (contador >= 2)
        printf("Acusacao correta! %s e o culpado.\n", suspeitoAcusado);
    else
        printf("Acusacao incorreta! Pistas insuficientes contra %s.\n", suspeitoAcusado);
}

// ============================
// FUNÇÃO PRINCIPAL
// ============================

int main() {
    // Inicializa tabela hash vazia
    HashNode* tabelaHash[HASH_SIZE] = {NULL};

    // Criar salas da mansão
    Sala* hall = criarSala("Hall de Entrada", "Pegada de lama");
    Sala* sala1 = criarSala("Sala de Estar", "Carta suspeita");
    Sala* sala2 = criarSala("Cozinha", "Chave dourada");
    Sala* sala3 = criarSala("Biblioteca", "Livro fora do lugar");
    Sala* sala4 = criarSala("Jardim", "Fio de cabelo");
    Sala* sala5 = criarSala("Quarto Secreto", "Diario antigo");

    // Montando conexões da árvore
    hall->esquerda = sala1;
    hall->direita = sala2;
    sala1->esquerda = sala3;
    sala1->direita = sala4;
    sala2->direita = sala5;

    // Associar pistas a suspeitos
    inserirNaHash(tabelaHash, "Pegada de lama", "Sr. Verde");
    inserirNaHash(tabelaHash, "Carta suspeita", "Sra. Rosa");
    inserirNaHash(tabelaHash, "Chave dourada", "Sr. Azul");
    inserirNaHash(tabelaHash, "Livro fora do lugar", "Sra. Rosa");
    inserirNaHash(tabelaHash, "Fio de cabelo", "Sr. Verde");
    inserirNaHash(tabelaHash, "Diario antigo", "Sr. Azul");

    // Inicializa BST de pistas coletadas
    PistaNode* arvorePistas = NULL;

    printf("Bem-vindo ao Detective Quest: fase final!\n");

    // Explora a mansão e coleta pistas
    explorarSalas(hall, &arvorePistas, tabelaHash);

    // Exibe todas as pistas coletadas em ordem alfabética
    printf("\n=== Pistas coletadas em ordem alfabetica ===\n");
    exibirPistas(arvorePistas);

    // Solicita acusacao final do jogador
    char acusacao[50];
    printf("\nInforme o suspeito que deseja acusar: ");
    scanf(" %[^\n]", acusacao);

    // Verifica se a acusacao está correta
    verificarSuspeitoFinal(arvorePistas, tabelaHash, acusacao);

    // Libera memória das salas
    free(sala5); free(sala4); free(sala3); free(sala2); free(sala1); free(hall);

    return 0;
}




