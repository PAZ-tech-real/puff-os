# Architecture and Design

## System Architecture

### Boot Flow

```
                    Power On
                      ↓
        ┌─────────────────────────────┐
        │  Raspberry Pi 5 Firmware    │
        │  (GPU Bootloader)           │
        │  - Initializes GPU          │
        │  - Loads ARM64 code         │
        └──────────────┬──────────────┘
                       ↓
        ┌─────────────────────────────┐
        │  ARM64 Bootloader           │
        │  (bootloader/boot.s)        │
        │  - Set up CPU               │
        │  - Initialize stack         │
        │  - Clear BSS section        │
        │  - Enable FPU               │
        └──────────────┬──────────────┘
                       ↓
        ┌─────────────────────────────┐
        │  Bootloader Main            │
        │  (bootloader/bootloader.c)  │
        │  - Initialize UART          │
        │  - Setup memory             │
        │  - Print debug info         │
        └──────────────┬──────────────┘
                       ↓
        ┌─────────────────────────────┐
        │  Kernel Main                │
        │  (kernel/main.c)            │
        │  - Init interrupts          │
        │  - Init memory manager      │
        │  - Init scheduler           │
        │  - Init drivers             │
        └──────────────┬──────────────┘
                       ↓
        ┌─────────────────────────────┐
        │  Kernel Idle Loop           │
        │  - Handle interrupts        │
        │  - Run scheduler            │
        │  - Wait for events          │
        └─────────────────────────────┘
```

## Memory Map

### Virtual Address Space (Kernel)

```
0xFFFFFFFFFFFFFFFF ┌────────────────┐
                   │   Unmapped     │
                   ├────────────────┤
0xFFFFFF8000000000 │   I/O Remap    │ (UART, GPIO, etc)
                   ├────────────────┤
0xFFFFFF0000000000 │   Kernel Heap  │ (Dynamic allocation)
                   ├────────────────┤
0xFFFF800000000000 │   Kernel Data  │ (BSS, .data sections)
                   ├────────────────┤
0xFFFF000000000000 │   Kernel Code  │ (.text sections)
                   │                │
0x0000000000000000 └────────────────┘ (User space - WIP)
```

### Physical Address Space

```
0x0000_0000 ┌─────────────────┐
            │  Bootloader     │ (0x00000 - 0x7FFFF)
            ├─────────────────┤
0x0008_0000 │  Kernel Code    │ (0x80000 - 0xFFFFF)
            ├─────────────────┤
0x0010_0000 │  Kernel Data    │ (dynamic)
            ├─────────────────┤
            │  Free/Heap      │
            │                 │
0xFC00_0000 │  GIC Controller │ (Generic Interrupt Controller)
            ├─────────────────┤
0xFC20_0000 │  GPIO           │
            ├─────────────────┤
0xFC20_1000 │  UART (PL011)   │
            ├─────────────────┤
            │  Other I/O      │
```

## Component Breakdown

### 1. Bootloader

**Files:** `bootloader/boot.s`, `bootloader/bootloader.c`

**Responsibilities:**
- CPU initialization
- Stack setup
- BSS clearing
- FPU enablement
- UART initialization
- Jump to kernel

**Key Functions:**
- `_start()` - Entry point
- `bootloader_main()` - Main initialization
- `enable_fpu()` - Enable NEON/FP

### 2. Kernel Core

**Files:** `kernel/main.c`

**Responsibilities:**
- Subsystem initialization
- Event loop management
- System configuration
- Panic handling

**Key Functions:**
- `kernel_main()` - Kernel entry
- `kernel_idle_loop()` - Main loop
- `enable_interrupts()` - Enable IRQ
- `panic()` - Fatal error

### 3. Drivers

#### UART Driver (drivers/uart.c)
- Serial communication (PL011 controller)
- Console output
- Debugging interface
- **Base:** 0xFC201000

#### Memory Driver (drivers/memory.c)
- Heap allocation
- Page allocation
- Memory statistics
- Physical page tracking

#### Interrupt Driver (drivers/interrupt.c)
- GIC (Generic Interrupt Controller) management
- IRQ registration
- Exception handling
- **Base:** 0xFC001000 (distributor)
- **Base:** 0xFC002000 (CPU interface)

#### Scheduler Driver (drivers/scheduler.c)
- Process management
- Round-robin scheduling
- Context switching (WIP)
- Process state tracking

#### GPIO Driver (drivers/gpio.c)
- GPIO pin control
- LED/button I/O
- Pull-up/down configuration
- **Base:** 0xFC200000

### 4. Include Headers

| Header | Purpose |
|--------|---------|
| types.h | Basic type definitions |
| uart.h | Serial console interface |
| memory.h | Memory management |
| interrupt.h | Interrupt handling |
| process.h | Process management |
| gpio.h | GPIO pin control |

## Compilation Pipeline

```
Source Files          Compilation        Linking             Final Output
────────────────      ──────────────      ───────────────     ──────────────

boot.s            ──→ as/gcc          ──→
                                       ├─→ ld ──→ kernel.elf ──→ objcopy
bootloader.c      ──→ gcc            ──→    (with linker.ld)     ──→
                                       ├─→
kernel/main.c     ──→ gcc            ──→
                                       ├─→ kernel.img
drivers/*.c       ──→ gcc            ──→    (raw binary)
                                       ├─→

linker.ld         (used by linker)
```

## Threading Model

### Current (WIP)
- Single CPU execution
- No true preemption
- Cooperative scheduling (yield-based)

### Planned
- Multi-core support (all 4 cores on Pi5)
- Preemptive scheduling
- Per-CPU schedulers
- Thread synchronization (mutex, semaphore)

## Interrupt Handling

### Current Flow
```
Hardware Interrupt
  ↓
Exception Handler (asm)
  ↓
handle_exception() (C)
  ↓
IRQ Handler Table
  ↓
Driver-specific Handler
```

### Supported Interrupts
- System timer
- UART interrupt (RX data ready)
- GPIO edge detection
- Custom driver interrupts

## Device Tree (TODO)

Currently hardware addresses are hardcoded. Future implementation will use device tree for:
- Hardware description
- MMIO address mapping
- Clock configuration
- Pin multiplexing

Example (future):
```dtb
/ {
    compatible = "brcm,bcm2712";
    
    uart@fc201000 {
        compatible = "arm,pl011";
        reg = <0xfc201000 0x1000>;
        interrupts = <125>;
    };
    
    gpio@fc200000 {
        compatible = "brcm,bcm2835-gpio";
        reg = <0xfc200000 0xb4>;
    };
};
```

## Performance Considerations

### Current
- Simple linear heap allocator
- No memory compression
- Basic scheduler (no priorities respected)
- Busy-wait delays

### Optimization Opportunities
- Buddy allocator (memory)
- Slab allocator (frequent sizes)
- Pre-emptive scheduling
- Timer-based delays
- Interrupt coalescing
- CPU idle states

## Build System

### Toolchain
- GCC aarch64-linux-gnu (cross-compiler)
- GNU LD (linker)
- GNU Binutils (objcopy, objdump, etc.)

### Configuration
- Makefile.config (global settings)
- Hardcoded #defines in headers
- Compile-time options in source

### Future Improvements
- Kbuild system (Linux-style)
- Menuconfig (configuration UI)
- Make targets for different configs

## Security Considerations

Current implementation has **NO** security features:
- No memory protection (MMU)
- No privilege separation
- No access control
- No code signing

Security improvements needed for production:
- Enable MMU with proper page tables
- User/kernel mode separation
- Stack canaries
- Address Space Layout Randomization (ASLR)

## Testing Strategy

Currently manual testing. Planned improvements:
- Unit tests for drivers
- Integration tests
- Boot sequence validation
- Memory leak detection
- Scheduler stress tests

## Documentation Map

| Document | Content |
|----------|---------|
| README.md | Overview and API docs |
| GETTING_STARTED.md | Setup and quick start |
| ARCHITECTURE.md | This file - design details |
| examples/README.md | Example usage |
| Code comments | Implementation details |

---

For questions about specific components, see the relevant source files with their detailed comments.
