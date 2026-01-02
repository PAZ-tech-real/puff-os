# Contributing to Raspberry Pi 5 Custom OS

Thank you for your interest in contributing! This guide will help you get started.

## Code of Conduct

- Be respectful and inclusive
- No harassment, discrimination, or offensive language
- Focus on the code, not the person
- Help each other learn and improve

## Getting Started

### Prerequisites

1. **Fork the repository** on GitHub
2. **Clone your fork locally:**
   ```bash
   git clone https://github.com/YOUR-USERNAME/os.git
   cd os
   ```

3. **Install dependencies:**
   ```bash
   sudo apt-get install gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu
   ```

4. **Create a development branch:**
   ```bash
   git checkout -b feature/your-feature-name
   ```

### Build & Test

Before submitting, ensure everything builds:

```bash
./dev clean
./dev build
./dev image
```

Check for warnings and errors:
```bash
./dev info        # Show kernel information
./dev dump        # Show disassembly
```

## What to Contribute

### 🟢 Easy (Good First Issues)

Perfect for beginners:

- **Documentation improvements** - Fix typos, clarify instructions, add examples
- **Code comments** - Add documentation to existing functions
- **Examples** - Create demo programs showing driver usage
- **README enhancements** - Improve clarity or add illustrations

### 🟡 Medium Difficulty

For intermediate developers:

- **New device drivers** - I2C, SPI, PWM, RTC, Temperature sensors
- **Scheduler improvements** - Priority levels, context switching
- **Memory allocator** - Add proper deallocation, reduce fragmentation
- **Graphics enhancements** - Font rendering, sprite support, double buffering

### 🔴 Advanced

For experienced embedded systems developers:

- **File system** - FAT32 or simple ext4 implementation
- **Multi-core support** - Utilize all 4 ARM cores
- **Virtual memory** - MMU implementation
- **USB support** - Host and device modes
- **Network stack** - Ethernet driver and TCP/IP

## Development Workflow

### 1. Create a Feature Branch

```bash
git checkout -b feature/descriptive-name
```

### 2. Make Changes

- Keep commits small and focused
- Write clear commit messages
- Follow the existing code style
- Add comments for complex logic

### 3. Build and Test

```bash
./dev rebuild    # Clean build
./dev image      # Generate image for testing
```

### 4. Commit Your Changes

```bash
git add .
git commit -m "Brief description of changes

- Detailed point 1
- Detailed point 2
- Any breaking changes or important notes"
```

### 5. Push to Your Fork

```bash
git push origin feature/descriptive-name
```

### 6. Create a Pull Request

- Go to GitHub and create a PR from your branch to `main`
- Describe what you changed and why
- Reference any related issues
- Include screenshots or demos if applicable

## Code Style Guidelines

### C Code

**Naming:**
```c
// Functions: snake_case
void uart_init(u64 base);
void gpio_set_pin(u32 pin, u32 value);

// Variables: snake_case
static volatile u64 uart_base = 0;
u32 pin_mask = 0x1F;

// Constants: UPPER_SNAKE_CASE
#define UART_BASE 0xFE201000
#define GPIO_PIN_COUNT 28

// Types: keep include/types.h up to date
typedef unsigned int u32;
typedef unsigned long long u64;
```

**Formatting:**
```c
// Use 4-space indentation
if (condition) {
    // Code here
}

// Functions with clear documentation
/**
 * function_name - Brief description
 * @param1: Description of param1
 * @param2: Description of param2
 * Returns: Description of return value
 */
void function_name(int param1, int param2) {
    // Implementation
}

// Register access patterns
*(volatile u32*)(base + OFFSET) = value;  // Write
u32 value = *(volatile u32*)(base + OFFSET);  // Read
```

**Comments:**
```c
// Single line comments for brief notes
int result = calculate();

/* Multi-line comments for
   longer explanations or
   important notes */

/* TODO: Future improvements
   XXX: Known issues
   FIXME: Bugs to fix
   HACK: Temporary solutions */
```

### Documentation

- Use Markdown for .md files
- Keep lines under 80 characters where possible
- Use code blocks for examples
- Link to related documentation
- Include table of contents for long docs

## Testing

### Manual Testing

1. **Build on clean system:**
   ```bash
   ./dev clean && ./dev build
   ```

2. **Create test image:**
   ```bash
   ./dev image
   ```

3. **Flash to test SD card:**
   ```bash
   sudo dd if=build/rpi5-os.img of=/dev/sdb bs=4M
   ```

4. **Boot on Pi5 and verify:**
   - Check HDMI display
   - Monitor serial console
   - Test GPIO pins
   - Verify UART communication

### Known Issues When Testing

If you encounter issues:
1. Check existing [GitHub Issues](https://github.com/PAZ-tech-real/os/issues)
2. Document the issue thoroughly
3. Include hardware information (Pi5 variant, SD card type, cables used)
4. Provide serial console output or error messages

## Documentation Requirements

### For New Features

When adding significant features, include:

1. **Function documentation** - Clear header with parameters and return values
2. **Inline comments** - Explain non-obvious logic
3. **Usage example** - Show how to use the new feature
4. **Update README.md** - Add to feature list or status section

### Example

```c
/**
 * gpio_set_direction - Set GPIO pin direction
 * @pin: GPIO pin number (0-27)
 * @direction: GPIO_INPUT (0) or GPIO_OUTPUT (1)
 * Returns: 0 on success, -1 on error
 * 
 * Example:
 *   gpio_set_direction(4, GPIO_OUTPUT);  // Set pin 4 as output
 *   gpio_set_pin(4, 1);                  // Set pin 4 high
 */
int gpio_set_direction(u32 pin, u32 direction) {
    if (pin >= GPIO_PIN_COUNT) return -1;
    // Implementation...
}
```

## Pull Request Process

### Before Submitting

- [ ] Code builds without errors: `./dev build`
- [ ] All changes tested locally
- [ ] Commit messages are clear and descriptive
- [ ] Documentation is updated if needed
- [ ] No build artifacts committed (only source code)
- [ ] Code follows style guidelines

### PR Description Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix (non-breaking)
- [ ] New feature (non-breaking)
- [ ] Breaking change (explain below)
- [ ] Documentation update

## Related Issues
Fixes #(issue number)
Related to #(other issue)

## Testing
Describe how you tested these changes:
- [ ] Built successfully
- [ ] Tested on Raspberry Pi 5
- [ ] Manual testing performed

## Hardware Tested
- Pi5 variant: (8GB/16GB)
- SD card: (brand/capacity)
- Display: (HDMI cable type)
```

### Review Process

1. **Automated checks** - GitHub Actions will validate the code
2. **Maintainer review** - Code review and feedback
3. **Revision** - Update code based on feedback
4. **Merge** - Once approved, changes are merged to main

## Questions?

- **General questions**: Open a [Discussion](https://github.com/PAZ-tech-real/os/discussions)
- **Report bugs**: [Create an Issue](https://github.com/PAZ-tech-real/os/issues/new)
- **Need help**: Comment on relevant issues or discussions

## Recognition

Contributors will be recognized:
- In the commit message
- In the CONTRIBUTORS.md file (coming soon)
- In the GitHub contributors page
- In release notes for major contributions

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for helping improve the Raspberry Pi 5 OS project!** 🚀
