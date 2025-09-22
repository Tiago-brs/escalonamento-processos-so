#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[64];
    int arrival;
    int burst;
    int priority; // menor = mais prioritário
    int start_time, finish_time, waiting_time, turnaround, response;
} Task;
typedef struct { Task* v; int n; int quantum_default; } TaskList;

int read_tasks(const char* path, TaskList* out);
void free_tasks(TaskList* tl);
void print_gantt(const char** labels, const int* starts, const int* ends, int m);
void print_metrics(Task* tasks, int n, int makespan, int cpu_busy);

int main(int argc, char** argv) {
    if (argc < 2) { fprintf(stderr, "Uso: %s schedule.txt\n", argv[0]); return 1; }
    TaskList tl; if (read_tasks(argv[1], &tl) != 0) return 1;

    int done = 0, time = 0, cpu_busy=0;
    int first_arrival = 1e9; for (int i=0;i<tl.n;i++) if (tl.v[i].arrival < first_arrival) first_arrival = tl.v[i].arrival;
    int* finished = (int*)calloc(tl.n, sizeof(int));

    const char* labels[2048]; int starts[2048]; int ends[2048]; int segs=0;

    printf("[PRIORITY] Ordem de execução:\n");
    while (done < tl.n) {
        int idx = -1, best_p = 1e9, best_arrival = 1e9;
        for (int i=0;i<tl.n;i++) {
            if (!finished[i] && tl.v[i].arrival <= time) {
                if (tl.v[i].priority < best_p ||
                    (tl.v[i].priority == best_p && tl.v[i].arrival < best_arrival)) {
                    best_p = tl.v[i].priority; best_arrival = tl.v[i].arrival; idx = i;
                }
            }
        }
        if (idx == -1) {
            int next_arrival = 1e9;
            for (int i=0;i<tl.n;i++) if (!finished[i] && tl.v[i].arrival < next_arrival) next_arrival = tl.v[i].arrival;
            time = next_arrival;
            continue;
        }

        tl.v[idx].start_time = time;
        tl.v[idx].waiting_time = tl.v[idx].start_time - tl.v[idx].arrival;
        time += tl.v[idx].burst;
        tl.v[idx].finish_time = time;
        tl.v[idx].turnaround = tl.v[idx].finish_time - tl.v[idx].arrival;
        tl.v[idx].response = tl.v[idx].start_time - tl.v[idx].arrival;
        cpu_busy += tl.v[idx].burst;
        finished[idx] = 1; done++;

        labels[segs] = tl.v[idx].name;
        starts[segs] = tl.v[idx].start_time;
        ends[segs] = tl.v[idx].finish_time;
        segs++;

        printf("  %s(p=%d): start=%d finish=%d wait=%d turnaround=%d\n",
            tl.v[idx].name, tl.v[idx].priority, tl.v[idx].start_time, tl.v[idx].finish_time,
            tl.v[idx].waiting_time, tl.v[idx].turnaround);
    }

    int makespan = time - (first_arrival < 0 ? 0 : first_arrival);
    print_gantt(labels, starts, ends, segs);
    print_metrics(tl.v, tl.n, makespan, cpu_busy);

    free(finished); free_tasks(&tl); return 0;
}