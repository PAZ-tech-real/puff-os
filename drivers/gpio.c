/*
 * GPIO Driver for Raspberry Pi 5
 * Supports GPIO pin control via registers
 */

#include "../include/gpio.h"
#include "../include/uart.h"

/* Raspberry Pi 5 GPIO Base Address */
#define GPIO_BASE 0xfc200000

/* GPIO Register Offsets */
#define GPFSEL0     0x00    /* GPIO Function Select 0 */
#define GPFSEL1     0x04    /* GPIO Function Select 1 */
#define GPFSEL2     0x08    /* GPIO Function Select 2 */
#define GPFSEL3     0x0c    /* GPIO Function Select 3 */
#define GPFSEL4     0x10    /* GPIO Function Select 4 */
#define GPFSEL5     0x14    /* GPIO Function Select 5 */

#define GPSET0      0x1c    /* GPIO Set 0 */
#define GPSET1      0x20    /* GPIO Set 1 */

#define GPCLR0      0x28    /* GPIO Clear 0 */
#define GPCLR1      0x2c    /* GPIO Clear 1 */

#define GPLEV0      0x34    /* GPIO Level 0 */
#define GPLEV1      0x38    /* GPIO Level 1 */

#define GPEDS0      0x40    /* GPIO Event Detect Status 0 */
#define GPEDS1      0x44    /* GPIO Event Detect Status 1 */

#define GPREN0      0x4c    /* GPIO Rising Edge Detect Enable 0 */
#define GPREN1      0x50    /* GPIO Rising Edge Detect Enable 1 */

#define GPFEN0      0x58    /* GPIO Falling Edge Detect Enable 0 */
#define GPFEN1      0x5c    /* GPIO Falling Edge Detect Enable 1 */

#define GPPUD       0x94    /* GPIO Pull-up/down Enable */
#define GPPUDCLK0   0x98    /* GPIO Pull-up/down Clock 0 */
#define GPPUDCLK1   0x9c    /* GPIO Pull-up/down Clock 1 */

/**
 * gpio_init - Initialize GPIO controller
 */
void gpio_init(void) {
    /* TODO: Initialize GPIO peripheral */
    uart_puts("GPIO controller initialized\n");
}

/**
 * gpio_set_mode - Set GPIO pin mode
 */
void gpio_set_mode(u32 pin, gpio_mode_t mode) {
    if (pin >= 28) return;  /* Only 28 GPIO pins on Pi5 */
    
    /* Calculate register offset and bit position */
    u32 reg_offset = (pin / 10) * 4;
    u32 bit = (pin % 10) * 3;
    
    volatile u32 *fsel = (volatile u32*)(GPIO_BASE + reg_offset);
    
    /* Clear existing mode bits and set new mode */
    *fsel = (*fsel & ~(0x7 << bit)) | ((mode & 0x7) << bit);
}

/**
 * gpio_set_pullmode - Set pull-up/down
 */
void gpio_set_pullmode(u32 pin, gpio_pull_t pull) {
    if (pin >= 28) return;
    
    /* Write pull mode to GPPUD register */
    volatile u32 *gppud = (volatile u32*)(GPIO_BASE + GPPUD);
    *gppud = pull & 0x3;
    
    /* Clock enable for 150 cycles */
    for (int i = 0; i < 150; i++) {
        asm volatile("nop");
    }
    
    /* Enable clock for pin */
    u32 clk_reg = (pin < 32) ? GPPUDCLK0 : GPPUDCLK1;
    u32 clk_pin = pin % 32;
    volatile u32 *gppudclk = (volatile u32*)(GPIO_BASE + clk_reg);
    *gppudclk = (1 << clk_pin);
    
    /* Clock off */
    for (int i = 0; i < 150; i++) {
        asm volatile("nop");
    }
    *gppudclk = 0;
    *gppud = 0;
}

/**
 * gpio_write - Write GPIO pin state
 */
void gpio_write(u32 pin, u32 value) {
    if (pin >= 28) return;
    
    if (value) {
        gpio_set(pin);
    } else {
        gpio_clear(pin);
    }
}

/**
 * gpio_read - Read GPIO pin state
 */
u32 gpio_read(u32 pin) {
    if (pin >= 28) return 0;
    
    u32 reg_offset = (pin < 32) ? GPLEV0 : GPLEV1;
    u32 pin_bit = pin % 32;
    
    volatile u32 *gplev = (volatile u32*)(GPIO_BASE + reg_offset);
    return (*gplev >> pin_bit) & 0x1;
}

/**
 * gpio_set - Set GPIO pin high
 */
void gpio_set(u32 pin) {
    if (pin >= 28) return;
    
    u32 reg_offset = (pin < 32) ? GPSET0 : GPSET1;
    u32 pin_bit = pin % 32;
    
    volatile u32 *gpset = (volatile u32*)(GPIO_BASE + reg_offset);
    *gpset = (1 << pin_bit);
}

/**
 * gpio_clear - Set GPIO pin low
 */
void gpio_clear(u32 pin) {
    if (pin >= 28) return;
    
    u32 reg_offset = (pin < 32) ? GPCLR0 : GPCLR1;
    u32 pin_bit = pin % 32;
    
    volatile u32 *gpclr = (volatile u32*)(GPIO_BASE + reg_offset);
    *gpclr = (1 << pin_bit);
}

/**
 * gpio_toggle - Toggle GPIO pin
 */
void gpio_toggle(u32 pin) {
    gpio_write(pin, !gpio_read(pin));
}
