/**
 * schedule_rr.c
 * Implementação do algoritmo Round Robin
 * Baseado no padrão dos arquivos fcfs e sjf.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

#define QUANTUM 10

int tid = 1;                 // contador global de ids 
struct node *head = NULL;    // início da lista de tarefas

// Adiciona uma nova tarefa à lista
void add(char name[], int priority, int burst) {
    Task *t = malloc(sizeof(Task));
    t->name = strdup(name);
    t->priority = priority;
    t->burst = burst;
    t->tid = __sync_fetch_and_add(&tid, 1);
    insert(&head, t);
}

// Escalonador Round Robin
void schedule() {
    struct node *temp = head;
    struct node *fila = NULL;

    // Inverter a lista para manter ordem de chegada 
    while (temp != NULL) {
        struct node *prox = temp->next;
        temp->next = fila;
        fila = temp;
        temp = prox;
    }

    int tempoGlobal = 0;
    float TT_turnaround = 0, TT_espera = 0, TT_resposta = 0;

    // Marca se já rodou (para calcular tempo de resposta)
    int vistos[100] = {0};

    while (fila != NULL) {
        struct node *atual = fila;
        struct node *prev = NULL;

        while (atual != NULL) {
            Task *t = atual->task;
            int slice = (t->burst > QUANTUM) ? QUANTUM : t->burst;

            // Se é a primeira vez, acumula tempo de resposta
            if (!vistos[t->tid]) {
                TT_resposta += tempoGlobal;
                vistos[t->tid] = 1;
            }

            run(t, slice);
            tempoGlobal += slice;
            t->burst -= slice;

            if (t->burst <= 0) {
                // Terminou: turnaround = tempoGlobal
                TT_turnaround += tempoGlobal;
                TT_espera += tempoGlobal - slice;

                // remover nó
                if (prev)
                    prev->next = atual->next;
                else
                    fila = atual->next;

                free(t->name);
                free(t);
                struct node *tmp = atual;
                atual = atual->next;
                free(tmp);
            } else {
                prev = atual;
                atual = atual->next;
            }
        }
    }

    printf("\nMédia Turnaround: %.3f", TT_turnaround / (tid - 1));
    printf("\nMédia Espera: %.3f", TT_espera / (tid - 1));
    printf("\nMédia Resposta: %.3f\n", TT_resposta / (tid - 1));
}
