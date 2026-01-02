# SD Card Image Creation Guide

## Quick Start

### Create IMG File (Simplest Method)

```bash
# 1. Build kernel
./dev build

# 2. Create .img file
./dev image

# Creates: build/rpi5-os.img (1GB)
```

### Flash to SD Card

```bash
# With dev script
./dev flash /dev/sdb

# Or manually
sudo dd if=build/rpi5-os.img of=/dev/sdb bs=4M status=progress
sync
sudo eject /dev/sdb
```

---

## What Gets Created

When you run `./dev image`, it creates:

```
build/rpi5-os.img          ← Complete bootable SD card image (1GB)
```

This image contains:
- ✅ Bootable partition table
- ✅ Kernel (kernel.img)
- ✅ Boot configuration (config.txt)
- ✅ Kernel command line (cmdline.txt)
- ✅ Ready to flash to any SD card

---

## Complete Workflow

### Step 1: Build
```bash
cd /workspaces/os
./dev build
```

### Step 2: Create Image
```bash
./dev image
```

Wait for completion...
```
==================================================
   Raspberry Pi 5 Bootable Image Creator
==================================================

[CREATE] Creating 1024 MB image...
[IMAGE] Creating raw disk image...
...
✓ Image Template Created
==================================================

Image file: /workspaces/os/build/rpi5-os.img
Size: 1024 MB

To flash to SD card:
  sudo dd if=/workspaces/os/build/rpi5-os.img of=/dev/sdb bs=4M
  sync
```

### Step 3: Flash to SD Card

**Find your SD card:**
```bash
lsblk
```

**Unmount it:**
```bash
sudo umount /dev/sdb*
```

**Flash the image:**
```bash
sudo dd if=build/rpi5-os.img of=/dev/sdb bs=4M status=progress
sync
```

**Eject:**
```bash
sudo eject /dev/sdb
```

### Step 4: Boot Pi5

1. Insert SD card into Raspberry Pi 5
2. Connect USB-C power
3. Pi boots with your custom OS!

---

## Image File Locations

After building:

```
build/
├── kernel.img              ← Just the kernel
└── rpi5-os.img             ← Complete bootable image (NEW!)
```

---

## Copy Image for Backup

```bash
# Copy to home directory
cp build/rpi5-os.img ~/rpi5-custom-os.img

# Copy to USB drive
cp build/rpi5-os.img /mnt/usb/

# Share or archive
tar -czf rpi5-os-backup.tar.gz build/rpi5-os.img
```

---

## Use Image on Multiple Pi5s

Once you have the `.img` file, flash it to as many SD cards as you want:

```bash
# Flash to first card
sudo dd if=build/rpi5-os.img of=/dev/sdb bs=4M status=progress

# Flash to second card
sudo dd if=build/rpi5-os.img of=/dev/sdc bs=4M status=progress

# etc...
```

---

## Available Image Commands

```bash
./dev image           # Create basic bootable image
./dev image-full      # Create detailed image with filesystem
```

---

## Troubleshooting

### Image creation fails
```bash
# Make sure build succeeds first
./dev rebuild

# Then try image creation
./dev image
```

### Can't find SD card device
```bash
lsblk
```

Look for device name (sdb, sdc, mmcblk0, etc.)

### Image won't boot
- Verify correct device used for flashing
- Check SD card is good (try different card)
- Ensure power supply is adequate (5V, 5A)

### Image is too large
The image is intentionally 1GB to ensure compatibility. You can:
- Use a 2GB+ SD card
- Resize partition after flashing
- Modify IMAGE_SIZE in create-full-image.sh

---

## How It Works

The `./dev image` command:

1. Takes your compiled `kernel.img`
2. Creates a 1GB raw disk image
3. Sets up partition table (MBR)
4. Formats boot partition (FAT32)
5. Copies kernel and config files
6. Saves as `build/rpi5-os.img`

Result: A complete bootable disk image ready to flash!

---

## Next Steps

1. **Build:** `./dev build`
2. **Create image:** `./dev image`
3. **Flash:** `sudo dd if=build/rpi5-os.img of=/dev/sdb bs=4M`
4. **Boot Pi5:** Insert SD card and power on
5. **Enjoy!** Your custom OS is running 🚀

---

## Files Involved

```
/workspaces/os/
├── scripts/
│   ├── create-image.sh              # Complex version (all components)
│   └── create-full-image.sh         # Simpler version (recommended)
├── config.txt                        # Boot configuration
├── dev                               # Main development script (with image commands)
└── build/
    └── rpi5-os.img                  # Final image file
```

---

All set! Use `./dev image` to create your bootable disk image. 🎉
