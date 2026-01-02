/*
 * Process Management Header
 * Task scheduling and context switching
 */

#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "types.h"

/* Process states */
typedef enum {
    PROC_RUNNING,
    PROC_READY,
    PROC_BLOCKED,
    PROC_TERMINATED
} process_state_t;

/* Process control block */
typedef struct process {
    u32 pid;                    /* Process ID */
    process_state_t state;      /* Current state */
    u64 stack_pointer;          /* Stack pointer */
    u64 program_counter;        /* Program counter */
    u32 priority;               /* Priority level */
    struct process *next;       /* Next in queue */
} process_t;

/**
 * scheduler_init - Initialize process scheduler
 */
void scheduler_init(void);

/**
 * schedule - Run scheduler to switch processes
 */
void schedule(void);

/**
 * create_process - Create a new process
 * @entry_point: Entry function
 * @priority: Process priority
 * Returns: Process ID
 */
u32 create_process(void (*entry_point)(void), u32 priority);

/**
 * kill_process - Terminate a process
 * @pid: Process ID
 */
void kill_process(u32 pid);

/**
 * yield - Yield CPU to scheduler
 */
void yield(void);

#endif /* __PROCESS_H__ */
