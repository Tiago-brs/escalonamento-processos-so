# escalonamento-processos-so
Implementação de algoritmos de escalonamento de processos em C, capitulo 5 do livro(FCFS, SJF, Prioridade, Round Robin, Prioridade com Round Robin).

Usamos, como o livro pediu, o download da base do projeto, que tinha o makefile e outras coisas importantes para simular a cpu e usar como código, o README dentro da pasta posix é o README do livro que veio com a pasta de download, assim como o makefile e os códigos que não são os código dos algoritimos.

Projeto com os arquivos de download do capitulo 5 do livro, cada algoritimo está em um arquivo .c diferente, 
(schedule_sjf.c, schedule_fcfs.c, schedule_priority.c, schedule_priority_rr.c, schedule_rr.c).

O programa roda na linha de comando do terminal linux, um algoritimo de cada vez separadamente.

Instruções de uso:
Para compilar com o makefile, chama o comando make com o nome do algoritimo, com os tais comandos:
make rr 
make fcfs 
make sjf 
make priority 
make priority_rr 

E para executar os algoritimos, executa um por um com o comando do linux ./ com o nome do algoritimo (do mesmo modo que foi usado para compilar) e o arquivo txt com a lista de tarefas depois.
Por exemplo:  ./fcfs schedule.txt

O arquivo .txt da lista  de tarefas tem o seguinte padrão, com apenas uma tarefa por linha:
<nome da tarefa>, <prioridade>, <tempo de burst>

Ao chamar cada algoritimo ele mostra a sequencia de execução e os tempos que cada tarefa passou de cada vez, e no final mostra o tempo médio de turnaround, espera e resposta que o algoritimo teve com aquela lista de tarefas.

Autores:
Algoritimos round-robin e round-robin com prioridade feito por Tiago Brito
Algoritimos SJF e FCFS feitos por Thiago César
Algoritimo prioridade feito por Ben Taurins
