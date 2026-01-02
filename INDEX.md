# Raspberry Pi 5 OS - Documentation Index

Welcome! This is a complete custom operating system for Raspberry Pi 5. Here's where to find everything you need.

## 🚀 Start Here (5 minutes)

**[GETTING_STARTED.md](GETTING_STARTED.md)** - Build and boot the OS in 5 minutes
- Install tools
- Build kernel
- Flash to SD card
- Boot on Pi5

## 📖 Main Documentation

### Overview & Guides
- **[README.md](README.md)** - Complete overview, features, API reference
- **[GETTING_STARTED.md](GETTING_STARTED.md)** - Step-by-step setup guide
- **[IMPLEMENTATION_GUIDE.md](IMPLEMENTATION_GUIDE.md)** - What's included & summary

### Technical Details
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - System design, memory layout, boot flow
- **[STATUS.md](STATUS.md)** - Feature completeness matrix & roadmap
- **[STATS.md](STATS.md)** - Code metrics and project statistics

### Code Examples
- **[examples/README.md](examples/README.md)** - LED blink, button input examples

## 💻 Source Code Structure

### Bootloader & Kernel
```
bootloader/
├── boot.s              - ARM64 assembly entry point
└── bootloader.c        - C bootloader initialization

kernel/
└── main.c              - Kernel core and idle loop
```

### Device Drivers
```
drivers/
├── uart.c              - Serial console driver (PL011)
├── gpio.c              - GPIO pin control
├── memory.c            - Heap allocator & page management
├── interrupt.c         - Interrupt controller (GIC)
└── scheduler.c         - Process scheduler
```

### Headers & APIs
```
include/
├── types.h             - Type definitions
├── uart.h              - Serial console API
├── gpio.h              - GPIO control API
├── memory.h            - Memory management API
├── interrupt.h         - Interrupt handling API
└── process.h           - Process management API
```

### Build System
```
scripts/
├── build.sh            - Compile kernel
├── clean.sh            - Remove artifacts
└── dev                 - Development helper

linker.ld              - Memory layout & linking
Makefile.config        - Build configuration
```

## 🛠 Quick Commands

```bash
# Build the kernel
./dev build

# Clean build artifacts
./dev clean

# Rebuild from scratch
./dev rebuild

# Flash to SD card
./dev flash /dev/sdb

# View kernel info
./dev info

# View disassembly
./dev dump

# Help
./dev help
```

## 📚 Documentation Map

| Need | Document |
|------|----------|
| **Quick start** | [GETTING_STARTED.md](GETTING_STARTED.md) |
| **Build & install** | [README.md](README.md#installation) or [GETTING_STARTED.md](GETTING_STARTED.md) |
| **API reference** | [README.md](README.md#api-documentation) |
| **System design** | [ARCHITECTURE.md](ARCHITECTURE.md) |
| **Feature status** | [STATUS.md](STATUS.md) |
| **Code examples** | [examples/README.md](examples/README.md) |
| **Project stats** | [STATS.md](STATS.md) |

## 🎯 What This OS Includes

✅ **Complete Bootloader** - ARM64 assembly + C initialization
✅ **Functional Kernel** - Multi-tasking kernel core
✅ **Device Drivers** - UART, GPIO, memory, interrupts, scheduler
✅ **Build System** - One-command cross-compilation
✅ **Full Documentation** - 1,600+ lines of guides
✅ **Working Examples** - LED blink, button input

## 🚀 Getting Started Checklist

- [ ] Install cross-compiler: `sudo apt-get install gcc-aarch64-linux-gnu`
- [ ] Clone repository
- [ ] Make scripts executable: `chmod +x scripts/build.sh dev`
- [ ] Build kernel: `./dev build`
- [ ] Prepare SD card
- [ ] Flash kernel: `./dev flash /dev/sdb`
- [ ] Boot Pi5
- [ ] View UART output at 115200 baud
- [ ] Read ARCHITECTURE.md to understand design

## 🔍 Common Questions

### Q: How long to get started?
**A:** ~5 minutes with GETTING_STARTED.md

### Q: What hardware do I need?
**A:** Raspberry Pi 5, SD card, USB power, USB-UART adapter (optional)

### Q: How is the code organized?
**A:** See ARCHITECTURE.md for detailed structure

### Q: Can I extend it?
**A:** Yes! See STATUS.md for areas needing work

### Q: Is it production-ready?
**A:** No, it's an educational OS. See STATUS.md for limitations.

## 📖 Reading Guide

### For Beginners
1. Start: [GETTING_STARTED.md](GETTING_STARTED.md)
2. Learn: [ARCHITECTURE.md](ARCHITECTURE.md)
3. Explore: Source code with comments

### For Developers
1. Build: [README.md](README.md#building)
2. Understand: [ARCHITECTURE.md](ARCHITECTURE.md)
3. Extend: Check STATUS.md for TODOs
4. Examples: [examples/README.md](examples/README.md)

### For System Designers
1. Overview: [README.md](README.md)
2. Design: [ARCHITECTURE.md](ARCHITECTURE.md)
3. Status: [STATUS.md](STATUS.md)
4. Details: [STATS.md](STATS.md)

## 🎓 Learning Topics Covered

- ARM64 assembly programming
- Operating system fundamentals
- Embedded systems development
- Cross-platform compilation
- Hardware driver development
- Memory management
- Interrupt handling
- Process scheduling
- Build automation
- Technical documentation

## 📁 File Statistics

- **1,162 lines** of source code (C + ARM64 asm)
- **1,668 lines** of documentation
- **31 files** total
- **7 directories**
- **~2,830 lines** overall

## 🔗 Related Resources

### Hardware References
- [Raspberry Pi 5 Datasheet](https://datasheets.raspberrypi.com/rpi5/raspberry-pi-5-datasheet.pdf)
- [BCM2712 SoC Details](https://datasheets.raspberrypi.com/rpi5/raspberry-pi-5-datasheet.pdf)
- [ARM64 Architecture](https://developer.arm.com/documentation/den0024/latest/)

### Software References
- [PL011 UART Controller](https://developer.arm.com/documentation/ddi0183/latest/)
- [Generic Interrupt Controller (GIC)](https://developer.arm.com/documentation/ihi0069/latest/)
- [AArch64 Instruction Set](https://developer.arm.com/documentation/ddi0596/latest/)

### Learning Resources
- [OS in 3 Months](https://github.com/cfenollosa/os-tutorial)
- [Raspberry Pi Bare Metal](https://github.com/raspberrypi/tools)
- [ARM64 Tutorial](https://github.com/PiSugar/PiSugar/wiki)

## 🎯 Next Steps

1. **Install Tools:** `sudo apt-get install gcc-aarch64-linux-gnu`
2. **Read Guide:** [GETTING_STARTED.md](GETTING_STARTED.md)
3. **Build OS:** `./dev build`
4. **Flash SD:** `./dev flash /dev/sdb`
5. **Boot Pi5** and watch it run!
6. **Explore Code:** Check examples and drivers
7. **Extend OS:** See STATUS.md for ideas

## 💬 Support

- **Setup Help:** [GETTING_STARTED.md](GETTING_STARTED.md)
- **Code Questions:** See source comments
- **Design Details:** [ARCHITECTURE.md](ARCHITECTURE.md)
- **Build Issues:** Check README.md troubleshooting

## 📋 Checklist for Using This Project

- [ ] Read GETTING_STARTED.md
- [ ] Install ARM64 cross-compiler
- [ ] Build kernel with `./dev build`
- [ ] Prepare SD card with correct device
- [ ] Flash kernel with `./dev flash /dev/sdb`
- [ ] Boot Raspberry Pi 5
- [ ] Connect serial console (optional)
- [ ] Read ARCHITECTURE.md to understand design
- [ ] Explore source code
- [ ] Run examples
- [ ] Extend with new features

## 🏆 What You've Got

A **complete, bootable operating system** for Raspberry Pi 5 with:
- Clean architecture
- Well-documented code
- Build automation
- Working examples
- Comprehensive guides

Ready to boot, run, and extend! 🚀

---

**Last Updated:** January 2, 2026
**Status:** Ready for use
**License:** Educational (see LICENSE)

**Start with:** [GETTING_STARTED.md](GETTING_STARTED.md) ➜ **Main Docs:** [README.md](README.md) ➜ **Deep Dive:** [ARCHITECTURE.md](ARCHITECTURE.md)
