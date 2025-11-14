# ==== STM32F411 Bare-Metal Blink Makefile ====

CC       = arm-none-eabi-gcc
OBJCOPY  = arm-none-eabi-objcopy

# MCU & Flags
MCU = -mcpu=cortex-m4 -mthumb
DEFS = -DSTM32F411xE
CFLAGS = $(MCU) -O0 -g -Wall $(DEFS) -ffreestanding -nostdlib

# Include paths
INCLUDES = \
  -I/home/vboxuser/STM32CubeF4/Drivers/CMSIS/Include \
  -I/home/vboxuser/cmsis_device_f4/Include \
  -I/home/vboxuser/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Inc

# Files
SRC = main.c startup_stm32f411xx.s
OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:.s=.o)

TARGET = blink.elf
BIN = blink.bin
LINKER = stm32f411xe.ld

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
	openocd -f board/st_nucleo_f4.cfg -c "program blink.elf verify reset exit"

clean:
	rm -f $(OBJ) $(TARGET) $(BIN)

.PHONY: all clean

