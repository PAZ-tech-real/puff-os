/*
 * UART (PL011) Driver for Raspberry Pi 5
 * Supports serial console output for debugging
 */

#include "../include/uart.h"
#include "../include/types.h"

/* PL011 UART Register Offsets */
#define UART_DR         0x00    /* Data Register */
#define UART_RSRECR     0x04    /* Receive Status / Error Clear */
#define UART_FR         0x18    /* Flag Register */
#define UART_ILPR       0x20    /* IrDA Low-Power Counter */
#define UART_IBRD       0x24    /* Integer Baud Rate Divisor */
#define UART_FBRD       0x28    /* Fractional Baud Rate Divisor */
#define UART_LCRH       0x2C    /* Line Control Register */
#define UART_CR         0x30    /* Control Register */
#define UART_IFLS       0x34    /* Interrupt FIFO Level Select */
#define UART_IMSC       0x38    /* Interrupt Mask Set/Clear */
#define UART_RIS        0x3C    /* Raw Interrupt Status */
#define UART_MIS        0x40    /* Masked Interrupt Status */
#define UART_ICR        0x44    /* Interrupt Clear Register */

/* Flag Register bits */
#define UART_FR_TXFF    0x20    /* Transmit FIFO Full */
#define UART_FR_RXFE    0x10    /* Receive FIFO Empty */
#define UART_FR_BUSY    0x08    /* UART Busy */

/* Control Register bits */
#define UART_CR_UARTEN  0x01    /* UART Enable */
#define UART_CR_TXE     0x100   /* Transmit Enable */
#define UART_CR_RXE     0x200   /* Receive Enable */

/* Line Control Register bits */
#define UART_LCRH_FEN   0x10    /* Enable FIFOs */
#define UART_LCRH_WLEN  0x60    /* Word length: 8 bits = 0x60 */

static volatile u64 uart_base = 0;

/**
 * uart_init - Initialize UART peripheral
 */
void uart_init(u64 base) {
    uart_base = base;
    
    /* Disable UART during setup */
    *(volatile u32*)(uart_base + UART_CR) = 0;
    
    /* Clear pending interrupts */
    *(volatile u32*)(uart_base + UART_ICR) = 0x7FF;
    
    /* Set baud rate for 115200 @ 48MHz clock
     * IBRD = 48000000 / (16 * 115200) = 26
     * FBRD = remainder * 64 / 115200
     */
    *(volatile u32*)(uart_base + UART_IBRD) = 26;
    *(volatile u32*)(uart_base + UART_FBRD) = 3;
    
    /* Set line control: 8N1, enable FIFOs */
    *(volatile u32*)(uart_base + UART_LCRH) = (UART_LCRH_WLEN | UART_LCRH_FEN);
    
    /* Enable UART, RX, TX */
    *(volatile u32*)(uart_base + UART_CR) = (UART_CR_UARTEN | UART_CR_RXE | UART_CR_TXE);
}

/**
 * uart_putc - Write single character
 */
void uart_putc(char c) {
    if (uart_base == 0) return;
    
    /* Wait while FIFO is full */
    while (*(volatile u32*)(uart_base + UART_FR) & UART_FR_TXFF) {
        asm volatile("nop");
    }
    
    /* Write character */
    *(volatile u32*)(uart_base + UART_DR) = (u32)c;
    
    /* Convert \n to \r\n */
    if (c == '\n') {
        while (*(volatile u32*)(uart_base + UART_FR) & UART_FR_TXFF) {
            asm volatile("nop");
        }
        *(volatile u32*)(uart_base + UART_DR) = '\r';
    }
}

/**
 * uart_puts - Write null-terminated string
 */
void uart_puts(const char *str) {
    if (!str) return;
    
    while (*str) {
        uart_putc(*str++);
    }
}

/**
 * uart_getc - Read single character (blocking)
 */
char uart_getc(void) {
    if (uart_base == 0) return 0;
    
    /* Wait while FIFO is empty */
    while (*(volatile u32*)(uart_base + UART_FR) & UART_FR_RXFE) {
        asm volatile("nop");
    }
    
    /* Read character */
    return (char)(*(volatile u32*)(uart_base + UART_DR) & 0xFF);
}

/**
 * uart_printf - Simple formatted output (no floating point)
 * Supports: %d (int), %x (hex), %s (string), %c (char), %%
 */
void uart_printf(const char *fmt, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    
    while (*fmt) {
        if (*fmt == '%' && *(fmt + 1)) {
            fmt++;
            switch (*fmt) {
                case 'd': {
                    int val = __builtin_va_arg(args, int);
                    if (val < 0) {
                        uart_putc('-');
                        val = -val;
                    }
                    char buf[16];
                    int i = 0;
                    if (val == 0) buf[i++] = '0';
                    else {
                        while (val > 0) {
                            buf[i++] = '0' + (val % 10);
                            val /= 10;
                        }
                    }
                    while (i > 0) uart_putc(buf[--i]);
                    break;
                }
                case 'x': {
                    unsigned int val = __builtin_va_arg(args, unsigned int);
                    uart_puts("0x");
                    char buf[16];
                    int i = 0;
                    if (val == 0) buf[i++] = '0';
                    else {
                        while (val > 0) {
                            int digit = val % 16;
                            buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
                            val /= 16;
                        }
                    }
                    while (i > 0) uart_putc(buf[--i]);
                    break;
                }
                case 's': {
                    const char *str = __builtin_va_arg(args, const char*);
                    uart_puts(str ? str : "(null)");
                    break;
                }
                case 'c': {
                    char c = (char)__builtin_va_arg(args, int);
                    uart_putc(c);
                    break;
                }
                case '%':
                    uart_putc('%');
                    break;
                default:
                    uart_putc(*fmt);
                    break;
            }
        } else if (*fmt == '\\' && *(fmt + 1) == 'n') {
            uart_putc('\n');
            fmt++;
        } else {
            uart_putc(*fmt);
        }
        fmt++;
    }
    
    __builtin_va_end(args);
}
