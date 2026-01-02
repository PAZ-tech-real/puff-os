/*
 * Example: Button Input
 * Read button state on GPIO pin 27
 */

#include "gpio.h"
#include "uart.h"

#define BUTTON_PIN 27       /* GPIO27 */

void button_example(void) {
    uart_puts("Button example on GPIO27\n");
    uart_puts("Press button to see event\n");
    
    /* Configure button pin as input with pull-up */
    gpio_set_mode(BUTTON_PIN, GPIO_INPUT);
    gpio_set_pullmode(BUTTON_PIN, GPIO_PULL_UP);
    
    u32 prev_state = 1;
    
    /* Poll button state */
    while (1) {
        u32 state = gpio_read(BUTTON_PIN);
        
        if (state != prev_state) {
            if (state == 0) {
                uart_puts("Button: PRESSED\n");
            } else {
                uart_puts("Button: RELEASED\n");
            }
            prev_state = state;
        }
    }
}
