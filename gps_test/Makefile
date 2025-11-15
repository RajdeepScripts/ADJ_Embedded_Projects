CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m4 -mthumb -O0 -g -Wall -ffreestanding -nostdlib \
         -I/home/vboxuser/STM32CubeF4/Drivers/CMSIS/Include \
         -I/home/vboxuser/cmsis_device_f4/Include -DSTM32F411xE

LDFLAGS = -T stm32f411xe.ld -nostdlib -Wl,--gc-sections \
          -lc -lm -lnosys -lgcc -specs=nano.specs -u _printf_float 

SRC = main.c startup_stm32f411xx.s syscalls.c
OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:.s=.o)

TARGET = GPS.elf
BIN = GPS.bin

all: $(TARGET) $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(TARGET)

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $(TARGET) $(BIN)

upload: $(TARGET)
	openocd -f board/st_nucleo_f4.cfg -c "program GPS.elf verify reset exit"

clean:
	rm -f *.o *.elf *.bin

