/**
 * schedule_priority_rr.c
 * Escalonador com Prioridade + Round Robin
 * Maior prioridade executa primeiro;
 * Round Robin entre tarefas de mesma prioridade.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

#define QUANTUM 10

int tid = 1;
struct node *head = NULL;

// Adiciona uma nova tarefa à lista
void add(char name[], int priority, int burst) {
    Task *t = malloc(sizeof(Task));
    t->name = strdup(name);
    t->priority = priority;
    t->burst = burst;
    t->tid = __sync_fetch_and_add(&tid, 1);
    insert(&head, t);
}

// Função auxiliar: encontra a maior prioridade na lista
static int maiorPrioridade(struct node *lista) {
    int max = -9999;
    while (lista) {
        if (lista->task->priority > max) {
            max = lista->task->priority;
        }
        lista = lista->next;
    }
    return max;
}

// Escalonador Prioridade + Round Robin
void schedule() {
    struct node *fila = head;
    int tempoGlobal = 0;
    int count = 0;
    float TT_turnaround = 0, TT_espera = 0, TT_resposta = 0;
    int vistos[100] = {0};

    // Conta processos
    struct node *tmp = fila;
    while (tmp) { count++; tmp = tmp->next; }

    // Enquanto houver processos na fila
    while (fila) {
        // Maior prioridade atual
        int prioAtual = maiorPrioridade(fila);

        struct node *atual = fila;
        struct node *prev = NULL;

        // Percorre a lista executando apenas tarefas com prioridade == prioAtual
        while (atual) {
            Task *t = atual->task;

            if (t->priority == prioAtual) {
                int slice = (t->burst > QUANTUM) ? QUANTUM : t->burst;

                if (!vistos[t->tid]) {
                    TT_resposta += tempoGlobal;
                    vistos[t->tid] = 1;
                }

                run(t, slice);
                tempoGlobal += slice;
                t->burst -= slice;

                if (t->burst <= 0) {
                    TT_turnaround += tempoGlobal;
                    TT_espera += tempoGlobal - slice;

                    // remove nó
                    if (prev)
                        prev->next = atual->next;
                    else
                        fila = atual->next;

                    free(t->name);
                    free(t);
                    struct node *tmpN = atual;
                    atual = atual->next;
                    free(tmpN);
                    continue; // não avança prev
                }
            }
            prev = atual;
            atual = atual->next;
        }
    }

    printf("\nMédia Turnaround: %.3f", TT_turnaround / count);
    printf("\nMédia Espera: %.3f", TT_espera / count);
    printf("\nMédia Resposta: %.3f\n", TT_resposta / count);
}
