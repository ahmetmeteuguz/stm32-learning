CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

CPU_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS    = $(CPU_FLAGS) -Wall -Wextra -g -O0 -std=c99
LDFLAGS   = $(CPU_FLAGS) -T linker.ld -nostartfiles -Wl,-Map=button.map

SRCS = startup.c main.c
OBJS = $(SRCS:.c=.o)

all: button.bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

button.elf: $(OBJS) linker.ld
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

button.bin: button.elf
	$(OBJCOPY) -O binary $< $@

# Flash with OpenOCD: Writes flash, verifies, and automatically resets & runs!
flash: button.elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program button.elf verify reset exit"

clean:
	rm -f $(OBJS) button.elf button.bin button.map

.PHONY: all flash clean
