set(TOOLCHAIN_PATH ${CMAKE_CURRENT_LIST_DIR}/libopencm3)

set(CMAKE_C_COMPILER        /usr/bin/arm-none-eabi-gcc)
set(CMAKE_C_STANDARD        11)
set(CMAKE_C_COMPILER_WORKS   1)
set(CMAKE_CXX_COMPILER      /usr/bin/arm-none-eabi-g++)
set(CMAKE_CXX_STANDARD      14)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(stm32_common_CFlags "\
    -ffunction-sections -fdata-sections -fno-common --static -Wl,--gc-sections \
    -nostartfiles \
")
include_directories (${TOOLCHAIN_PATH}/libopencm3/include)
list(APPEND toolchain_libs m c gcc nosys)

if(${BOARD} STREQUAL "stm32f0")
    set(BOARD_CFlags
        "-mcpu=cortex-m0 -mthumb \
        -DSTM32F0 -Tlibopencm3_stm32f0.ld")
    list(APPEND toolchain_libs opencm3_stm32f0)
    set(OpenOCD_CFG "/usr/share/openocd/scripts/board/st_nucleo_f0.cfg")

elseif(${BOARD} STREQUAL "stm32f3")

    set(BOARD_CFlags
        "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
        -DSTM32F3 -Tlibopencm3_stm32f3.ld")
    list(APPEND toolchain_libs opencm3_stm32f3)

elseif(${BOARD} STREQUAL "stm32f407-discovery")
    set(BOARD_CFlags
        "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
        -DSTM32F4 -Tstm32f4-discovery.ld")
    list(APPEND toolchain_libs opencm3_stm32f4)

elseif(${BOARD} STREQUAL "stm32f429i-discovery")
    set(BOARD_CFlags
        "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
        -DSTM32F4 -Tstm32f429i-discovery.ld")
    list(APPEND toolchain_libs opencm3_stm32f4)
    set(OpenOCD_CFG "/usr/share/openocd/scripts/board/stm32429i_eval_stlink.cfg")

elseif(${BOARD} STREQUAL "stm32l4")
    set(BOARD_CFlags
        "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
        -DSTM32L4 -Tlibopencm3_stm32l4.ld")
    list(APPEND toolchain_libs opencm3_stm32l4)

endif()

set(CMAKE_C_FLAGS "-DDEBUG=${DEBUG} -L${TOOLCHAIN_PATH} ${BOARD_CFlags} ${stm32_common_CFlags}"
CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

if (NOT TARGET flash)


add_custom_target(flash
    DEPENDS main_binary
    COMMAND arm-none-eabi-objcopy -Oihex $<TARGET_FILE:main_binary> $<TARGET_FILE:main_binary>.hex
    COMMAND echo "Flashing the STM32 Board !"
    COMMAND sudo openocd -f ${OpenOCD_CFG}
            -c \"init\"
            -c \"reset init\"
            -c \"flash write_image erase $<TARGET_FILE:main_binary>.hex\"
            -c \"reset\"
            -c \"shutdown\"
)

endif()

if (NOT TARGET lib-stm32f4)

add_custom_target(lib-stm32f0 WORKING_DIRECTORY ${TOOLCHAIN_PATH}
    COMMAND make -C libopencm3 lib/stm32/f0
    COMMAND cp libopencm3/lib/libopencm3_stm32f0.a  .
    COMMAND cp libopencm3/lib/libopencm3_stm32f0.ld .
)
add_custom_target(lib-stm32f1 WORKING_DIRECTORY ${TOOLCHAIN_PATH}
    COMMAND make -C libopencm3 lib/stm32/f1
    COMMAND cp libopencm3/lib/libopencm3_stm32f1.a  .
    COMMAND cp libopencm3/lib/libopencm3_stm32f1.ld .
)
add_custom_target(lib-stm32f3 WORKING_DIRECTORY ${TOOLCHAIN_PATH}
    COMMAND make -C libopencm3 lib/stm32/f3
    COMMAND cp libopencm3/lib/libopencm3_stm32f3.a  .
    COMMAND cp libopencm3/lib/libopencm3_stm32f3.ld .
)
add_custom_target(lib-stm32f4 WORKING_DIRECTORY ${TOOLCHAIN_PATH}
    COMMAND make -C libopencm3 lib/stm32/f4
    COMMAND cp libopencm3/lib/libopencm3_stm32f4.a  .
    COMMAND cp libopencm3/lib/libopencm3_stm32f4.ld .
)

add_custom_target(lib-stm32l4 WORKING_DIRECTORY ${TOOLCHAIN_PATH}
    COMMAND make -C libopencm3 lib/stm32/l4
    COMMAND cp libopencm3/lib/libopencm3_stm32l4.a  .
    COMMAND cp libopencm3/lib/libopencm3_stm32l4.ld .
)

endif()
