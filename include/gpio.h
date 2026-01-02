/*
 * GPIO (General Purpose I/O) Driver Header
 * Supports LED control, button input, and GPIO manipulation
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#include "types.h"

/* GPIO modes */
typedef enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT = 1,
    GPIO_ALT0 = 4,
    GPIO_ALT1 = 5,
    GPIO_ALT2 = 6,
    GPIO_ALT3 = 7,
    GPIO_ALT4 = 3,
    GPIO_ALT5 = 2
} gpio_mode_t;

/* Pull-up/down modes */
typedef enum {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP = 1,
    GPIO_PULL_DOWN = 2
} gpio_pull_t;

/**
 * gpio_init - Initialize GPIO controller
 */
void gpio_init(void);

/**
 * gpio_set_mode - Set GPIO pin mode
 * @pin: GPIO pin number (0-27)
 * @mode: GPIO mode (input/output/alt)
 */
void gpio_set_mode(u32 pin, gpio_mode_t mode);

/**
 * gpio_set_pullmode - Set GPIO pull-up/down
 * @pin: GPIO pin number
 * @pull: Pull mode
 */
void gpio_set_pullmode(u32 pin, gpio_pull_t pull);

/**
 * gpio_write - Write GPIO pin state
 * @pin: GPIO pin number
 * @value: Pin state (0 or 1)
 */
void gpio_write(u32 pin, u32 value);

/**
 * gpio_read - Read GPIO pin state
 * @pin: GPIO pin number
 * Returns: Pin state (0 or 1)
 */
u32 gpio_read(u32 pin);

/**
 * gpio_set - Set GPIO pin high (1)
 * @pin: GPIO pin number
 */
void gpio_set(u32 pin);

/**
 * gpio_clear - Set GPIO pin low (0)
 * @pin: GPIO pin number
 */
void gpio_clear(u32 pin);

/**
 * gpio_toggle - Toggle GPIO pin state
 * @pin: GPIO pin number
 */
void gpio_toggle(u32 pin);

#endif /* __GPIO_H__ */
