/*
 * Interrupt Handling Header
 * Manages CPU exceptions and external interrupts
 */

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "types.h"

/* Interrupt handler typedef */
typedef void (*irq_handler_t)(u32 irq_num);

/**
 * interrupt_init - Initialize interrupt controller
 */
void interrupt_init(void);

/**
 * register_irq_handler - Register handler for IRQ
 * @irq: IRQ number
 * @handler: Callback function
 */
void register_irq_handler(u32 irq, irq_handler_t handler);

/**
 * handle_exception - Handle CPU exception
 * @exc_type: Exception type
 * @esr: Exception Syndrome Register value
 * @elr: Exception Link Register value
 */
void handle_exception(u32 exc_type, u64 esr, u64 elr);

/**
 * enable_irq - Enable specific IRQ
 * @irq: IRQ number
 */
void enable_irq(u32 irq);

/**
 * disable_irq - Disable specific IRQ
 * @irq: IRQ number
 */
void disable_irq(u32 irq);

#endif /* __INTERRUPT_H__ */
