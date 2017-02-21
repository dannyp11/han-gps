/**
 * STM32F103C8 Blink Demonstration
 *
 * Kevin Cuzner
 */

#include "stm32f1xx.h"

int main(void)
{
    //Step 1: Enable the clock to PORT B
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    //Step 2: Change PC13's mode to 0x3 (output) and cfg to 0x0 (push-pull)
    GPIOC->CRH = GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1;

    while (1)
    {
        //Step 3: Set PB0 high
        GPIOC->BSRR = GPIO_BSRR_BS13;
        for (uint16_t i = 0; i != 0xffff; i++) { }
        //Step 4: Reset PB0 low
        GPIOC->BSRR = GPIO_BSRR_BR13;
        for (uint16_t i = 0; i != 0xffff; i++) { }
    }

    return 0;
}

