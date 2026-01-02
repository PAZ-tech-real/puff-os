/*
 * Process Scheduler Implementation
 * Round-robin scheduling with priority levels
 */

#include "../include/process.h"
#include "../include/uart.h"
#include "../include/memory.h"

#define MAX_PROCESSES 256

static process_t processes[MAX_PROCESSES];
static process_t *ready_queue = NULL;
static process_t *current_process = NULL;
static u32 next_pid = 1;

/**
 * scheduler_init - Initialize scheduler
 */
void scheduler_init(void) {
    /* Initialize process table */
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].pid = 0;
        processes[i].state = PROC_TERMINATED;
        processes[i].next = NULL;
    }
    
    uart_puts("Scheduler initialized\n");
}

/**
 * schedule - Run scheduler to find next process
 */
void schedule(void) {
    /* Simple round-robin scheduler */
    if (current_process != NULL && current_process->next != NULL) {
        current_process = current_process->next;
    } else if (ready_queue != NULL) {
        current_process = ready_queue;
    }
    
    /* TODO: Context switch to current_process */
}

/**
 * create_process - Create new process
 */
u32 create_process(void (*entry_point)(void), u32 priority) {
    /* Find free slot */
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == 0) {
            processes[i].pid = next_pid++;
            processes[i].state = PROC_READY;
            processes[i].priority = priority;
            processes[i].program_counter = (u64)entry_point;
            processes[i].stack_pointer = (u64)malloc(4096) + 4096;  /* Stack grows down */
            
            /* Add to ready queue */
            if (ready_queue == NULL) {
                ready_queue = &processes[i];
            } else {
                process_t *p = ready_queue;
                while (p->next != NULL) p = p->next;
                p->next = &processes[i];
            }
            
            return processes[i].pid;
        }
    }
    
    return 0;  /* Failed */
}

/**
 * kill_process - Terminate process
 */
void kill_process(u32 pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == pid) {
            processes[i].state = PROC_TERMINATED;
            processes[i].pid = 0;
            /* TODO: Free process resources */
            break;
        }
    }
}

/**
 * yield - Yield CPU to scheduler
 */
void yield(void) {
    schedule();
    /* TODO: Context switch */
}
