/* 
 * ARM64 Bootloader for Raspberry Pi 5
 * Initializes CPU, memory, and jumps to kernel entry point
 */

.section ".text.boot"
.global _start

_start:
    /* Set stack pointer for boot CPU (CPU 0) */
    ldr x0, =stack_end
    mov sp, x0

    /* Disable interrupts */
    msr daifset, #0xf

    /* Clear the BSS section */
    ldr x0, =bss_start
    ldr x1, =bss_end
    sub x1, x1, x0
    mov x2, #0
    bl memset_simple

    /* Initialize MMU and memory management */
    bl mmu_init

    /* Call kernel main */
    bl kernel_main

    /* Hang if kernel returns */
    b hang

hang:
    wfe
    b hang

/* Simple memset for BSS clearing */
memset_simple:
    cmp x1, #0
    beq memset_done
    str xzr, [x0], #8
    sub x1, x1, #8
    cmp x1, #0
    bne memset_simple
memset_done:
    ret

/* MMU initialization stub */
mmu_init:
    /* TODO: Implement MMU setup for kernel space mapping */
    ret

.section ".bss"
.align 16
stack_bottom:
    .space 16384
stack_end:

.section ".data"
bss_start:
    .word 0
bss_end:
    .word 0
