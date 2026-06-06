cmake_minimum_required(VERSION 3.13)

# Initialize pico-sdk
include(pico_sdk_import.cmake)

project(nizkoteno_midi C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(nizkoteno_midi
    src/main.c
    src/usb/usb_descriptors.c
)

# Add PIO generation
pico_generate_pio_header(nizkoteno_midi ${CMAKE_CURRENT_LIST_DIR}/src/leds/ws2812.pio)

# Add include paths
target_include_directories(nizkoteno_midi PRIVATE
    src
    src/usb
)

# Pull in common dependencies
target_link_libraries(nizkoteno_midi 
    pico_stdlib 
    hardware_pio 
    hardware_dma 
    tinyusb_device 
    tinyusb_board
)

# Enable USB MIDI
target_compile_definitions(nizkoteno_midi PRIVATE
    CFG_TUSB_MCU=OPT_MCU_RP2040
    CFG_TUD_MIDI=1
    CFG_TUD_CDC=0
    CFG_TUD_MSC=0
)

# Enable usb output
pico_enable_stdio_usb(nizkoteno_midi 1)
pico_enable_stdio_uart(nizkoteno_midi 0)

# Create map/bin/hex/uf2 file
pico_add_extra_outputs(nizkoteno_midi)
