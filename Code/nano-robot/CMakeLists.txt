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
add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)
add_executable(main_binary ALIAS ${PROJECT_NAME})

include_directories(
    ${PROJECT_SOURCE_DIR}/
)


target_link_libraries(${PROJECT_NAME}
    ${toolchain_libs}
)


add_custom_target(flash-nano-drone
    DEPENDS main_binary
    COMMAND arm-none-eabi-objcopy -Oihex $<TARGET_FILE:main_binary> $<TARGET_FILE:main_binary>.hex
    COMMAND echo "Flashing the Nano Drone !"
    COMMAND sudo openocd
            -f stm32f0-openocd.cfg
            -c \"reset_config none separate\"
            -c \"init\"
            -c \"reset init\"
            -c \"stm_flash $<TARGET_FILE:main_binary>.hex\"
            -c \"shutdown\"
)
