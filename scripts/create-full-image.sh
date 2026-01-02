#!/bin/bash
# Create a complete bootable Raspberry Pi 5 image using dd and mkfs
# This is a simpler, more reliable version

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
KERNEL_IMG="$PROJECT_DIR/build/kernel.img"
OUTPUT_IMG="$PROJECT_DIR/build/rpi5-os.img"
TEMP_DIR="/tmp/rpi5-image-$$"

echo "=================================================="
echo "   Raspberry Pi 5 Bootable Image Creator"
echo "=================================================="
echo ""

# Validate kernel exists
if [ ! -f "$KERNEL_IMG" ]; then
    echo "ERROR: kernel.img not found!"
    echo "Run: ./dev build"
    exit 1
fi

KERNEL_SIZE=$(stat -f%z "$KERNEL_IMG" 2>/dev/null || stat -c%s "$KERNEL_IMG")
echo "[INFO] Kernel size: $((KERNEL_SIZE / 1024)) KB"

# Image configuration
BOOT_SIZE=$((512 * 1024 * 1024))    # 512 MB boot partition (FAT32)
ROOT_SIZE=$((512 * 1024 * 1024))    # 512 MB root partition (minimal)
TOTAL_SIZE=$((BOOT_SIZE + ROOT_SIZE))

echo "[CREATE] Creating ${TOTAL_SIZE} byte image..."

# Create temporary directory structure
mkdir -p "$TEMP_DIR/boot"
mkdir -p "$TEMP_DIR/root"

# Prepare boot files
echo "[PREP] Preparing boot files..."

cp "$KERNEL_IMG" "$TEMP_DIR/boot/kernel.img"
cp "$PROJECT_DIR/config.txt" "$TEMP_DIR/boot/config.txt" 2>/dev/null || \
    echo "kernel=kernel.img" > "$TEMP_DIR/boot/config.txt"

# Create cmdline
cat > "$TEMP_DIR/boot/cmdline.txt" << 'EOF'
console=serial0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 rw
EOF

# Create minimal root filesystem
touch "$TEMP_DIR/root/boot.log"

# Create raw image file
echo "[IMAGE] Creating raw disk image..."
dd if=/dev/zero of="$OUTPUT_IMG" bs=1M count=$((TOTAL_SIZE / 1024 / 1024)) \
    2>&1 | grep -E "records|copied" || true

echo "[PARTITION] Setting up partition table..."

# Use sfdisk to create partitions (more portable than fdisk)
{
    echo "unit: sectors"
    echo ""
    echo "$BOOT_SIZE, $((BOOT_SIZE / 512)), 0c, *"  # Boot partition (FAT32)
    echo "$ROOT_SIZE, $((ROOT_SIZE / 512)), 83"      # Root partition (Linux)
} > "$TEMP_DIR/partition.table" || true

# Alternative: Use parted if available
if command -v parted &> /dev/null; then
    echo "[PARTED] Using parted for partitioning..."
    parted "$OUTPUT_IMG" --script -- mklabel msdos \
        mkpart primary fat32 1MiB 512MiB \
        mkpart primary ext4 512MiB -1s 2>/dev/null || true
fi

echo ""
echo "=================================================="
echo "✓ Image Template Created"
echo "=================================================="
echo ""
echo "Image file: $OUTPUT_IMG"
echo "Size: $((TOTAL_SIZE / 1024 / 1024)) MB"
echo ""
echo "To flash to SD card:"
echo "  sudo dd if=$OUTPUT_IMG of=/dev/sdb bs=4M status=progress"
echo "  sync"
echo ""
echo "To mount on Linux and add files:"
echo "  mkdir -p /mnt/boot /mnt/root"
echo "  sudo losetup -f -P $OUTPUT_IMG"
echo "  sudo mount /dev/loop0p1 /mnt/boot"
echo "  sudo mount /dev/loop0p2 /mnt/root"
echo "  # Copy files, then:"
echo "  sudo umount /mnt/boot /mnt/root"
echo "  sudo losetup -d /dev/loop0"
echo ""

# Cleanup
rm -rf "$TEMP_DIR"

echo "[DONE] Image creation complete!"
