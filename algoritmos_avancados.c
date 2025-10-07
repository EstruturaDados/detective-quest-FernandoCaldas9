#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================
// ESTRUTURAS DE DADOS
// ============================

// Nó da árvore que representa cada cômodo da mansão
typedef struct Sala {
    char nome[50];           // Nome do cômodo
    char pista[100];         // Pista encontrada (pode estar vazia)
    struct Sala* esquerda;   // Ponteiro para sala à esquerda
    struct Sala* direita;    // Ponteiro para sala à direita
} Sala;

// Nó da BST de pistas coletadas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// ============================
// FUNÇÕES DE CRIAÇÃO
// ============================

// Cria dinamicamente uma sala da mansão com pista opcional
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro de alocacao!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Cria um nó de pista na BST
PistaNode* criarPistaNode(const char* pista) {
    PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (!novo) {
        printf("Erro de alocacao!\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// ============================
// FUNÇÕES BST DE PISTAS
// ============================

// Insere uma nova pista na BST em ordem alfabética
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (!raiz)
        return criarPistaNode(pista);
    
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    
    // Se for igual, não insere duplicata
    return raiz;
}

// Exibe as pistas em ordem alfabética (in-order traversal)
void exibirPistas(PistaNode* raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// ============================
// FUNÇÃO DE EXPLORAÇÃO
// ============================

// Explora a mansão, coletando pistas e permitindo navegação
void explorarSalasComPistas(Sala* atual, PistaNode** arvorePistas) {
    if (!atual) return;

    printf("\nVoce esta na sala: %s\n", atual->nome);

    // Coleta pista se existir
    if (strlen(atual->pista) > 0) {
        printf("Voce encontrou uma pista: %s\n", atual->pista);
        *arvorePistas = inserirPista(*arvorePistas, atual->pista);
    }

    // Verifica se há caminhos
    if (!atual->esquerda && !atual->direita) {
        printf("Nao ha mais caminhos a seguir.\n");
        return;
    }

    char escolha;
    do {
        printf("Escolha o caminho: esquerda (e), direita (d) ou sair (s): ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda)
                explorarSalasComPistas(atual->esquerda, arvorePistas);
            else
                printf("Nao ha sala a esquerda!\n");
            break;
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita)
                explorarSalasComPistas(atual->direita, arvorePistas);
            else
                printf("Nao ha sala a direita!\n");
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
// FUNÇÃO PRINCIPAL
// ============================

int main() {
    // Criando manualmente o mapa da mansão com pistas
    Sala* hall = criarSala("Hall de Entrada", "Chave dourada");
    Sala* sala1 = criarSala("Sala de Estar", "Carta suspeita");
    Sala* sala2 = criarSala("Cozinha", "Pegada de lama");
    Sala* sala3 = criarSala("Biblioteca", "Livro fora do lugar");
    Sala* sala4 = criarSala("Jardim", "Fio de cabelo");
    Sala* sala5 = criarSala("Quarto Secreto", "Diario antigo");
    Sala* sala6 = criarSala("Sala do Tesouro", "Mapa escondido");

    // Montando conexões (esquerda/direita)
    hall->esquerda = sala1;
    hall->direita = sala2;

    sala1->esquerda = sala3;
    sala1->direita = sala4;

    sala2->esquerda = sala5;
    sala2->direita = sala6;

    // Inicializa árvore de pistas vazia
    PistaNode* arvorePistas = NULL;

    printf("Bem-vindo ao Detective Quest com coleta de pistas!\n");
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibe todas as pistas coletadas em ordem alfabética
    printf("\n=== Pistas coletadas em ordem alfabetica ===\n");
    exibirPistas(arvorePistas);

    // Liberando memória das salas
    free(sala6);
    free(sala5);
    free(sala4);
    free(sala3);
    free(sala2);
    free(sala1);
    free(hall);

    // Obs: Para simplicidade, memória da BST de pistas não está sendo liberada
    // mas poderia ser liberada com uma função de pós-ordem recursiva.

    return 0;
}



