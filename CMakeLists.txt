# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(picoCPU C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(picoCPU picoCPU.cpp Audio.cpp TextureManager.cpp GeometryManager.cpp Bluetooth.cpp)


target_compile_definitions(picoCPU PRIVATE
    CYW43_PIO_CLOCK_DIV_DYNAMIC=1
)
target_compile_definitions(picoCPU PRIVATE
    CYW43_PIO_CLOCK_DIV_INT=4
    CYW43_PIO_CLOCK_DIV_FRAC8=0
)

pico_set_program_name(picoCPU "picoCPU")
pico_set_program_version(picoCPU "0.1")

# Generate PIO header
pico_generate_pio_header(picoCPU ${CMAKE_CURRENT_LIST_DIR}/dac_out.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(picoCPU 1)
pico_enable_stdio_usb(picoCPU 0)

# Add the standard library to the build
target_link_libraries(picoCPU
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(picoCPU PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
# Add any user requested libraries
target_link_libraries(picoCPU 
        hardware_spi
        hardware_i2c
        hardware_dma
        hardware_pio
        hardware_interp
        hardware_timer
        hardware_watchdog
        hardware_clocks
        pico_multicore
        pico_btstack_ble
        pico_btstack_cyw43
        pico_cyw43_arch_none
        )

target_compile_definitions(picoCPU PRIVATE
    RUNNING_AS_CLIENT=1
)
pico_add_extra_outputs(picoCPU)

