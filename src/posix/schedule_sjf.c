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

    while(head != NULL){
        struct node *temp = head;
        struct node *minNode = head;
        /*Cria um novo nó e armazena a task do arquivo, um por um. armazenando um temporario e um minimo
        Para comparar e decidir qual o com menor tempo*/

        while(temp != NULL){
            if(temp->task->burst < minNode->task->burst){//compara o tempo de cpu do atual que leu(temp) com o menor armazenado(min)
                minNode = temp;                          //E se o atual for menor, armazena o novo menor
            }
            temp = temp->next;// no fim ele passa para o próximo nó para encadear 
        }
    
    run(minNode->task, minNode->task->burst);/*roda a função run do dowload do livro que printa a task que ta rodando e as informações 
                                                da task, simulando uma cpu*/
    delete(&head, minNode->task);
        // e por fim deleta a task que acabou de rodar da lista
    }
}