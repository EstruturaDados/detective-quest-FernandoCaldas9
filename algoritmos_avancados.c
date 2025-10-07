#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10  // tamanho da tabela hash

// ==========================
// STRUCTS
// ==========================

// Nó da árvore da mansão (cômodo)
typedef struct Sala {
    char nome[50];        // nome do cômodo
    char pista[100];      // pista associada à sala
    struct Sala* esquerda; // ponteiro para sala à esquerda
    struct Sala* direita;  // ponteiro para sala à direita
} Sala;

// Nó da BST de pistas coletadas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Nó da tabela hash: cada pista aponta para um suspeito
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* proximo;
} HashNode;

// ==========================
// FUNÇÕES AUXILIARES
// ==========================

// Criar uma sala dinamicamente
Sala* criarSala(char nome[], char pista[]) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Inserir uma pista na BST de forma ordenada
PistaNode* inserirPista(PistaNode* raiz, char pista[]) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// Exibir todas as pistas em ordem alfabética (in-order traversal)
void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// Função hash simples para strings
int hashFunction(char pista[]) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++)
        soma += pista[i];
    return soma % HASH_SIZE;
}

// Inserir uma pista na tabela hash associando ao suspeito
void inserirNaHash(HashNode* tabela[], char pista[], char suspeito[]) {
    int idx = hashFunction(pista);
    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabela[idx];
    tabela[idx] = novo;
}

// Procurar o suspeito associado a uma pista
char* encontrarSuspeito(HashNode* tabela[], char pista[]) {
    int idx = hashFunction(pista);
    HashNode* atual = tabela[idx];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->proximo;
    }
    return NULL; // não encontrado
}

// ==========================
// FUNÇÃO PRINCIPAL DE EXPLORAÇÃO
// ==========================

// Navega pela mansão, coleta pistas e armazena na BST
void explorarSalas(Sala* atual, PistaNode** arvorePistas, HashNode* tabelaHash[]) {
    if (atual == NULL) return;

    char escolha;
    printf("\nVocê está na sala: %s\n", atual->nome);
    if (strlen(atual->pista) > 0) {
        printf("Pista encontrada: %s\n", atual->pista);
        *arvorePistas = inserirPista(*arvorePistas, atual->pista);
    }

    printf("Escolha o próximo caminho (e = esquerda, d = direita, s = sair): ");
    scanf(" %c", &escolha);

    if (escolha == 'e')
        explorarSalas(atual->esquerda, arvorePistas, tabelaHash);
    else if (escolha == 'd')
        explorarSalas(atual->direita, arvorePistas, tabelaHash);
    else
        return; // sair da exploração
}

// Verifica se o suspeito acusado possui pelo menos duas pistas
void verificarSuspeitoFinal(PistaNode* raiz, HashNode* tabela[], char suspeitoAcusado[]) {
    if (raiz == NULL) return;
    static int contador = 0;
    char* suspeito = encontrarSuspeito(tabela, raiz->pista);
    if (suspeito != NULL && strcmp(suspeito, suspeitoAcusado) == 0)
        contador++;

    verificarSuspeitoFinal(raiz->esquerda, tabela, suspeitoAcusado);
    verificarSuspeitoFinal(raiz->direita, tabela, suspeitoAcusado);

    // No retorno final da pilha de chamadas, verificar contagem
    if (raiz->pista[0] == '\0') return; // apenas um truque para não imprimir várias vezes
    if (raiz->pista != NULL) {
        if (contador >= 2)
            printf("\nAcusação correta! O suspeito '%s' possui evidências suficientes (%d pistas).\n", suspeitoAcusado, contador);
        else
            printf("\nAcusação incorreta! O suspeito '%s' possui apenas %d pista(s).\n", suspeitoAcusado, contador);
        contador = 0; // reset para próxima acusação
    }
}

// ==========================
// FUNÇÃO MAIN
// ==========================
int main() {
    // Criar o mapa da mansão
    Sala* hall = criarSala("Hall de Entrada", "Pegadas sujas");
    Sala* cozinha = criarSala("Cozinha", "Faca com impressões");
    Sala* salaEstar = criarSala("Sala de Estar", "Vidro quebrado");
    Sala* escritorio = criarSala("Escritório", "Carta rasgada");
    Sala* quarto = criarSala("Quarto", "Chave do cofre");

    // Montar conexões da árvore (binária)
    hall->esquerda = cozinha;
    hall->direita = salaEstar;
    cozinha->esquerda = escritorio;
    cozinha->direita = quarto;

    // Inicializar árvore de pistas
    PistaNode* arvorePistas = NULL;

    // Inicializar tabela hash
    HashNode* tabelaHash[HASH_SIZE] = {NULL};

    // Inserir pistas e suspeitos na hash
    inserirNaHash(tabelaHash, "Pegadas sujas", "Suspeito A");
    inserirNaHash(tabelaHash, "Faca com impressões", "Suspeito B");
    inserirNaHash(tabelaHash, "Vidro quebrado", "Suspeito A");
    inserirNaHash(tabelaHash, "Carta rasgada", "Suspeito C");
    inserirNaHash(tabelaHash, "Chave do cofre", "Suspeito B");

    printf("=== Bem-vindo ao Detective Quest ===\n");
    explorarSalas(hall, &arvorePistas, tabelaHash);

    printf("\n--- Pistas coletadas em ordem alfabética ---\n");
    exibirPistas(arvorePistas);

    char suspeitoAcusado[50];
    printf("\nDigite o suspeito que deseja acusar: ");
    scanf("%s", suspeitoAcusado);

    verificarSuspeitoFinal(arvorePistas, tabelaHash, suspeitoAcusado);

    return 0;
}




