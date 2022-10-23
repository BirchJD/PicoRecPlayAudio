cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS"${CMAKE_C_FLAGS}-O3")
set(CMAKE_CXX_FLAGS"${CMAKE_CXX_FLAGS}-O3")

include(pico_sdk_import.cmake)

project(PicoRecPlayAudio C CXX ASM)

pico_sdk_init()
add_executable(PicoRecPlayAudio
   PicoRecPlayAudio.c
   WavPwmAudio.c
)

target_link_libraries(PicoRecPlayAudio
   pico_stdlib
   hardware_adc
   hardware_dma
   hardware_pwm
)

pico_enable_stdio_usb(PicoRecPlayAudio 1)
pico_enable_stdio_uart(PicoRecPlayAudio 0)
pico_add_extra_outputs(PicoRecPlayAudio)

