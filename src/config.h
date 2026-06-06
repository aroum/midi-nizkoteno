#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// MIDI CONFIGURATION
// =============================================================================
#define MIDI_CHANNEL 1

typedef enum {
    MSG_NOTE,
    MSG_CC
} midi_msg_t;

typedef struct {
    midi_msg_t type;
    uint8_t index;     // Note number or CC number
    uint32_t color;    // LED Color in HEX (0xRRGGBB)
} button_config_t;

// 10 MIDI Assignments and LED Colors
static const button_config_t BUTTON_CONFIGS[10] = {
    {MSG_NOTE, 60, 0xFF0000}, // Button 0: Note 60 (C4), Red
    {MSG_NOTE, 62, 0x00FF00}, // Button 1: Note 62 (D4), Green
    {MSG_NOTE, 64, 0x0000FF}, // Button 2: Note 64 (E4), Blue
    {MSG_NOTE, 65, 0xFFFF00}, // Button 3: Note 65 (F4), Yellow
    {MSG_NOTE, 67, 0xFF00FF}, // Button 4: Note 67 (G4), Magenta
    {MSG_CC,   10, 0x00FFFF}, // Button 5: CC 10 (Pan), Cyan
    {MSG_CC,   11, 0xFFFFFF}, // Button 6: CC 11 (Expr), White
    {MSG_CC,   12, 0xFF8000}, // Button 7: CC 12, Orange
    {MSG_CC,   13, 0x8000FF}, // Button 8: CC 13, Purple
    {MSG_CC,   14, 0x00FF80}  // Button 9: CC 14, Spring Green
};

// =============================================================================
// HARDWARE MAPPING (RP2040)
// =============================================================================

#define BTN_COUNT 10
static const uint8_t BUTTON_PINS[BTN_COUNT] = {9, 8, 6, 5, 10, 7, 4, 2, 1, 3};

// RGB LED Data Pin (WS2812B)
#define PIN_RGB 11

// Status LED (Glows while at least one button is held)
#define PIN_STATUS_LED 15

// USB Configuration
#define USB_VENDOR_ID  0xCAFE
#define USB_PRODUCT_ID 0x4005
#define USB_MANUFACTURER "Stepateho"
#define USB_PRODUCT      "Nizkoteno MIDI"

#endif // CONFIG_H
