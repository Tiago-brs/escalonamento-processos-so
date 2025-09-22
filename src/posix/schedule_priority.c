/* schedule_priority.c
 *
 * Escalonador por PRIORIDADE (n�o-preemptivo).
 * Regra: prioridade maior = mais priorit�rio.
 * Desempate: menor burst primeiro.
 *
 * Se quiser inverter (prioridade MENOR = mais priorit�rio),
 * mude a compara��o dentro de find_best().
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"     // Task { name, tid, priority, burst }  :contentReference[oaicite:2]{index=2}
#include "list.h"     // insert(), delete(), struct node       :contentReference[oaicite:3]{index=3}
#include "cpu.h"      // run(Task*, slice)                     :contentReference[oaicite:4]{index=4}

/* A especifica��o pede que cada algoritmo exponha:
 *   void add(char *name, int priority, int burst);
 *   void schedule(void);
 * (driver.c chama add() para cada linha do arquivo e depois schedule())  :contentReference[oaicite:5]{index=5}
 */

/* Lista ligada das tarefas (utilit�rio j� fornecido no projeto) */
static struct node *head = NULL;

/* Gerador simples de TID */
static int tid_counter = 1;

/* Cria e insere uma tarefa na lista (ordem de inser��o n�o importa,
 * pois a sele��o � feita a cada passo pelo melhor candidato).        */
void add(char *name, int priority, int burst) {
    Task *t = malloc(sizeof(Task));
    if (!t) { perror("malloc"); exit(1); }
    t->name = strdup(name);   // delete() usa strcmp(name), ent�o manter nome �nico ajuda  :contentReference[oaicite:6]{index=6}
    t->tid = tid_counter++;
    t->priority = priority;
    t->burst = burst;
    insert(&head, t);         // insere no come�o da lista                :contentReference[oaicite:7]{index=7}
}

/* Encontra o n� com MAIOR prioridade; em empate, menor burst. */
static struct node* find_best(struct node *h) {
    if (!h) return NULL;
    struct node *best = h;
    for (struct node *p = h; p != NULL; p = p->next) {
        int bp = best->task->priority;
        int bb = best->task->burst;
        int cp = p->task->priority;
        int cb = p->task->burst;

        /* Crit�rio principal: prioridade MAIOR primeiro.
           Desempate: menor burst. */
        if (cp > bp || (cp == bp && cb < bb)) {
            best = p;
        }
    }
    return best;
}

/* La�o principal do escalonador:
 * enquanto houver tarefas, escolhe a de maior prioridade
 * e executa por todo o burst (n�o-preemptivo). */
void schedule(void) {
    while (head != NULL) {
        struct node *chosen = find_best(head);
        Task *t = chosen->task;

        /* Executa a tarefa de uma vez (burst inteiro) na CPU virtual */
        run(t, t->burst);     // imprime: nome, prioridade, burst, slice  :contentReference[oaicite:8]{index=8}

        /* Remove da lista e libera mem�ria */
        delete(&head, t);     // remo��o por nome                          :contentReference[oaicite:9]{index=9}
        free(t->name);
        free(t);
    }
}
