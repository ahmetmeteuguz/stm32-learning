#include <stdint.h>

/* Symbols exported by the linker script */
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

/* Prototype for main */
int main(void);

/* Default handler for unimplemented interrupts */
void Default_Handler(void) {
    while (1);
}

/* Reset Handler: The very first C code executed by the CPU */
void Reset_Handler(void) {
    // 1. Copy initialized data from Flash to SRAM
    uint32_t *pSrc = &_sidata;
    uint32_t *pDst = &_sdata;
    while (pDst < &_edata) {
        *pDst++ = *pSrc++;
    }

    // 2. Zero-fill the .bss segment in SRAM
    uint32_t *pBss = &_sbss;
    while (pBss < &_ebss) {
        *pBss++ = 0;
    }

    // 3. Jump to application main
    main();

    // If main returns, loop forever
    while (1);
}

/* Minimal Vector Table placed in .isr_vector section */
__attribute__((section(".isr_vector")))
uint32_t *vector_table[] = {
    &_estack,               // Initial Stack Pointer
    (uint32_t *)Reset_Handler, // Reset Handler
    (uint32_t *)Default_Handler, // NMI Handler
    (uint32_t *)Default_Handler, // Hard Fault Handler
    (uint32_t *)Default_Handler, // MemManage Handler
    (uint32_t *)Default_Handler, // BusFault Handler
    (uint32_t *)Default_Handler, // UsageFault Handler
};
