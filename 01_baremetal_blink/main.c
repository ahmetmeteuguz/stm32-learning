#include <stdint.h>

/* =========================================================================
 * Hardware Register Addresses (from STM32F446xx Reference Manual RM0390)
 * =========================================================================
 * Base addresses:
 *   Peripherals Base:       0x40000000
 *   AHB1 Bus Base:          0x40020000
 *   GPIOA Base:             0x40020000
 *   RCC (Clock Ctrl) Base:  0x40023800
 */

#define RCC_BASE      (0x40023800UL)
#define GPIOA_BASE    (0x40020000UL)

/* RCC AHB1 Enable Register: Offset 0x30 */
#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_GPIOAEN   (1U << 0)  // Bit 0 enables GPIO Port A clock

/* Structure representing the memory layout of a STM32 GPIO Port */
typedef struct {
    volatile uint32_t MODER;    // 0x00: Mode register (Input, Output, AF, Analog)
    volatile uint32_t OTYPER;   // 0x04: Output type register (Push-Pull / Open-Drain)
    volatile uint32_t OSPEEDR;  // 0x08: Output speed register
    volatile uint32_t PUPDR;    // 0x0C: Pull-up / Pull-down register
    volatile uint32_t IDR;      // 0x10: Input data register
    volatile uint32_t ODR;      // 0x14: Output data register
    volatile uint32_t BSRR;     // 0x18: Bit set/reset register (atomic)
    volatile uint32_t LCKR;     // 0x1C: Configuration lock register
    volatile uint32_t AFR[2];   // 0x20-0x24: Alternate function registers
} GPIO_TypeDef;

/* Cast GPIOA_BASE address to a pointer to our GPIO struct */
#define GPIOA         ((GPIO_TypeDef *) GPIOA_BASE)

/* Pin 5 definitions for LD2 (Green LED on Nucleo-F446RE) */
#define LED_PIN       (5)

static void delay(volatile uint32_t count) {
    while (count--) {
        // Prevent compiler from optimizing away the loop
        __asm__("nop");
    }
}

int main(void) {
    /* 1. ENABLE CLOCK: Power on GPIOA by setting bit 0 in RCC_AHB1ENR */
    RCC_AHB1ENR |= RCC_GPIOAEN;

    /* 2. CONFIGURE PIN 5 AS OUTPUT:
     * MODER has 2 bits per pin. For Pin 5, bits are [11:10].
     * 00 = Input, 01 = Output, 10 = Alternate Function, 11 = Analog.
     * We clear bits [11:10] and then set bit 10 to 1.
     */
    GPIOA->MODER &= ~(0x3U << (LED_PIN * 2)); // Clear mode bits 11:10
    GPIOA->MODER |=  (0x1U << (LED_PIN * 2)); // Set bit 10 (01 = Output)

    /* 3. SUPER LOOP: Toggle LED */
    while (1) {
        // Toggle Pin 5 in the Output Data Register (ODR)
        GPIOA->ODR ^= (1U << LED_PIN);

        // Crude delay (~500ms at default 16MHz internal HSI clock)
        delay(500000);
    }

    return 0;
}
