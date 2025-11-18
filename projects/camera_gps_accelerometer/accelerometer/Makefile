# ==== STM32F411 Bare-Metal Blink Makefile ====

CC       = arm-none-eabi-gcc
OBJCOPY  = arm-none-eabi-objcopy

# MCU & Flags
MCU = -mcpu=cortex-m4 -mthumb
DEFS = -DSTM32F411xE
CFLAGS = $(MCU) -O0 -g -Wall $(DEFS) -ffreestanding -nostdlib

# Include paths
INCLUDES = -I /home/rajdeep/bin/gcc-arm-none-eabi-10.3-2021.10/arm-none-eabi/include   \
	   -I /home/rajdeep/misc/src/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Include     \
	   -I /home/rajdeep/misc/src/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include

#INCLUDES = \
#  -I/home/vboxuser/STM32CubeF4/Drivers/CMSIS/Include \
#  -I/home/vboxuser/cmsis_device_f4/Include \
#  -I/home/vboxuser/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Inc

# Files
SRC = main.c startup_stm32f411xx.s adxl345.c i2c.c uart.c syscalls.c
OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:.s=.o)

TARGET = adxl345.elf
BIN = adxl345.bin
# LINKER = stm32f411xe.ld
LINKER = STM32F411RETX_FLASH.ld

# ==== Build rules ====

all: $(TARGET) $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) -T $(LINKER) -o $(TARGET)

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $(TARGET) $(BIN)
run:
	openocd -f board/st_nucleo_f4.cfg -c "program adxl345.elf verify reset exit"

clean:
	rm -f $(OBJ) $(TARGET) $(BIN)

.PHONY: all clean

