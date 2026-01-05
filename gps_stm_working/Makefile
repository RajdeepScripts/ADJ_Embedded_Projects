# ==== STM32F411 Test Project Makefile ====
NAME = gps_stm
SRC = main.c gps.c

# Toolchain

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# MCU & Flags
MCU = -mcpu=cortex-m4 -mthumb
DEFS = -DSTM32F411xE
CFLAGS = $(MCU) -O0 -g -Wall $(DEFS) -ffreestanding -specs=nano.specs

# Paths to common resources
COMMON_ROOT = ../common
COMMON_INCLUDE = $(COMMON_ROOT)/include
COMMON_LIB = $(COMMON_ROOT)/lib
COMMON_SRC = $(COMMON_ROOT)/src
COMMON_MCU_DIR = $(COMMON_ROOT)/mcu/stm32f4xx


INCLUDE_BASE = /usr/local
INCLUDES = -I $(INCLUDE_BASE)/bin/gcc-arm-none-eabi-10.3-2021.10/arm-none-eabi/include   \
           -I $(INCLUDE_BASE)/include/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Include     \
           -I $(INCLUDE_BASE)/include/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
	   -I /home/rajdeep/misc/src/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
	   -I /home/rajdeep/misc/src/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Core/Include \
           -I /home/rajdeep/common/include



# Files
STARTUP = $(COMMON_MCU_DIR)/startup/startup_stm32f411xx.s
LINKER = $(COMMON_MCU_DIR)/ldscripts/STM32F411RETX_FLASH.ld

OBJ = $(SRC:.c=.o) startup_stm32f411xx.o

TARGET = $(NAME).elf
BIN = $(NAME).bin

# Library to link
DEBUG_LIB = $(COMMON_LIB)/libstm32_debug.a

# ==== Build rules ====

all: $(TARGET) $(BIN)

# Build common library first if needed
$(DEBUG_LIB):
	$(MAKE) -C $(COMMON_SRC)/debug

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

startup_stm32f411xx.o: $(STARTUP)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(DEBUG_LIB) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -T $(LINKER) -L$(COMMON_LIB) -lstm32_debug -lc -lnosys -o $(TARGET)

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $(TARGET) $(BIN)

run:
	openocd -f board/st_nucleo_f4.cfg -c "program $(TARGET) verify reset exit"

clean:
	rm -f $(OBJ) $(TARGET) $(BIN)

clean-all: clean
	$(MAKE) -C $(COMMON_SRC)/debug clean

.PHONY: all clean clean-all run
