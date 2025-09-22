
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "cpu.h"

static struct node *head = NULL;
static int tid_counter = 1;

void add(char *name, int priority, int burst) {
    Task *t = malloc(sizeof(Task));
    if (!t) { perror("malloc"); exit(1); }
    t->name = strdup(name);
    t->tid = tid_counter++;
    t->priority = priority;
    t->burst = burst;
    insert(&head, t);
}

static struct node* find_best(struct node *h) {
    if (!h) return NULL;
    struct node *best = h;
    for (struct node *p = h; p != NULL; p = p->next) {
        int bp = best->task->priority;
        int bb = best->task->burst;
        int cp = p->task->priority;
        int cb = p->task->burst;
        if (cp > bp || (cp == bp && cb < bb)) {
            best = p;
        }
    }
    return best;
}

void schedule(void) {
    while (head != NULL) {
        struct node *chosen = find_best(head);
        Task *t = chosen->task;
        run(t, t->burst);
        delete(&head, t);
        free(t->name);
        free(t);
    }
}
