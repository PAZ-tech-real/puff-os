#!/bin/bash
# Build script for Raspberry Pi 5 OS
# Compiles bootloader, kernel, and creates final image

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="${PROJECT_DIR}/build"
CROSS_COMPILE="${CROSS_COMPILE:-aarch64-linux-gnu-}"

cd "$PROJECT_DIR"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}[BUILD] Raspberry Pi 5 OS Compiler${NC}"
echo "Cross-compiler: ${CROSS_COMPILE}gcc"
echo "Build directory: ${BUILD_DIR}"
echo ""

# Create build directory
mkdir -p "$BUILD_DIR"

# Check for cross-compiler
if ! command -v "${CROSS_COMPILE}gcc" &> /dev/null; then
    echo -e "${RED}[ERROR] Cross-compiler not found: ${CROSS_COMPILE}gcc${NC}"
    echo "Install with: sudo apt-get install gcc-aarch64-linux-gnu"
    exit 1
fi

echo -e "${GREEN}[OK] Cross-compiler found${NC}"
echo ""

# Compile bootloader assembly
echo -e "${YELLOW}[COMPILE] Bootloader (boot.s)${NC}"
${CROSS_COMPILE}gcc \
    -Wall -nostdinc -fno-builtin \
    -march=armv8-a \
    -c bootloader/boot.s -o "$BUILD_DIR/boot.o" 2>&1 || {
    echo -e "${RED}[ERROR] Bootloader assembly failed${NC}"
    exit 1
}
echo -e "${GREEN}[OK] boot.o${NC}"

# Compile bootloader C code
echo -e "${YELLOW}[COMPILE] Bootloader (bootloader.c)${NC}"
${CROSS_COMPILE}gcc \
    -Wall -Wextra -nostdinc -fno-builtin \
    -march=armv8-a -Iinclude \
    -c bootloader/bootloader.c -o "$BUILD_DIR/bootloader.o" 2>&1 || {
    echo -e "${RED}[ERROR] Bootloader compilation failed${NC}"
    exit 1
}
echo -e "${GREEN}[OK] bootloader.o${NC}"

# Compile kernel
echo -e "${YELLOW}[COMPILE] Kernel (kernel/main.c)${NC}"
${CROSS_COMPILE}gcc \
    -Wall -Wextra -nostdinc -fno-builtin \
    -march=armv8-a -Iinclude \
    -c kernel/main.c -o "$BUILD_DIR/kernel.o" 2>&1 || {
    echo -e "${RED}[ERROR] Kernel compilation failed${NC}"
    exit 1
}
echo -e "${GREEN}[OK] kernel.o${NC}"

# Compile drivers
echo -e "${YELLOW}[COMPILE] Drivers${NC}"

${CROSS_COMPILE}gcc \
    -Wall -Wextra -nostdinc -fno-builtin -fno-stack-protector \
    -march=armv8-a -Iinclude \
    -c drivers/uart.c -o "$BUILD_DIR/uart.o" 2>&1 || {
    echo -e "${RED}[ERROR] UART driver compilation failed${NC}"
    exit 1
}
echo -e "${GREEN}[OK] uart.o${NC}"

${CROSS_COMPILE}gcc \
    -Wall -Wextra -nostdinc -fno-builtin \
    -march=armv8-a -Iinclude \
    -c drivers/memory.c -o "$BUILD_DIR/memory.o" 2>&1 || {
    echo -e "${RED}[ERROR] Memory driver compilation failed${NC}"
    exit 1
}
echo -e "${GREEN}[OK] memory.o${NC}"

${CROSS_COMPILE}gcc \
    -Wall -Wextra -nostdinc -fno-builtin \
    -march=armv8-a -Iinclude \
    -c drivers/interrupt.c -o "$BUILD_DIR/interrupt.o" 2>&1 || {
    echo -e "${RED}[ERROR] Interrupt driver compilation failed${NC}"
    exit 1
}
echo -e "${GREEN}[OK] interrupt.o${NC}"

${CROSS_COMPILE}gcc \
    -Wall -Wextra -nostdinc -fno-builtin \
    -march=armv8-a -Iinclude \
    -c drivers/scheduler.c -o "$BUILD_DIR/scheduler.o" 2>&1 || {
    echo -e "${RED}[ERROR] Scheduler compilation failed${NC}"
    exit 1
}
echo -e "${GREEN}[OK] scheduler.o${NC}"

${CROSS_COMPILE}gcc \
    -Wall -Wextra -nostdinc -fno-builtin \
    -march=armv8-a -Iinclude \
    -c drivers/hdmi.c -o "$BUILD_DIR/hdmi.o" 2>&1 || {
    echo -e "${RED}[ERROR] HDMI driver compilation failed${NC}"
    exit 1
}
echo -e "${GREEN}[OK] hdmi.o${NC}"

# Stub implementations for missing functions
echo -e "${YELLOW}[CREATE] Stubs${NC}"
cat > "$BUILD_DIR/stubs.c" << 'EOF'
#include "types.h"

/* Stub implementations for functions not yet implemented */

void drivers_init(void) {
    /* TODO: Initialize device drivers */
}

/* Add more stubs as needed */
EOF

${CROSS_COMPILE}gcc \
    -Wall -Wextra -nostdinc -fno-builtin \
    -march=armv8-a -I"${PROJECT_DIR}/include" \
    -c "$BUILD_DIR/stubs.c" -o "$BUILD_DIR/stubs.o" 2>&1 || {
    echo -e "${RED}[ERROR] Stubs compilation failed${NC}"
    exit 1
}
echo -e "${GREEN}[OK] stubs.o${NC}"

# Link all objects
echo -e "${YELLOW}[LINK] Creating kernel ELF${NC}"

# Create linker script if it doesn't exist
if [ ! -f "linker.ld" ]; then
    cat > "linker.ld" << 'LINKER_EOF'
ENTRY(_start)

SECTIONS {
    . = 0x80000;
    
    .text : {
        *(.text.boot)
        *(.text)
    }
    
    .data : {
        *(.data)
    }
    
    .bss : {
        bss_start = .;
        *(.bss)
        bss_end = .;
    }
}
LINKER_EOF
    echo -e "${GREEN}[OK] linker.ld created${NC}"
fi

${CROSS_COMPILE}ld \
    -T linker.ld \
    -o "$BUILD_DIR/kernel.elf" \
    "$BUILD_DIR/boot.o" \
    "$BUILD_DIR/bootloader.o" \
    "$BUILD_DIR/kernel.o" \
    "$BUILD_DIR/uart.o" \
    "$BUILD_DIR/memory.o" \
    "$BUILD_DIR/interrupt.o" \
    "$BUILD_DIR/scheduler.o" \
    "$BUILD_DIR/hdmi.o" \
    "$BUILD_DIR/stubs.o" 2>&1 || {
    echo -e "${RED}[ERROR] Linking failed${NC}"
    exit 1
}

echo -e "${GREEN}[OK] kernel.elf${NC}"

# Create kernel image
echo -e "${YELLOW}[OBJCOPY] Creating kernel image${NC}"

${CROSS_COMPILE}objcopy \
    -O binary \
    "$BUILD_DIR/kernel.elf" \
    "$BUILD_DIR/kernel.img" 2>&1 || {
    echo -e "${RED}[ERROR] Image creation failed${NC}"
    exit 1
}

echo -e "${GREEN}[OK] kernel.img${NC}"

# Print summary
echo ""
echo -e "${GREEN}[SUCCESS] Build complete!${NC}"
echo ""
echo "Output files:"
echo "  ELF:   $BUILD_DIR/kernel.elf ($(stat -f%z "$BUILD_DIR/kernel.elf" 2>/dev/null || stat -c%s "$BUILD_DIR/kernel.elf") bytes)"
echo "  Image: $BUILD_DIR/kernel.img ($(stat -f%z "$BUILD_DIR/kernel.img" 2>/dev/null || stat -c%s "$BUILD_DIR/kernel.img") bytes)"
echo ""
echo "To flash to SD card:"
echo "  sudo dd if=$BUILD_DIR/kernel.img of=/dev/sdX bs=4M"
echo "  (Replace /dev/sdX with your SD card device)"
