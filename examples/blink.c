/*
 * Example: LED Blinker
 * Toggles LED on GPIO pin 17 (ACT LED on Pi5)
 * 
 * Build: aarch64-linux-gnu-gcc -nostdinc -fno-builtin -march=armv8-a -Iinclude -c examples/blink.c
 */

#include "gpio.h"
#include "uart.h"

#define LED_PIN 17          /* GPIO17 - ACT LED on Pi5 */
#define BLINK_DELAY 1000000 /* Iterations for delay */

void delay_ms(u32 ms) {
    volatile u32 count = 0;
    for (u32 i = 0; i < ms * 1000; i++) {
        count++;
    }
}

void blink_example(void) {
    uart_puts("Starting LED blink example on GPIO17\n");
    
    /* Configure LED pin as output */
    gpio_set_mode(LED_PIN, GPIO_OUTPUT);
    
    /* Blink forever */
    while (1) {
        gpio_set(LED_PIN);      /* LED on */
        uart_puts("LED: ON\n");
        delay_ms(500);
        
        gpio_clear(LED_PIN);    /* LED off */
        uart_puts("LED: OFF\n");
        delay_ms(500);
    }
}
