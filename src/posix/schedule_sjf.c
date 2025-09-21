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

        int count=0, tempoGlobal=0;
        int Temp_espera, Temp_resposta, Temp_TurnAr;
        float TT_espera=0, TT_resposta=0, TT_TurnAr=0;

    while(head != NULL){
        struct node *temp = head;
        struct node *minNode = head;
        /*Cria um novo nó e armazena a task do arquivo, um por um. armazenando um temporario e um minimo
        Para comparar e decidir qual o com menor tempo*/

        while(temp != NULL){
            if(temp->task->burst < minNode->task->burst){ //vê o nó que foi pego como cabeça temporaria e compara un por um para ver se é o menor da lista, até a lista acabar
                minNode = temp;          
            }
            temp = temp->next; // no fim ele define o temporario para o próximo nó da lista
        }
    
    run(minNode->task, minNode->task->burst);/*roda a função run do dowload do livro que printa a task que ta rodando e as informações 
                                                da task, simulando uma cpu. rodando a menor task que encontrou no momento*/
    
    Temp_espera = tempoGlobal;
    Temp_resposta = tempoGlobal;
    Temp_TurnAr = tempoGlobal + minNode->task->burst;

    TT_espera = TT_espera + Temp_espera;
    TT_resposta = TT_resposta + Temp_resposta;
    TT_TurnAr = TT_TurnAr + Temp_TurnAr;

    tempoGlobal = tempoGlobal + minNode->task->burst;        
    count++;
    
    delete(&head, minNode->task);
        // e por fim deleta a task menor que acabou de rodar da lista, e volta o while para a próxima task da lista e compara tudo de novo
    }

    printf("\nMedia tempo de turnaround: %.3f", (TT_TurnAr/count));
    printf("\nMedia tempo de espera: %.3f", (TT_espera/count));
    printf("\nMedia tempo de resposta: %.3f\n", (TT_resposta/count));
}