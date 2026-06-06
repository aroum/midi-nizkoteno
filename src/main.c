#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "tusb.h"
#include "config.h"

//--------------------------------------------------------------------+
// LED Helpers
//--------------------------------------------------------------------+
static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

static inline uint32_t hex_to_grb(uint32_t hex) {
    uint8_t r = (hex >> 16) & 0xFF;
    uint8_t g = (hex >> 8) & 0xFF;
    uint8_t b = hex & 0xFF;
    return urgb_u32(r, g, b);
}

void update_leds(void) {
    for (int i = 0; i < BTN_COUNT; i++) {
        put_pixel(hex_to_grb(BUTTON_CONFIGS[i].color));
    }
}

//--------------------------------------------------------------------+
// Button Handling
//--------------------------------------------------------------------+
static bool button_states[BTN_COUNT] = {false};
static uint32_t button_history[BTN_COUNT] = {0};

void poll_buttons(void) {
    bool any_pressed = false;
    uint8_t channel = MIDI_CHANNEL - 1;

    for (int i = 0; i < BTN_COUNT; i++) {
        // Active low buttons
        bool raw_state = !gpio_get(BUTTON_PINS[i]);
        
        // Simple debouncing: shift history and check for stable 4 readings
        button_history[i] = (button_history[i] << 1) | raw_state;
        
        bool stable_pressed = (button_history[i] & 0x0F) == 0x0F;
        bool stable_released = (button_history[i] & 0x0F) == 0x00;

        if (stable_pressed && !button_states[i]) {
            button_states[i] = true;
            // Send MIDI Press
            if (BUTTON_CONFIGS[i].type == MSG_NOTE) {
                uint8_t msg[] = { 0x90 | channel, BUTTON_CONFIGS[i].index, 100 };
                tud_midi_stream_write(0, msg, 3);
            } else {
                uint8_t msg[] = { 0xB0 | channel, BUTTON_CONFIGS[i].index, 127 };
                tud_midi_stream_write(0, msg, 3);
            }
        } 
        else if (stable_released && button_states[i]) {
            button_states[i] = false;
            // Send MIDI Release
            if (BUTTON_CONFIGS[i].type == MSG_NOTE) {
                uint8_t msg[] = { 0x80 | channel, BUTTON_CONFIGS[i].index, 0 };
                tud_midi_stream_write(0, msg, 3);
            } else {
                uint8_t msg[] = { 0xB0 | channel, BUTTON_CONFIGS[i].index, 0 };
                tud_midi_stream_write(0, msg, 3);
            }
        }

        if (button_states[i]) any_pressed = true;
    }

    gpio_put(PIN_STATUS_LED, any_pressed);
}

//--------------------------------------------------------------------+
// Main
//--------------------------------------------------------------------+
int main(void) {
    stdio_init_all();
    tusb_init();

    // PIO WS2812 Init
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, PIN_RGB, 800000, false);

    // GPIO Init: Buttons
    for (int i = 0; i < BTN_COUNT; i++) {
        gpio_init(BUTTON_PINS[i]);
        gpio_set_dir(BUTTON_PINS[i], GPIO_IN);
        gpio_pull_up(BUTTON_PINS[i]);
    }

    // GPIO Init: Status LED
    gpio_init(PIN_STATUS_LED);
    gpio_set_dir(PIN_STATUS_LED, GPIO_OUT);
    gpio_put(PIN_STATUS_LED, 0);

    while (1) {
        tud_task();
        poll_buttons();
        
        // Update LEDs periodically to ensure they stay correct
        static uint32_t last_led_update = 0;
        uint32_t now = to_ms_since_boot(get_absolute_time());
        if (now - last_led_update > 20) { // 50Hz update
            update_leds();
            last_led_update = now;
        }
    }

    return 0;
}
