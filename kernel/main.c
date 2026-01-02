/*
 * Raspberry Pi 5 OS Kernel
 * Main kernel entry point and core functionality
 */

#include "../include/types.h"
#include "../include/uart.h"
#include "../include/memory.h"
#include "../include/interrupt.h"
#include "../include/process.h"
#include "../include/hdmi.h"

#define UART0_BASE 0xfc201000

/**
 * kernel_main - Kernel entry point
 * Called from bootloader after initial setup
 */
void kernel_main(void) {
    /* Initialize UART for kernel output */
    uart_init(UART0_BASE);
    
    uart_puts("\n");
    uart_puts("=====================================\n");
    uart_puts("Raspberry Pi 5 Custom OS\n");
    uart_puts("=====================================\n\n");
    
    /* Initialize core subsystems */
    uart_puts("[INIT] Initializing interrupt controller...\n");
    interrupt_init();
    
    uart_puts("[INIT] Initializing memory manager...\n");
    memory_allocator_init();
    
    uart_puts("[INIT] Initializing scheduler...\n");
    scheduler_init();
    
    uart_puts("[INIT] Initializing device drivers...\n");
    drivers_init();
    
    uart_puts("[INIT] Initializing HDMI display...\n");
    if (hdmi_init() == 0) {
        uart_puts("[HDMI] Display initialized successfully\n");
        
        /* Draw welcome screen */
        hdmi_clear(0x000000FF);  /* Blue background */
        hdmi_draw_rectangle(100, 100, 1720, 200, 0xFFFFFFFF);  /* White box */
        hdmi_put_string(150, 150, "Raspberry Pi 5 OS", 0x000000FF, 0xFFFFFFFF);
    } else {
        uart_puts("[HDMI] Display initialization failed\n");
    }
    
    uart_puts("\n[BOOT] Kernel initialization complete!\n");
    uart_puts("[BOOT] System ready for user processes\n\n");
    
    /* Enable interrupts */
    enable_interrupts();
    
    /* Start idle loop */
    kernel_idle_loop();
}

/**
 * kernel_idle_loop - Main kernel loop
 * Processes interrupts and runs scheduler
 */
void kernel_idle_loop(void) {
    while(1) {
        /* Process any pending interrupts */
        check_interrupts();
        
        /* Run scheduler to switch to next process */
        schedule();
        
        /* Low power wait for interrupt */
        asm volatile("wfe");
    }
}

/**
 * enable_interrupts - Enable CPU interrupts (IRQ and FIQ)
 */
void enable_interrupts(void) {
    asm volatile(
        "msr daifclr, #0x3\n"  /* Clear PSTATE.I and PSTATE.F bits */
        "isb"                   /* Instruction synchronization barrier */
    );
}

/**
 * check_interrupts - Check for pending interrupts
 */
void check_interrupts(void) {
    /* TODO: Read GIC (Generic Interrupt Controller) status */
    /* Handle pending interrupts */
}

/**
 * panic - Kernel panic handler
 */
void panic(const char* message) {
    uart_puts("\n!!! KERNEL PANIC !!!\n");
    uart_puts("Message: ");
    uart_puts(message);
    uart_puts("\n");
    
    /* Hang system */
    while(1) {
        asm volatile("wfe");
    }
}
