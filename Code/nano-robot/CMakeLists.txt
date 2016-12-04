####################################################################
#   Project configuration
####################################################################
cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

# Toolchain = architecture utilisée
set(BOARD stm32f0)
include("${CMAKE_CURRENT_SOURCE_DIR}/libopencm3.cmake")

project (NanoRobot)
set     (NanoRobot_VERSION_MAJOR 0)
set     (NanoRobot_VERSION_MINOR 1)

####################################################################
#   Source
####################################################################
set(FreeRTOS_directory      "${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS")
set(FreeRTOS_sources
    ${FreeRTOS_directory}/croutine.c
    ${FreeRTOS_directory}/event_groups.c
    ${FreeRTOS_directory}/list.c
    ${FreeRTOS_directory}/queue.c
    ${FreeRTOS_directory}/tasks.c
    ${FreeRTOS_directory}/timers.c
    ${FreeRTOS_directory}/portable/MemMang/heap_3.c
    ${FreeRTOS_directory}/portable/GCC/ARM_CM0/port.c
)
include_directories(
    ${FreeRTOS_directory}/include
    ${FreeRTOS_directory}/portable/GCC/ARM_CM0/
)

add_executable(${PROJECT_NAME}
    ${FreeRTOS_sources}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ledsTask.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/radioTask.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/videoTask.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/hardware.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/eeprom.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/leds.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/motors.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/mpu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/radio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/video.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/HAL/main_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/HAL/stm32f0xx_hal_i2c.c
)
add_executable(main_binary ALIAS ${PROJECT_NAME})

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)
## ST HAL
add_definitions("-DSTM32F031x6")

add_definitions("-Os -g")

target_link_libraries(${PROJECT_NAME}
    ${toolchain_libs}
)
# set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wl,-Map=${PROJECT_BINARY_DIR}/output.map")

add_custom_target(flash-nano-drone
    DEPENDS main_binary
    COMMAND echo "Flashing the Nano Drone !"
    COMMAND arm-none-eabi-size $<TARGET_FILE:main_binary>
    COMMAND openocd
            -f interface/stlink-v2-1.cfg
            -f target/stm32f0x.cfg
            -c "init" -c "reset init"
            -c "flash write_image erase $<TARGET_FILE:main_binary>"
            -c "reset"
            -c "shutdown"
)

add_custom_target(print-size
    DEPENDS main_binary
    COMMAND arm-none-eabi-size $<TARGET_FILE:main_binary>
    )
