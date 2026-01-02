/*
 * Raspberry Pi 5 Bootloader - Second Stage
 * Initializes hardware and prepares for kernel execution
 */

#include "../include/types.h"
#include "../include/uart.h"
#include "../include/memory.h"

#define Pi5_BASE 0xfc000000
#define UART0_BASE (Pi5_BASE + 0x00201000)

/* Raspberry Pi 5 MMIO base addresses */
#define GPIO_BASE (Pi5_BASE + 0x00200000)
#define UART_BASE (Pi5_BASE + 0x00201000)
#define TIMER_BASE (Pi5_BASE + 0x00003000)

/**
 * bootloader_main - Entry point for bootloader
 * Initializes UART, memory, and prepares kernel environment
 */
void bootloader_main(void) {
    /* Initialize UART for early debug output */
    uart_init(UART_BASE);
    uart_puts("Raspberry Pi 5 Bootloader Starting\n");

    /* Initialize memory management */
    memory_init();
    uart_puts("Memory initialized\n");

    /* Enable floating point unit */
    enable_fpu();
    uart_puts("FPU enabled\n");

    /* Print system info */
    uart_puts("System ready for kernel launch\n");
    
    /* Jump to kernel */
    kernel_entry();
}

/**
 * enable_fpu - Enable floating point unit (NEON)
 */
void enable_fpu(void) {
    u64 cpacr;
    
    /* Read CPACR_EL1 */
    asm volatile("mrs %0, cpacr_el1" : "=r" (cpacr));
    
    /* Enable FP (bits 20-21) and SIMD (bits 22-23) */
    cpacr |= (3 << 20) | (3 << 22);
    
    /* Write back */
    asm volatile("msr cpacr_el1, %0" : : "r" (cpacr));
    asm volatile("isb");
}

/**
 * kernel_entry - Jump to kernel main
 */
extern void kernel_main(void);

void kernel_entry(void) {
    uart_puts("Jumping to kernel...\n");
    kernel_main();
    
    /* Hang if kernel returns */
    while(1) {
        asm volatile("wfe");
    }
}
