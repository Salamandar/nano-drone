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
set(FreeRTOS_port_directory "${FreeRTOS_directory}/portable/GCC/ARM_CM0/")
set(FreeRTOS_sources
    ${FreeRTOS_directory}/croutine.c
    ${FreeRTOS_directory}/event_groups.c
    ${FreeRTOS_directory}/list.c
    ${FreeRTOS_directory}/queue.c
    ${FreeRTOS_directory}/tasks.c
    ${FreeRTOS_directory}/timers.c
    ${FreeRTOS_directory}/portable/MemMang/heap_3.c
    ${FreeRTOS_port_directory}/port.c
)
include_directories(
    ${FreeRTOS_directory}/include
    ${FreeRTOS_port_directory}
)

add_executable(${PROJECT_NAME}
    ${FreeRTOS_sources}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/hardware.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/leds.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/motors.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/mpu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware/radio.c
)
add_executable(main_binary ALIAS ${PROJECT_NAME})

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)


target_link_libraries(${PROJECT_NAME}
    ${toolchain_libs}
)


add_custom_target(flash-nano-drone
    DEPENDS main_binary
    COMMAND arm-none-eabi-objcopy -Oihex $<TARGET_FILE:main_binary> $<TARGET_FILE:main_binary>.hex
    COMMAND echo "Flashing the Nano Drone !"
    # COMMAND sudo openocd
    #         -f stm32f0-openocd.cfg
    #         -c \"reset_config none separate\"
    #         -c \"init\"
    #         -c \"reset init\"
    #         -c \"stm_flash $<TARGET_FILE:main_binary>.hex\"
    #         -c \"shutdown\"
    COMMAND sudo openocd
            -f interface/stlink-v2-1.cfg -f target/stm32f0x.cfg
            -c "init" -c "reset init"
            -c "flash write_image erase NanoRobot"
            -c "reset"
            -c "shutdown"
)
