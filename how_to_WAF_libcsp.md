# Building libcsp for STM32 F411RE NUCLEO Board

## Prerequisites

1. Use Ubuntu or WSL (Windows Subsystem for Linux). This process doesn't work directly on Windows.

2. Set up the ARM GNU toolchain (compilers, linkers, debuggers) on Linux:
   Follow the guide at [Install Arm GNU Toolchain on Ubuntu](https://lindevs.com/install-arm-gnu-toolchain-on-ubuntu)

## Build Process

1. Navigate to the libcsp directory in your workspace:
   ```
   cd <your_workspace>/OBC_new/Libraries/libcsp
   ```

2. Configure WAF (replace paths with your actual directories):
   ```
   CFLAGS="-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g" \
   LINKFLAGS="-latomic" \
   ./waf configure \
      --toolchain=arm-none-eabi- \
      --with-os=freertos \
      --enable-rtable \
      --prefix=/mnt/c/Users/Alessandro/STM32CubeIDE/workspace_1.13.2/OBC_new/Libraries \
      --includes=/mnt/c/Users/Alessandro/STM32CubeIDE/workspace_1.13.2/OBC_new/Middlewares/Third_Party/FreeRTOS/Source/include,\
   /mnt/c/Users/Alessandro/STM32CubeIDE/workspace_1.13.2/OBC_new/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F,\
   /mnt/c/Users/Alessandro/STM32CubeIDE/workspace_1.13.2/OBC_new/Core/Inc,\
   /mnt/c/Users/Alessandro/STM32CubeIDE/workspace_1.13.2/OBC_new/Drivers/STM32F4xx_HAL_Driver/Inc,\
   /mnt/c/Users/Alessandro/STM32CubeIDE/workspace_1.13.2/OBC_new/Drivers/CMSIS/Device/ST/STM32F4xx/Include,\
   /mnt/c/Users/Alessandro/STM32CubeIDE/workspace_1.13.2/OBC_new/Drivers/CMSIS/Include \
      -v
   ```

3. Build and install:
   ```
   ./waf build install
   ```

## STM32CubeIDE Configuration
