/*
 * UART (Serial) Driver Header
 * Provides serial communication for debugging and console I/O
 */

#ifndef __UART_H__
#define __UART_H__

#include "types.h"

/**
 * uart_init - Initialize UART for serial communication
 * @base: Base address of UART peripheral
 */
void uart_init(u64 base);

/**
 * uart_putc - Write a single character
 * @c: Character to write
 */
void uart_putc(char c);

/**
 * uart_puts - Write a null-terminated string
 * @str: String to write
 */
void uart_puts(const char *str);

/**
 * uart_getc - Read a single character (blocking)
 * Returns: Character read
 */
char uart_getc(void);

/**
 * uart_printf - Formatted output to UART
 * @fmt: Format string
 * @...: Variable arguments
 */
void uart_printf(const char *fmt, ...);

#endif /* __UART_H__ */
