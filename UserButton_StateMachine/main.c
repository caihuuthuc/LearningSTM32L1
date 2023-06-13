#include "key.h"

/* counts 1ms timeTicks */
volatile uint32_t msTicks;             

void Delay (uint32_t dlyTicks);
void SysTick_Handler(void);

int main (void) {
	SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000); 
	KeyInit();
	
	for (;;) {
	
	}
}




void SysTick_Handler(void) {
  msTicks++;
}

void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}


// Handle assertion violations in the firmware library
#ifdef  USE_FULL_ASSERT
void assert_failed (uint8_t * file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we are here*/
  while (1)
    ;
}
#endif