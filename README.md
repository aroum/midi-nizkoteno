# Nizkoteno MIDI Firmware

MIDI firmware for the **Nizkoteno** keyboard based on the RP2040 microcontroller.

## Description

This is an embedded system firmware project for the Nizkoteno MIDI keyboard developed on the Raspberry Pi Pico (RP2040) platform. The firmware enables:

- 🎹 Send MIDI notes and control signals (CC)
- 🎨 Control RGB LED indicators (WS2812B) for each button
- 🔌 Connect via USB-MIDI to DAW (Digital Audio Workstations)
- 🎵 Fully customize button assignments and LED colors

## Features

- **10 programmable buttons** with individual RGB LED indicators
- **Flexible configuration** — all settings in one `config.h` file
- **Fast build** — automated `build_all.sh` script
- **USB-MIDI** — standard connection to computer
- **Status LED** — activity indicator

## Requirements

### Hardware

- Raspberry Pi Pico (RP2040)
- 10 buttons with pull-up resistors
- WS2812B RGB LED strip (addressable LEDs)
- USB cable for programming and power

### Software

- CMake 3.13+
- ARM GCC (`arm-none-eabi-gcc`, `arm-none-eabi-size`)
- Pico SDK (automatically loaded via CMake)

## Building

### Quick Build

```bash
./build_all.sh
```

### Build Options

```bash
./build_all.sh [options]

-c, --clean    Delete build directory and reinitialize CMake
-s, --size     Show detailed memory usage report
-f, --flash    Build and flash firmware to RP2040 via picotool
-h, --help     Show help

Example: ./build_all.sh -csf  # Clean, show size, and flash
```

### Results

After successful build:

- `build/nizkoteno_midi.uf2` — file for loading (via BOOTSEL)
- `build/nizkoteno_midi.elf` — ELF file with debug information

## Configuration

All settings are located in the **`src/config.h`** file:

### MIDI Assignments

```c
static const button_config_t BUTTON_CONFIGS[10] = {
    {MSG_NOTE, 60, 0xFF0000}, // Button 0: Note 60, red LED
    {MSG_CC,   10, 0x00FFFF}, // Button 5: CC 10, cyan LED
    // ...
};
```

**Parameters:**

- `type` — `MSG_NOTE` (MIDI note) or `MSG_CC` (control signal)
- `index` — note number (0-127) or CC number
- `color` — LED color in RGB format (0xRRGGBB)

### GPIO Pin Distribution

```c
#define BUTTON_PINS[]   {9, 8, 6, 5, 10, 7, 4, 2, 1, 3}  // GPIO button pins
#define PIN_RGB         11                                 // WS2812B data
#define PIN_STATUS_LED  15                                 // Status LED
```

### USB Identifiers

```c
#define USB_VENDOR_ID   0xCAFE
#define USB_PRODUCT_ID  0x4005
#define USB_MANUFACTURER "Stepateho"
#define USB_PRODUCT     "Nizkoteno MIDI"
```

## Flashing Firmware

### Method 1: Via BOOTSEL (manual)

1. Connect Pico to PC via USB cable
2. Press and hold the **BOOTSEL** button
3. Press the **RESET** button
4. Release **BOOTSEL**
5. Pico will appear as a USB flash drive
6. Copy `build/nizkoteno_midi.uf2` to Pico

### Method 2: Via picotool (automatic)

```bash
./build_all.sh -f
```

Requires Pico in BOOTSEL mode or ready to flash.

## Project Structure

```
.
├── CMakeLists.txt           # CMake configuration
├── build_all.sh             # Build script
├── src/
│   ├── config.h             # Project configuration
│   ├── main.c               # Main code
│   ├── usb/
│   │   ├── tusb_config.h    # USB config
│   │   └── usb_descriptors.c
│   └── leds/
│       └── ws2812.pio       # PIO program for WS2812B
└── build/                   # Build directory (created automatically)
```

## License

See the [LICENSE](LICENSE) file for details.
