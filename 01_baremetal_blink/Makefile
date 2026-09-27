# Toolchain
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# Target CPU Flags for STM32F446 (Cortex-M4 with FPU)
CPU_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Compilation Flags
CFLAGS  = $(CPU_FLAGS) -Wall -Wextra -g -O0 -std=c99
LDFLAGS = $(CPU_FLAGS) -T linker.ld -nostartfiles -Wl,-Map=blink.map

# Source files
SRCS = startup.c main.c
OBJS = $(SRCS:.c=.o)

all: blink.bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

blink.elf: $(OBJS) linker.ld
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

blink.bin: blink.elf
	$(OBJCOPY) -O binary $< $@

# Flash directly to Nucleo board via ST-LINK at Flash origin (0x08000000)
flash: blink.bin
	st-flash write blink.bin 0x08000000

clean:
	rm -f $(OBJS) blink.elf blink.bin blink.map

.PHONY: all flash clean
