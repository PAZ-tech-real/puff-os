# Getting Started Guide

## Quick Start (5 minutes)

### 1. Install Tools
```bash
sudo apt-get update
sudo apt-get install -y gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu
```

### 2. Clone and Build
```bash
cd /workspaces/os
./dev build
```

### 3. Flash to SD Card
```bash
./dev flash /dev/sdb
```

Reboot Raspberry Pi 5 and watch the kernel boot!

## Detailed Guide

### Prerequisites

**Minimum:**
- Ubuntu/Debian Linux (20.04+)
- ARM64 cross-compiler
- Text editor or IDE
- 2GB free disk space

**Recommended:**
- Ubuntu 22.04 LTS or later
- 4GB+ RAM on development machine
- USB-UART adapter for serial debugging
- Raspberry Pi 5 with power supply

### Installation Steps

#### Step 1: Install ARM64 Cross-Compiler

On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install -y \
    gcc-aarch64-linux-gnu \
    binutils-aarch64-linux-gnu \
    gdb-multiarch \
    qemu-system-arm
```

Verify:
```bash
$ aarch64-linux-gnu-gcc --version
aarch64-linux-gnu-gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
```

#### Step 2: Clone Repository

```bash
git clone <repo-url>
cd os
```

#### Step 3: Make Scripts Executable

```bash
chmod +x scripts/build.sh scripts/clean.sh dev
```

### Building the OS

#### Standard Build

```bash
./dev build
```

Output:
```
[BUILD] Raspberry Pi 5 OS Compiler
[OK] Cross-compiler found

[COMPILE] Bootloader (boot.s)
[OK] boot.o
[COMPILE] Bootloader (bootloader.c)
[OK] bootloader.o
...
[SUCCESS] Build complete!

Output files:
  ELF:   build/kernel.elf
  Image: build/kernel.img
```

#### Rebuild from Scratch

```bash
./dev rebuild
```

This cleans all build artifacts and rebuilds.

### Preparing SD Card

#### Identify Your SD Card

```bash
# Insert SD card and run:
lsblk

# Look for your card (e.g., /dev/sdb, /dev/sdc)
# WARNING: Double-check before flashing!
```

#### Flash Kernel

**Method 1: Using dev script (recommended)**
```bash
./dev flash /dev/sdb
```

**Method 2: Manual flash**
```bash
# Unmount any mounted partitions
sudo umount /dev/sdb*

# Flash image
sudo dd if=build/kernel.img of=/dev/sdb bs=4M status=progress
sync
```

**Method 3: Using Balena Etcher (GUI)**
1. Download Balena Etcher
2. Select build/kernel.img
3. Select SD card
4. Click Flash

### Booting the OS

1. Insert SD card into Pi5
2. Connect USB power
3. Pi5 should boot and display output on UART

### Serial Console Setup (Optional)

For debugging output, connect USB-UART adapter:

#### Wiring
```
USB-UART → Pi5
GND      → Pin 6 (GND)
TX       → Pin 10 (GPIO15/RX)
RX       → Pin 8 (GPIO14/TX)
```

#### Connect Terminal
```bash
# At 115200 baud
screen /dev/ttyUSB0 115200

# Or with minicom
minicom -b 115200 -D /dev/ttyUSB0

# Or with picocom
picocom -b 115200 /dev/ttyUSB0
```

You should see:
```
=====================================
Raspberry Pi 5 Custom OS
=====================================

[INIT] Initializing interrupt controller...
[INIT] Initializing memory manager...
[INIT] Initializing scheduler...
[INIT] Initializing device drivers...

[BOOT] Kernel initialization complete!
[BOOT] System ready for user processes
```

## Development Workflow

### Typical Workflow

1. Make changes to source files
2. Build: `./dev build`
3. Check for errors
4. Flash to SD card: `./dev flash /dev/sdb`
5. Boot Pi5 and observe output
6. Use serial console for debugging

### Useful Commands

```bash
# View kernel disassembly
./dev dump

# Get kernel information
./dev info

# Clean build artifacts
./dev clean

# Get help
./dev help
```

### Examining the Kernel

```bash
# See all symbols
aarch64-linux-gnu-nm build/kernel.elf | grep kernel

# See sections
aarch64-linux-gnu-readelf -l build/kernel.elf

# Disassemble specific function
aarch64-linux-gnu-objdump -t build/kernel.elf | grep kernel_main
aarch64-linux-gnu-objdump -d build/kernel.elf | grep -A 20 kernel_main
```

## Common Issues & Solutions

### Issue: "command not found: aarch64-linux-gnu-gcc"
**Solution:** Install cross-compiler
```bash
sudo apt-get install gcc-aarch64-linux-gnu
```

### Issue: "Build fails - undefined reference to kernel_main"
**Solution:** Check that kernel/main.c exists and has no syntax errors
```bash
./dev rebuild
```

### Issue: "kernel.img write failed"
**Solution:** 
1. Verify device path: `lsblk`
2. Unmount card: `sudo umount /dev/sdb*`
3. Check permissions: `sudo dd if=build/kernel.img of=/dev/sdb ...`

### Issue: "No output on serial console"
**Solution:**
1. Verify cable connections (TX/RX crossed?)
2. Check baud rate (115200)
3. Try different USB port
4. Ensure kernel compiled successfully: `./dev info`

### Issue: "Kernel hangs immediately"
**Solution:**
1. Check bootloader initialization
2. Verify UART_BASE address matches Pi5 hardware
3. Enable debugging output in uart.c

## Next Steps

1. **Explore the code:**
   - Read kernel/main.c for boot sequence
   - Check drivers/uart.c for UART implementation
   - Review include/types.h for kernel types

2. **Add features:**
   - Implement GPIO driver (see examples/gpio.c)
   - Add file system support
   - Implement proper memory management

3. **Run examples:**
   - See examples/README.md for LED blink, button input, etc.

4. **Performance tuning:**
   - Profile scheduler
   - Optimize memory allocator
   - Cache optimization

## Useful Resources

- [Raspberry Pi 5 Datasheet](https://datasheets.raspberrypi.com/rpi5/raspberry-pi-5-datasheet.pdf)
- [ARM64 Quick Reference](https://developer.arm.com/documentation/den0018/latest/)
- [PL011 UART Spec](https://developer.arm.com/documentation/ddi0183/latest/)
- [Generic Interrupt Controller](https://developer.arm.com/documentation/ihi0069/latest/)
- [AArch64 Instruction Set](https://developer.arm.com/documentation/ddi0596/latest/)

## Getting Help

- Check README.md for full documentation
- Review code comments for explanations
- Search GitHub issues for similar problems
- Post on Raspberry Pi forums

Happy coding! 🚀
