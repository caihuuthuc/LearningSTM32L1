#include "stm32l152xe.h"

#define LED_NUM                      (1)

volatile uint32_t msTicks;                                 // counts 1ms timeTicks

void Delay (uint32_t dlyTicks);

void SysTick_Handler(void) {
  msTicks++;
}

int main (void) {

  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000); 

  /* Configure LED (PA.5) pins as GPIO push-pull outputs, No pull-up, pull-down */
  RCC->AHBENR |=  RCC_AHBENR_GPIOAEN; /* Enable GPIOA clock         */

  /*  GPIO_MODER    : GPIO port mode register 
      These bits are written by software to configure the I/O direction mode.
          00: Input (reset state)
          01: General purpose output mode
          10: Alternate function mode
          11: Analog mode   
  */
  GPIOA->MODER  &= ~GPIO_MODER_MODER5;      /*  Reset the value of GPIOA_MODER at PA5 */
  GPIOA->MODER  |= GPIO_MODER_MODER5_0;     /*  Turn PA5 into 01 -- GPIO mode */

  /*  GPIO_OTYPR    : GPIO port output type register
      Bits 31:16 Reserved, must be kept at reset value.
      Bits 15:0 OTy: Port x configuration bits (y = 0..15)
        These bits are written by software to configure the output type of the I/O port.
          0: Output push-pull (reset state)
          1: Output open-drain
  */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;     /* Reset the value of GPIOA_OTYPR at PA5*/


  /*  GPIO_OSPEEDR  : GPIO port output speed register
      These bits are written by software to configure the I/O output speed.
        00: Low speed
        01: Medium speed
        10: High speed
        11: Very high speed
  */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR5; /* Reset the value of GPIOA_OSPEEDR at PA5 */

  /*  GPIO_PUPDR     : GPIO port pull-up/pull-down register\
      These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved
  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5;       /* Reset the value of GPIOA_PUPDR at PA5 */

  
	/* Configure Builtin Black Button at PC13 as GPIO push-pull outputs, No pull-up, pull-down */
  RCC->AHBENR |=  RCC_AHBENR_GPIOCEN;                   /* Enable GPIOC clock         */

  GPIOC->MODER   &= ~GPIO_MODER_MODER13;
  
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR13;
  
	GPIOC->PUPDR   &= ~GPIO_PUPDR_PUPDR13;

	GPIOA->BSRR = GPIO_BSRR_BS_5;
  while (1) {
    /* Turn on the PA.5 LED*/
    GPIOA->BSRR = GPIO_BSRR_BS_5;

    Delay(500);
    while (((GPIOC->IDR & GPIO_IDR_IDR_13) == 0) & (1 << 0)); // Wait while holding USER button
    /* Turn off the PA.5 LED*/
    GPIOA->BSRR &= ~GPIO_BSRR_BS_5;
    GPIOA->BSRR = GPIO_BSRR_BR_5;
    Delay(500);
    while (((GPIOC->IDR & GPIO_IDR_IDR_13) == 0) & (1 << 0)); // Wait while holding USER button

  }
}

/*----------------------------------------------------------------------------
 * Delay: delays a number of Systicks
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}

