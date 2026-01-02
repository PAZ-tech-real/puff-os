/*
 * Example: HDMI Display Test
 * Draws shapes and text on HDMI display
 */

#include "hdmi.h"
#include "uart.h"

void hdmi_example(void) {
    uart_puts("Starting HDMI display example\n");
    
    /* Initialize display */
    if (hdmi_init() != 0) {
        uart_puts("ERROR: Failed to initialize HDMI\n");
        return;
    }
    
    /* Clear to black */
    hdmi_clear(0x00000000);
    
    /* Draw colorful test pattern */
    
    /* Red rectangle (top-left) */
    hdmi_draw_rectangle(50, 50, 400, 300, 0xFF0000FF);  /* RGBA: Red */
    
    /* Green rectangle (top-right) */
    hdmi_draw_rectangle(1470, 50, 400, 300, 0x00FF00FF);  /* RGBA: Green */
    
    /* Blue rectangle (bottom-left) */
    hdmi_draw_rectangle(50, 730, 400, 300, 0x0000FFFF);  /* RGBA: Blue */
    
    /* Yellow rectangle (bottom-right) */
    hdmi_draw_rectangle(1470, 730, 400, 300, 0xFFFF00FF);  /* RGBA: Yellow */
    
    /* Draw white lines forming cross */
    hdmi_draw_line(960, 0, 960, 1080, 0xFFFFFFFF);      /* Vertical */
    hdmi_draw_line(0, 540, 1920, 540, 0xFFFFFFFF);      /* Horizontal */
    
    /* Draw text */
    hdmi_put_string(700, 450, "Raspberry Pi 5 OS", 0xFFFFFFFF, 0x000000FF);
    hdmi_put_string(800, 500, "HDMI Test", 0xFFFFFFFF, 0x000000FF);
    
    uart_puts("HDMI test pattern displayed\n");
}
