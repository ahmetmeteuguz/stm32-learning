/* Linker script for STM32F446RE (512KB Flash, 128KB RAM) */

ENTRY(Reset_Handler)

/* End of Stack: top of RAM (ARM Cortex stack grows downwards) */
_estack = ORIGIN(RAM) + LENGTH(RAM);

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    /* The vector table must be placed at the very start of Flash */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* Program code and constants */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;  /* End of code in Flash */
    } > FLASH

    /* Used by the startup to initialize data:
       _sidata is the load address in Flash where initial values of .data live */
    _sidata = LOADADDR(.data);

    /* Initialized data goes into RAM, but loaded from Flash */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;  /* Start of .data in RAM */
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;  /* End of .data in RAM */
    } > RAM AT > FLASH

    /* Uninitialized data (zero-initialized) stays only in RAM */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;   /* Start of .bss in RAM */
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;   /* End of .bss in RAM */
    } > RAM
}
