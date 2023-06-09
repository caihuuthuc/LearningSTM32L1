#include "stm32l152xe.h"

/* counts 1ms timeTicks */
volatile uint32_t msTicks;

void Delay(uint32_t dlyTicks);

int usart_putchar(char c);
char usart_getchar(void);

int main(void)
{
	SystemCoreClockUpdate(); /* default clock: 2MHz */
	SysTick_Config(SystemCoreClock / 1000);

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; /* Enable GPIOA clock         */

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; /* Enable USART1 clock         */

	/* Configure PA9 as USART TX -- AF USART1 -- Push-pull outputs, pull-up*/

	GPIOA->MODER &= ~GPIO_MODER_MODER9;	 /*  Reset the value of GPIOA_MODER at PA9 */
	GPIOA->MODER |= GPIO_MODER_MODER9_1; /*  Turn PA9 into 10 -- AF mode */

	GPIOA->AFR[1] = 0;
	GPIOA->AFR[1] |= (7 << 4); /* bit (7:6:5:4)=(0:1:1:1) -- AF7 Alternate function for USART1 at pin PA9*/
	GPIOA->AFR[1] |= (7 << 8); /* bit (11:10:9:8)=(0:1:1:1) -- AF7 Alternate function for USART1 at pin PA10*/

	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9; /* Reset the value of GPIOA_OTYPR at PA9*/

	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR9; /* Reset the value of GPIOA_OSPEEDR at PA9 */

	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;	 /* Reset the value of GPIOA_PUPDR at PA9 */
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR9_0; /* Pull-up mode*/

	/* Configure PA10 as USART RX -- AF USART1 -- Push-pull outputs, pull-up*/

	GPIOA->MODER &= ~GPIO_MODER_MODER10;  /*  Reset the value of GPIOA_MODER at PA10 */
	GPIOA->MODER |= GPIO_MODER_MODER10_1; /*  Turn PA10 into 10 -- AF mode */

	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_10; /* Reset the value of GPIOA_OTYPR at PA10*/

	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR10; /* Reset the value of GPIOA_OSPEEDR at PA10 */

	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10;  /* Reset the value of GPIOA_PUPDR at PA10 */
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR10_0; /* Pull-up mode*/

	/* USART configuration */
	USART1->CR1 &= ~USART_CR1_UE; /* Disable the USART */
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;

	USART1->BRR = 0;
	USART1->BRR |= (26U << 4); /* CLOCK = 2MHz, baud rate is 9600, MENTISSA = 26, FRACTION = 0*/

	USART1->CR1 &= ~USART_CR1_M; /* 8bit mode -- Force reset M bit in CR1 register*/

	USART1->CR1 &= ~USART_CR1_PCE; /* Disable parity mode -- Force reset PCE bit in CR1 register*/

	USART1->CR2 &= ~USART_CR2_STOP; /* 1 stop bit -- force reset*/

	USART1->CR3 &= ~USART_CR3_CTSE; /* Disable CTS -- Force reset CTS bit in CR3 register*/

	USART1->CR3 &= ~USART_CR3_RTSE; /* Disable RTS -- Force reset RTS bit in CR3 register*/

	USART1->CR1 |= USART_CR1_UE; /* Enable the USART */

	for (;;)
	{

		usart_putchar((char)1);
		Delay(50);
	}
}

int usart_putchar(char c)
{
	USART1->DR = c;
	while (!(USART1->SR & USART_SR_TXE))
		;

	return 0;
}

char usart_getchar(void)
{
	char temp;

	while (!(USART1->SR & USART_SR_RXNE))
		;
	temp = (char)USART1->DR;
	return temp;
}

void Delay(uint32_t dlyTicks)
{
	uint32_t curTicks;

	curTicks = msTicks;
	while ((msTicks - curTicks) < dlyTicks)
	{
		__NOP();
	}
}
