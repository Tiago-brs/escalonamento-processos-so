#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

int tid = 1;
struct node *head = NULL;


void add(char nome[],int prioridade,int burst){
//printf("Teste função add \n");

    Task *t = malloc(sizeof(Task));
    t->name = strdup(nome);
    t->priority = prioridade;
    t->burst = burst;
    t->tid = __sync_fetch_and_add(&tid, 1); // incrementar tid sem causar condição de corrida como o livro falou

    insert(&head, t);   // coloca as tarefas na lista de nós(de list.c e list.h do dowload do livro)
}

void schedule(){
//printf("Teste função schedule em sjf\n");
struct node* temp = NULL;
struct node* atual = head;
struct node* prox = NULL;

    while(atual != NULL){   //função pra inverter a ordem da lista, porque a implementação de add do dowload do livro deixa a lista com a cabeça sendo o ultimo item colocado(como uma pilha)
        prox = atual->next;
        atual->next = temp;
        temp = atual;
        atual = prox;
    }
    // sendo temp a cabeça da lista invertida, que é a ordem de qual chegou primeiro

    while(temp != NULL){//passa por toda a lista até terminar
        run(temp->task, temp->task->burst);/*roda a função run do dowload do livro que printa a task que ta rodando e as informações da task, simulando uma cpu*/
        delete(&temp, temp->task); //deleta a task que acabou de rodar
    }
}