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
    }   // sendo temp a cabeça da lista invertida, que é a ordem de qual chegou primeiro
    

        int count=0, tempoGlobal=0;
        int Temp_espera, Temp_resposta, Temp_TurnAr;
        float TT_espera=0, TT_resposta=0, TT_TurnAr=0;

    while(temp != NULL){ //passa por toda a lista até terminar
        run(temp->task, temp->task->burst);/*roda a função run do dowload do livro que printa a task que ta rodando e as informações da task, simulando uma cpu*/
        
        Temp_espera = tempoGlobal;
        Temp_resposta = tempoGlobal;
        Temp_TurnAr = tempoGlobal + temp->task->burst;  //calcula e armazena o tempo de espera, resposta e turnaround da task que rodou

        TT_espera = TT_espera + Temp_espera;
        TT_resposta = TT_resposta + Temp_resposta;
        TT_TurnAr = TT_TurnAr + Temp_TurnAr;        //Adiciona os tempos de resposta, espera e turnaround a soma total

        tempoGlobal = tempoGlobal + temp->task->burst;  //atualiza o tempo global, que seria o tempo real simulado, com base na task que acabou de rodas
        count++; //soma ao contador de tasks, pra tirar a média depois

        delete(&temp, temp->task); //deleta a task que acabou de rodar
    }

    printf("\nMedia tempo de turnaround: %.3f", (TT_TurnAr/count));     //printa todas as médias dos tempos de espera, turnaround e resposta
    printf("\nMedia tempo de espera: %.3f", (TT_espera/count));
    printf("\nMedia tempo de resposta: %.3f\n", (TT_resposta/count));
}

/* Calcular a média do tempo de turnaround, espera e resposta pra todos os algorítimos
turnaround vai ser sempre o tempo de conclusão do processo, porque todos os procesos chegam ao mesmo tempo, como o livro pediu, então TAT=t.conclusão - t.chegada(0)
tempo para começar + burst

tempo de espera vai ser o (tempo de conclusão - tempo de burst), porque todos chegam ao mesmo tempo. o tempo total que ele esperou para ser completamente executado
tempo de completar(começar+burst) - burst -> tempos de começar no caso de fcfs e sjf

tempo de resposta é o tempo de espera até ser executado pela primeira vez
tempo para começar
    no caso de fcfs o tempo de resposta e de espera é o mesmo, mas em round robin é diferente
*/