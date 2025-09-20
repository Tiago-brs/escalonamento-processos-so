/**
 * schedule_rr.c
 * Implementação do algoritmo Round Robin
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

#define QUANTUM 10   // tempo máximo por vez que um processo pode usar a CPU

extern struct node *head;   // lista de tarefas (definida no driver)
extern int tid;             // id global de tarefas

/**
 * Adiciona uma nova tarefa na lista
 */
void add(char *name, int priority, int burst) {
    Task *t = malloc(sizeof(Task));
    t->name = strdup(name);
    t->priority = priority;
    t->burst = burst;
    t->tid = __sync_fetch_and_add(&tid, 1);
    insert(&head, t);
}

/**
 * Função principal do escalonador Round Robin
 */
void schedule() {
    struct node *temp = head;
    int tempoGlobal = 0;
    int count = 0;

    float somaTurnaround = 0;
    float somaEspera = 0;
    float somaResposta = 0;

    // Guardar tempos de resposta para cada tarefa
    // (primeira vez que ela roda)
    while (temp) { count++; temp = temp->next; }
    temp = head;

    // Criar uma cópia da lista, pois vamos alterar bursts
    struct node *fila = NULL;
    while (temp) {
        Task *t = malloc(sizeof(Task));
        t->name = strdup(temp->task->name);
        t->priority = temp->task->priority;
        t->burst = temp->task->burst;
        t->tid = temp->task->tid;
        insert(&fila, t);
        temp = temp->next;
    }

    int *primeiraVez = calloc(count+1, sizeof(int)); // marca tempo de resposta

    while (fila) {
        struct node *atual = fila;
        struct node *prev = NULL;

        while (atual) {
            Task *t = atual->task;

            int slice = (t->burst > QUANTUM) ? QUANTUM : t->burst;

            // registra tempo de resposta se for a primeira execução
            if (!primeiraVez[t->tid]) {
                somaResposta += tempoGlobal;
                primeiraVez[t->tid] = 1;
            }

            run(t, slice);
            tempoGlobal += slice;
            t->burst -= slice;

            if (t->burst <= 0) {
                // terminou: calcula turnaround e espera
                somaTurnaround += tempoGlobal;
                somaEspera += tempoGlobal - slice;
                // remover da lista
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

    printf("\nMédia Turnaround: %.2f\n", somaTurnaround / count);
    printf("Média Espera: %.2f\n", somaEspera / count);
    printf("Média Resposta: %.2f\n", somaResposta / count);

    free(primeiraVez);
}
