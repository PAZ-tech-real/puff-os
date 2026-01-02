/*
 * HDMI Display Driver Header
 * Support for HDMI framebuffer output on Raspberry Pi 5
 */

#ifndef __HDMI_H__
#define __HDMI_H__

#include "types.h"

/* HDMI resolution modes */
typedef enum {
    HDMI_640x480,      /* VGA */
    HDMI_800x600,      /* SVGA */
    HDMI_1024x768,     /* XGA */
    HDMI_1280x1024,    /* SXGA */
    HDMI_1920x1080,    /* Full HD */
    HDMI_3840x2160     /* 4K UHD */
} hdmi_mode_t;

/* Pixel formats */
typedef enum {
    HDMI_RGB565,       /* 16-bit RGB */
    HDMI_RGB888,       /* 24-bit RGB */
    HDMI_RGBA8888      /* 32-bit RGBA */
} hdmi_format_t;

/* HDMI display configuration */
typedef struct {
    hdmi_mode_t mode;
    hdmi_format_t format;
    u32 width;
    u32 height;
    u32 refresh_rate;  /* Hz */
} hdmi_config_t;

/**
 * hdmi_init - Initialize HDMI controller
 * Returns: 0 on success, -1 on failure
 */
int hdmi_init(void);

/**
 * hdmi_set_mode - Set display resolution
 * @config: Display configuration
 * Returns: 0 on success, -1 on failure
 */
int hdmi_set_mode(hdmi_config_t *config);

/**
 * hdmi_get_framebuffer - Get framebuffer memory address
 * Returns: Physical address of framebuffer
 */
u64 hdmi_get_framebuffer(void);

/**
 * hdmi_get_stride - Get scanline width in bytes
 * Returns: Stride value
 */
u32 hdmi_get_stride(void);

/**
 * hdmi_set_pixel - Write pixel to framebuffer
 * @x: X coordinate
 * @y: Y coordinate
 * @color: Color value (format dependent)
 */
void hdmi_set_pixel(u32 x, u32 y, u32 color);

/**
 * hdmi_get_pixel - Read pixel from framebuffer
 * @x: X coordinate
 * @y: Y coordinate
 * Returns: Color value
 */
u32 hdmi_get_pixel(u32 x, u32 y);

/**
 * hdmi_clear - Clear entire screen to color
 * @color: Color value
 */
void hdmi_clear(u32 color);

/**
 * hdmi_draw_rectangle - Draw filled rectangle
 * @x: Left coordinate
 * @y: Top coordinate
 * @width: Rectangle width
 * @height: Rectangle height
 * @color: Fill color
 */
void hdmi_draw_rectangle(u32 x, u32 y, u32 width, u32 height, u32 color);

/**
 * hdmi_draw_line - Draw line using Bresenham algorithm
 * @x1, @y1: Start coordinates
 * @x2, @y2: End coordinates
 * @color: Line color
 */
void hdmi_draw_line(u32 x1, u32 y1, u32 x2, u32 y2, u32 color);

/**
 * hdmi_put_char - Draw character at position
 * @x: X coordinate
 * @y: Y coordinate
 * @c: Character to draw
 * @color: Text color
 * @bg_color: Background color
 */
void hdmi_put_char(u32 x, u32 y, char c, u32 color, u32 bg_color);

/**
 * hdmi_put_string - Draw string starting at position
 * @x: X coordinate
 * @y: Y coordinate
 * @str: String to draw
 * @color: Text color
 * @bg_color: Background color
 */
void hdmi_put_string(u32 x, u32 y, const char *str, u32 color, u32 bg_color);

/**
 * hdmi_flush - Synchronize framebuffer changes (if needed)
 */
void hdmi_flush(void);

/**
 * hdmi_is_connected - Check if HDMI display is connected
 * Returns: 1 if connected, 0 if not
 */
int hdmi_is_connected(void);

/**
 * hdmi_get_edid - Get EDID data from display
 * @edid: Buffer for EDID data (256 bytes)
 * Returns: 0 on success, -1 on failure
 */
int hdmi_get_edid(u8 *edid);

#endif /* __HDMI_H__ */
