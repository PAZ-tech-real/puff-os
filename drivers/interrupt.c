/*
 * Interrupt Controller Driver
 * Manages GIC (Generic Interrupt Controller) for Raspberry Pi 5
 */

#include "../include/interrupt.h"
#include "../include/uart.h"

#define MAX_IRQS 1024

/* GIC base addresses for Raspberry Pi 5 */
#define GIC_DISTRIBUTOR_BASE  0xfc001000
#define GIC_CPU_INTERFACE_BASE 0xfc002000

static irq_handler_t irq_handlers[MAX_IRQS];

/**
 * interrupt_init - Initialize interrupt controller
 */
void interrupt_init(void) {
    /* Initialize handler table */
    for (int i = 0; i < MAX_IRQS; i++) {
        irq_handlers[i] = NULL;
    }
    
    /* TODO: Initialize GIC distributor */
    /* TODO: Initialize GIC CPU interface */
    
    uart_puts("Interrupt controller initialized\n");
}

/**
 * register_irq_handler - Register IRQ handler
 */
void register_irq_handler(u32 irq, irq_handler_t handler) {
    if (irq < MAX_IRQS) {
        irq_handlers[irq] = handler;
    }
}

/**
 * handle_exception - Handle CPU exception
 */
void handle_exception(u32 exc_type, u64 esr, u64 elr) {
    uart_puts("Exception occurred:\n");
    uart_printf("Type: 0x%x, ESR: 0x%llx, ELR: 0x%llx\n", exc_type, esr, elr);
    
    /* TODO: Implement exception handling */
}

/**
 * enable_irq - Enable IRQ
 */
void enable_irq(u32 irq) {
    if (irq >= MAX_IRQS) return;
    
    /* TODO: Set interrupt enable bit in GIC distributor */
    u32 reg_offset = (irq / 32) * 4;
    u32 bit = irq % 32;
    
    volatile u32 *gicd_isenabler = (volatile u32*)(GIC_DISTRIBUTOR_BASE + 0x100 + reg_offset);
    *gicd_isenabler |= (1 << bit);
}

/**
 * disable_irq - Disable IRQ
 */
void disable_irq(u32 irq) {
    if (irq >= MAX_IRQS) return;
    
    /* TODO: Clear interrupt enable bit in GIC distributor */
    u32 reg_offset = (irq / 32) * 4;
    u32 bit = irq % 32;
    
    volatile u32 *gicd_icenabler = (volatile u32*)(GIC_DISTRIBUTOR_BASE + 0x180 + reg_offset);
    *gicd_icenabler |= (1 << bit);
}
