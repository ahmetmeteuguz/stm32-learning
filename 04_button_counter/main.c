#include "stm32f446xx.h"

static void delay(uint32_t count)
{
    while(count--){
        __asm__("nop");
    }
}

int main(void){

    /*Enable GPIOA clock*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
    
    /*Configure pins*/
    GPIOA->MODER |= GPIO_MODER_MODE5_0; // Set PA5 as output (LED)
    GPIOC->MODER &= ~GPIO_MODER_MODE13; // Set PC13 as input (Button)
    GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_0; // Enable pull-up resistor for PC13

    uint8_t last_button_state = 1; // Initialize last button state
    uint8_t press_count = 0; // Initialize press count

    while(1){

        uint8_t current_button_state = (GPIOC->IDR & GPIO_IDR_ID13) ? 1 : 0; // Read current button state (1 if not pressed, 0 if pressed)
        uint32_t button_pressed = 0;
        if(last_button_state == 1 && current_button_state == 0){
            press_count++;
            button_pressed = press_count;
        }

        last_button_state = current_button_state;

        // Optional: Add a small delay to debounce the button
        delay(10000);

        
        for (int i = 0; i < button_pressed; i++) {
            GPIOA->BSRR = GPIO_BSRR_BS5; // Toggle LED
            delay(500000); // Delay for visibility
            GPIOA->BSRR = ~GPIO_BSRR_BS5; // Turn off LED
            delay(500000); // Delay for visibility
        }

    }
}