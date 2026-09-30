# STM32F446RE Quick Reference (Pocket Datasheet)

## 1. Board Hardware Map (Nucleo-64 UM1724)
| Hardware Feature | STM32 Pin | Logic Level | Electrical Behavior |
| :--- | :--- | :--- | :--- |
| **User LED (LD2)** | **PA5** | Active-HIGH | Write `1` $\rightarrow$ 3.3V (ON), Write `0` $\rightarrow$ 0V (OFF) |
| **User Button (B1)** | **PC13** | Active-LOW | Released $\rightarrow$ Pull-up reads `1`, Pressed $\rightarrow$ Reads `0` |
| **Reset Button (B2)**| **NRST** | Active-LOW | Restarts MCU from address `0x08000000` |

---

## 2. Bus & Peripheral Base Addresses (RM0390 Table 1)
All peripherals live in the `0x4000 0000` to `0x5FFF FFFF` memory range.

| Bus | Base Address | Peripherals Attached |
| :--- | :--- | :--- |
| **AHB1** | `0x4002 0000` | GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOH, RCC, DMA1, DMA2 |
| **APB1** | `0x4000 0000` | TIM2-TIM7, TIM12-TIM14, USART2, USART3, UART4, UART5, I2C1-I2C3 |
| **APB2** | `0x4001 0000` | TIM1, TIM8, USART1, USART6, ADC1-ADC3, EXTI, SYSCFG, SPI1, SPI4 |

### Specific Peripheral Bases:
* **GPIOA**: `0x4002 0000`
* **GPIOB**: `0x4002 0400`
* **GPIOC**: `0x4002 0800`
* **RCC** (Reset & Clock Control): `0x4002 3800`

---

## 3. Clock Control: RCC AHB1ENR Register
* **Register Address**: `RCC_BASE + 0x30` = `0x4002 3830`
* **Purpose**: Must enable the clock before the peripheral can respond.

| Bit | Name | Peripheral Controlled | C Code to Enable |
| :--- | :--- | :--- | :--- |
| **Bit 0** | `GPIOAEN` | GPIO Port A | `RCC->AHB1ENR \|= (1U << 0);` |
| **Bit 1** | `GPIOBEN` | GPIO Port B | `RCC->AHB1ENR \|= (1U << 1);` |
| **Bit 2** | `GPIOCEN` | GPIO Port C | `RCC->AHB1ENR \|= (1U << 2);` |
| **Bit 3** | `GPIODEN` | GPIO Port D | `RCC->AHB1ENR \|= (1U << 3);` |

---

## 4. GPIO Registers Layout (RM0390 Section 8.4)
Every GPIO port (A, B, C...) shares the exact same internal register layout:

| Register | Offset | Access | Purpose |
| :--- | :--- | :--- | :--- |
| `MODER` | `+0x00` | Read/Write | Mode (2 bits per pin): `00`=Input, `01`=Output, `10`=AF, `11`=Analog |
| `OTYPER` | `+0x04` | Read/Write | Output type: `0`=Push-Pull, `1`=Open-Drain |
| `OSPEEDR`| `+0x08` | Read/Write | Speed: `00`=Low, `01`=Medium, `10`=Fast, `11`=High |
| `PUPDR` | `+0x0C` | Read/Write | Resistors: `00`=None, `01`=Pull-up, `10`=Pull-down |
| `IDR` | `+0x10` | Read-only | **Input Data Register**: Read physical pin voltage (1 bit per pin) |
| `ODR` | `+0x14` | Read/Write | **Output Data Register**: Write pin voltage (1 bit per pin) |
| `BSRR` | `+0x18` | Write-only | **Bit Set/Reset**: Bits 0-15 = Atomic Set, Bits 16-31 = Atomic Reset |

---

## 5. Common Register Bit Recipes

### A. Set Pin as Output (`MODER`)
```c
// Pin y uses bits [(2y+1) : 2y]
GPIOx->MODER &= ~(3U << (PIN * 2)); // Clear 2 bits
GPIOx->MODER |=  (1U << (PIN * 2)); // Set to 01 (Output)
```

### B. Set Pin as Input (`MODER`)
```c
GPIOx->MODER &= ~(3U << (PIN * 2)); // Clear 2 bits to 00 (Input)
```

### C. Read Pin (`IDR`)
```c
uint8_t state = (GPIOx->IDR & (1U << PIN)) ? 1 : 0;
```

### D. Atomic Set & Reset (`BSRR`)
```c
GPIOx->BSRR = (1U << PIN);        // Set HIGH (Atomic 1)
GPIOx->BSRR = (1U << (PIN + 16)); // Set LOW (Atomic 0)
```
