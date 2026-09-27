#include <stdint.h>
/* =========================================================================
 * Base Addresses (from STM32F446 Reference Manual RM0390)
 * ========================================================================= */
#define RCC_BASE      (0x40023800UL)
#define GPIOA_BASE    (0x40020000UL)
#define GPIOC_BASE    (0x40020800UL)  // GPIOC starts at offset 0x0800 in AHB1

/* RCC Register */
#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x30))

/* Clock Enable Bits in RCC->AHB1ENR */
#define RCC_GPIOAEN   (1U << 0)  // Bit 0: Port A clock
#define RCC_GPIOCEN   (1U << 2)  // Bit 2: Port C clock

/* GPIO Registers Layout */
typedef struct {
    volatile uint32_t MODER;    // 0x00: Mode (00=Input, 01=Output)
    volatile uint32_t OTYPER;   // 0x04: Output Type
    volatile uint32_t OSPEEDR;  // 0x08: Output Speed
    volatile uint32_t PUPDR;    // 0x0C: Pull-up / Pull-down
    volatile uint32_t IDR;      // 0x10: Input Data Register (read physical pin state)
    volatile uint32_t ODR;      // 0x14: Output Data Register
    volatile uint32_t BSRR;     // 0x18: Bit Set/Reset Register (atomic write)
    volatile uint32_t LCKR;     // 0x1C: Configuration Lock
    volatile uint32_t AFR[2];   // 0x20: Alternate Functions
} GPIO_TypeDef;

#define GPIOA         ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOC         ((GPIO_TypeDef *) GPIOC_BASE)

/* Pin Definitions */
#define LED_PIN       (5)   // PA5: Green LED (LD2)
#define BUTTON_PIN    (13)  // PC13: Blue User Button (B1)

int main(void){
    /* 1. ENABLE CLOCKS: Power on both GPIOA and GPIOC */
    RCC_AHB1ENR |= RCC_GPIOAEN | RCC_GPIOCEN;

    /* 2. CONFIGURE PA5 AS OUTPUT (LED)
     * Clear bits 11:10, then set to 01 (General purpose output)
     */
    GPIOA->MODER &= ~(3U << (LED_PIN * 2));
    GPIOA->MODER |=  (1U << (LED_PIN * 2));

    /* 3. CONFIGURE PC13 AS INPUT (Button)
     * Pin 13 uses bits [27:26]. Mode 00 = Input.
     * We only need to clear bits 27:26 to 0.
     */
    GPIOC->MODER &= ~(3U << (BUTTON_PIN * 2));

    /* 4. SUPER LOOP: Poll the button and update the LED */

    uint8_t led_state = 0; // Variable to hold the LED state (0=OFF, 1=ON)
    uint8_t last_button_state = 1; // Variable to hold the button state (1=RELEASED, 0=PRESSED)

    while (1) {
        
        uint8_t current_button_state;

        if(GPIOC->IDR & (1U << BUTTON_PIN)) {
            current_button_state = 1; // Button is RELEASED
        } else {
            current_button_state = 0; // Button is PRESSED
        }

        if (current_button_state == 0 && last_button_state == 1) {
            // Button was just pressed, toggle the LED state
            led_state ^= 1; // Toggle LED state
        }
        
        last_button_state = current_button_state;

        if (led_state) {
            // Turn LED ON
            GPIOA->BSRR = (1U << LED_PIN);
        } else {
            // Turn LED OFF
            GPIOA->BSRR = (1U << (LED_PIN + 16));
        }
    }
}

