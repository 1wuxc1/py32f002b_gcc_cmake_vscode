cmake_minimum_required(VERSION 3.10.0)
include(sdk_config.cmake)

# 设置交叉编译工具链
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-gcc.exe)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}/arm-none-eabi-objcopy.exe)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PATH}/arm-none-eabi-objdump.exe)
set(CMAKE_SIZE ${TOOLCHAIN_PATH}/arm-none-eabi-size.exe)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-gcc.exe)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-g++.exe)  # 添加这一行
set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}/arm-none-eabi-objcopy.exe)
# 添加C++编译标志
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

# 强制使用指定的工具链
set(CMAKE_C_COMPILER_FORCED TRUE)

project(py32f002b_demo VERSION 0.1.0 LANGUAGES C CXX ASM)


# 添加源文件
set(SOURCES
    main.cpp
    startup_py32f002bxx.s
    ${SDK_PATH}/Drivers/BSP/PY32F002Bxx_Start_Kit/py32f002bxx_Start_Kit.c
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Src/py32f002b_hal.c
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Src/py32f002b_hal_rcc.c
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Src/py32f002b_hal_rcc_ex.c
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Src/py32f002b_hal_gpio.c
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Src/py32f002b_hal_pwr.c
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Src/py32f002b_hal_uart.c
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Src/py32f002b_hal_cortex.c
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Src/py32f002b_hal_adc.c
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Src/py32f002b_hal_tim.c
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Src/py32f002b_hal_lptim.c
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Src/py32f002b_hal_tim_ex.c
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Src/py32f002b_hal_i2c.c
    # ${FreeRTOS_PATH}/portable/GCC/ARM_CM0/port.c
    # ${FreeRTOS_PATH}/portable/GCC/ARM_CM0/portasm.c
    # ${FreeRTOS_PATH}/portable/GCC/ARM_CM0/mpu_wrappers_v2_asm.c
    ${FreeRTOS_PATH}/tasks.c
    ${FreeRTOS_PATH}/queue.c
    ${FreeRTOS_PATH}/list.c
    ${FreeRTOS_PATH}/event_groups.c
    ${FreeRTOS_PATH}/timers.c
    ${FreeRTOS_PATH}/portable/MemMang/heap_2.c



)


add_executable(py32f002b_demo ${SOURCES})
# 在 add_executable 之后添加
set_property(SOURCE startup_py32f002bxx.s PROPERTY LANGUAGE ASM)

add_subdirectory(user)

# 添加头文件搜索路径
target_include_directories(py32f002b_demo PRIVATE
    Inc
    ${SDK_PATH}/Drivers/BSP/PY32F002Bxx_Start_Kit
    ${SDK_PATH}/Drivers/PY32F002B_HAL_Driver/Inc
    ${SDK_PATH}/Drivers/CMSIS/Device/PY32F0xx/Include
    ${SDK_PATH}/Drivers/CMSIS/Include
    ${FreeRTOS_PATH}/include
    # ${FreeRTOS_PATH}/portable/GCC/ARM_CM0
)

# 修复 main 函数返回类型警告
target_compile_options(py32f002b_demo PRIVATE
    -mcpu=cortex-m0plus
    -mthumb
    -Wall
    -ffunction-sections
    -fdata-sections
    -DUSE_HAL_DRIVER
    -DPY32F002Bx5
)

# 设置 ARM 链接选项，明确指定使用 ld 链接器并避免 Windows 特定选项
target_link_options(py32f002b_demo PRIVATE
    -mcpu=cortex-m0plus
    -mthumb
    -Wl,--gc-sections
    -static
    # -T"${CMAKE_SOURCE_DIR}/py32f002bx5.ld"  # 链接脚本
)

# 设置输出文件扩展名
set_target_properties(py32f002b_demo PROPERTIES
    OUTPUT_NAME "py32f002b_demo"
    SUFFIX ".elf"
)

# 生成 HEX 和 BIN 文件
add_custom_command(TARGET py32f002b_demo POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:py32f002b_demo> $<TARGET_FILE_DIR:py32f002b_demo>/py32f002b_demo.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:py32f002b_demo> $<TARGET_FILE_DIR:py32f002b_demo>/py32f002b_demo.bin
    COMMENT "Generating HEX and BIN files..."
    VERBATIM
)

# 显示程序大小
add_custom_command(TARGET py32f002b_demo POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:py32f002b_demo>
    COMMENT "Displaying memory usage..."
    VERBATIM
)