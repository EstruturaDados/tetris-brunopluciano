#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

/* =====================================
   ESTRUTURA DA PEÇA
===================================== */
typedef struct {
    char nome;
    int id;
} Peca;

/* =====================================
   FILA CIRCULAR
===================================== */
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

/* =====================================
   PILHA
===================================== */
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

/* =====================================
   FUNÇÃO GERAR PEÇA
===================================== */
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;

    nova.nome = tipos[rand() % 4];
    nova.id = id;

    return nova;
}

/* =====================================
   FILA
===================================== */
void inicializarFila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->quantidade = 0;
}

int filaCheia(Fila *fila) {
    return fila->quantidade == TAM_FILA;
}

int filaVazia(Fila *fila) {
    return fila->quantidade == 0;
}

void enqueue(Fila *fila, Peca peca) {
    if (filaCheia(fila))
        return;

    fila->itens[fila->fim] = peca;
    fila->fim = (fila->fim + 1) % TAM_FILA;
    fila->quantidade++;
}

Peca dequeue(Fila *fila) {
    Peca removida = fila->itens[fila->inicio];
    fila->inicio = (fila->inicio + 1) % TAM_FILA;
    fila->quantidade--;
    return removida;
}

/* Retorna posição lógica da fila */
int posicaoFila(Fila *fila, int indice) {
    return (fila->inicio + indice) % TAM_FILA;
}

/* =====================================
   PILHA
===================================== */
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == TAM_PILHA - 1;
}

void push(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha))
        return;

    pilha->topo++;
    pilha->itens[pilha->topo] = peca;
}

Peca pop(Pilha *pilha) {
    Peca removida = pilha->itens[pilha->topo];
    pilha->topo--;
    return removida;
}

/* =====================================
   EXIBIÇÃO
===================================== */
void exibirFila(Fila *fila) {
    printf("Fila de peças:        ");

    for (int i = 0; i < fila->quantidade; i++) {
        int pos = posicaoFila(fila, i);
        printf("[%c %d] ", fila->itens[pos].nome, fila->itens[pos].id);
    }

    printf("\n");
}

void exibirPilha(Pilha *pilha) {
    printf("Pilha de reserva:     ");

    if (pilhaVazia(pilha)) {
        printf("[vazia]");
    } else {
        printf("(Topo -> Base): ");
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->itens[i].nome, pilha->itens[i].id);
        }
    }

    printf("\n");
}

void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\n====================================\n");
    printf("ESTADO ATUAL\n");
    printf("====================================\n");

    exibirFila(fila);
    exibirPilha(pilha);

    printf("====================================\n");
}

/* =====================================
   MANTER FILA CHEIA
===================================== */
void reporFila(Fila *fila, int *proximoID) {
    while (!filaCheia(fila)) {
        enqueue(fila, gerarPeca(*proximoID));
        (*proximoID)++;
    }
}

/* =====================================
   TROCA PEÇA ATUAL
===================================== */
void trocarAtual(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("\nTroca impossível.\n");
        return;
    }

    int posFrente = fila->inicio;

    Peca temp = fila->itens[posFrente];
    fila->itens[posFrente] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temp;

    printf("\nTroca da peça atual realizada!\n");
}

/* =====================================
   TROCA MÚLTIPLA
===================================== */
void trocaMultipla(Fila *fila, Pilha *pilha) {
    if (fila->quantidade < 3 || pilha->topo < 2) {
        printf("\nTroca múltipla impossível.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int posFila = posicaoFila(fila, i);
        int posPilha = pilha->topo - i;

        Peca temp = fila->itens[posFila];
        fila->itens[posFila] = pilha->itens[posPilha];
        pilha->itens[posPilha] = temp;
    }

    printf("\nTroca realizada entre os 3 primeiros da fila e as 3 peças da pilha.\n");
}

/* =====================================
   MAIN
===================================== */
int main() {
    Fila fila;
    Pilha pilha;

    int opcao;
    int proximoID = 0;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    /* Preenche fila inicial */
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca(proximoID));
        proximoID++;
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("\nMENU DE AÇÕES\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Reservar peça (fila -> pilha)\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar peça atual com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1: {
                Peca usada = dequeue(&fila);
                printf("\nPeça jogada: [%c %d]\n", usada.nome, usada.id);
                reporFila(&fila, &proximoID);
                break;
            }

            case 2: {
                if (pilhaCheia(&pilha)) {
                    printf("\nPilha cheia!\n");
                } else {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);

                    printf("\nPeça reservada: [%c %d]\n",
                           reservada.nome, reservada.id);

                    reporFila(&fila, &proximoID);
                }
                break;
            }

            case 3: {
                if (pilhaVazia(&pilha)) {
                    printf("\nNenhuma peça reservada.\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("\nPeça usada da reserva: [%c %d]\n",
                           usada.nome, usada.id);
                }
                break;
            }

            case 4:
                trocarAtual(&fila, &pilha);
                break;

            case 5:
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando programa...\n");
                break;

            default:
                printf("\nOpção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}