# Examples for Raspberry Pi 5 OS

This directory contains example programs demonstrating OS features.

## Available Examples

### 1. LED Blinker (blink.c)
Toggles GPIO17 (ACT LED) on and off.
- Demonstrates GPIO output control
- Shows basic timing/delay loops
- Example 500ms on/off cycle

### 2. Button Input (button.c)
Reads GPIO27 button state.
- Demonstrates GPIO input reading
- Shows pull-up configuration
- Implements simple debouncing

### 3. HDMI Display (hdmi.c)
Draws test pattern and text on HDMI display.
- Demonstrates HDMI framebuffer
- Shows graphics drawing (rectangles, lines)
- Text rendering support
- Color test patterns

## Building Examples

To compile an example:

```bash
aarch64-linux-gnu-gcc \
    -Wall -Wextra \
    -nostdinc -fno-builtin \
    -march=armv8-a \
    -Iinclude \
    -c examples/hdmi.c -o build/hdmi.o
```

Then link with kernel:

```bash
aarch64-linux-gnu-ld \
    -T linker.ld \
    -o build/kernel-hdmi.elf \
    build/boot.o \
    build/bootloader.o \
    build/kernel.o \
    build/uart.o \
    build/memory.o \
    build/interrupt.o \
    build/scheduler.o \
    build/gpio.o \
    build/hdmi.o \
    build/hdmi.o
```

## Running on Hardware

1. Build kernel: `./dev build`
2. Flash to SD card: `./dev flash /dev/sdb`
3. Power on Pi5
4. View output on HDMI display
5. See serial console output (optional)

## GPIO Pin Reference (Raspberry Pi 5)

| Pin | Function | Notes |
|-----|----------|-------|
| GPIO17 | ACT LED | Activity indicator |
| GPIO27 | Status LED | Power indicator |
| GPIO4 | I2C SDA | I2C interface |
| GPIO5 | I2C SCL | I2C interface |
| GPIO14 | UART TX | Serial output |
| GPIO15 | UART RX | Serial input |
| GPIO23 | CLK | Clock output |
| GPIO24 | CLK | Clock output |
| GPIO25 | CLK | Clock output |

## HDMI Display Modes

Supported resolutions:
- 640x480 (VGA)
- 800x600 (SVGA)
- 1024x768 (XGA)
- 1280x1024 (SXGA)
- 1920x1080 (Full HD) - Default
- 3840x2160 (4K UHD)

Supported color formats:
- RGB565 (16-bit)
- RGB888 (24-bit)
- RGBA8888 (32-bit, default)

## HDMI API Example

```c
// Initialize display
hdmi_init();

// Clear screen to black
hdmi_clear(0x00000000);

// Draw a rectangle
hdmi_draw_rectangle(100, 100, 200, 200, 0xFF0000FF);  // Red

// Draw text
hdmi_put_string(50, 50, "Hello Pi5!", 0xFFFFFFFF, 0x000000FF);

// Set individual pixel
hdmi_set_pixel(960, 540, 0xFFFFFFFF);  // White pixel at center
```

## Color Format (RGBA8888)

Colors are specified as 32-bit values: `0xRRGGBBAA`

Examples:
- `0xFF0000FF` = Red
- `0x00FF00FF` = Green
- `0x0000FFFF` = Blue
- `0xFFFFFFFF` = White
- `0x000000FF` = Black (with alpha)
- `0x00000000` = Transparent black

## Advanced Examples (TODO)

- [ ] ADC conversion
- [ ] I2C communication
- [ ] SPI interface
- [ ] Interrupt-driven button
- [ ] Timer/PWM control
- [ ] Serial shell
- [ ] File system access
- [ ] HDMI animation
- [ ] Font rendering

