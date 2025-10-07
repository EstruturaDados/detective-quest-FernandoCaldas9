#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================
// ESTRUTURA DE DADOS
// ============================

// Struct que representa uma sala da mansão
typedef struct Sala {
    char nome[50];           // Nome da sala
    struct Sala* esquerda;   // Ponteiro para a sala à esquerda
    struct Sala* direita;    // Ponteiro para a sala à direita
} Sala;

// ============================
// FUNÇÕES
// ============================

// Função que cria dinamicamente uma nova sala
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro de alocacao!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Função que permite ao jogador explorar as salas a partir de um nó da árvore
void explorarSalas(Sala* atual) {
    if (!atual) return;  // Se a sala não existir, retorna

    printf("\nVoce esta na sala: %s\n", atual->nome);

    // Se a sala não tem caminhos à esquerda nem à direita, é folha
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
                explorarSalas(atual->esquerda);
            else
                printf("Nao ha sala a esquerda!\n");
            break;
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita)
                explorarSalas(atual->direita);
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
    // Criando manualmente o mapa da mansao (árvore binária)
    Sala* hall = criarSala("Hall de Entrada");
    Sala* sala1 = criarSala("Sala de Estar");
    Sala* sala2 = criarSala("Cozinha");
    Sala* sala3 = criarSala("Biblioteca");
    Sala* sala4 = criarSala("Jardim");
    Sala* sala5 = criarSala("Quarto Secreto");
    Sala* sala6 = criarSala("Sala do Tesouro");

    // Montando conexões (esquerda/direita)
    hall->esquerda = sala1;
    hall->direita = sala2;

    sala1->esquerda = sala3;
    sala1->direita = sala4;

    sala2->esquerda = sala5;
    sala2->direita = sala6;

    // Inicia exploração a partir do hall de entrada
    printf("Bem-vindo ao Detective Quest!\n");
    explorarSalas(hall);

    // Liberando memória alocada
    free(sala6);
    free(sala5);
    free(sala4);
    free(sala3);
    free(sala2);
    free(sala1);
    free(hall);

    return 0;
}


