CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

CPU_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS    = $(CPU_FLAGS) -Wall -Wextra -g -O0 -std=c99 -I../CMSIS
LDFLAGS   = $(CPU_FLAGS) -T linker.ld -nostartfiles -Wl,-Map=counter.map

SRCS = startup.c main.c
OBJS = $(SRCS:.c=.o)

all: counter.bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

counter.elf: $(OBJS) linker.ld
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

counter.bin: counter.elf
	$(OBJCOPY) -O binary $< $@

flash: counter.elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program counter.elf verify reset exit"

clean:
	rm -f $(OBJS) counter.elf counter.bin counter.map

.PHONY: all flash clean
