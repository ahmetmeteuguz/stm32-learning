# STM32 Bare-Metal Learning

Hands-on embedded programming projects on the **STM32 NUCLEO-F446RE** board (ARM Cortex-M4), built from first principles without auto-generated boilerplate.

## Board Specifications
* **MCU**: STM32F446RE (ARM Cortex-M4 with FPU @ up to 180 MHz)
* **Flash**: 512 KB
* **SRAM**: 128 KB
* **Debugger**: On-board ST-LINK/V2-1

## Projects
- `01_baremetal_blink`: Pure register-level LED blink with custom linker script (`linker.ld`), minimal startup file (`startup.c`), memory-mapped register definitions, and a GNU `Makefile`.
- `02_gpio_button`: Polling input pin (PC13 blue button) using `IDR` register and controlling output pin (PA5 green LED) using atomic `BSRR` register writes.
- `03_button_toggle`: T-Flip-Flop button toggle using edge detection (falling edge on active-low input) and software switch debouncing.
- `04_button_counter`: Multi-blink counter using official ARM/ST CMSIS headers (`stm32f446xx.h`).

## Documentation
- `STM32F446RE_Cheatsheet.md`: Pocket datasheet reference with base addresses, register offsets, bit recipes, and pin mappings.

## Build & Flash
Navigate to any project directory and run:
```bash
make
make flash
```
