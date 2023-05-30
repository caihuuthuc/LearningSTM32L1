#include "stm32l152xe.h"  

int main (void) {
  while (1) 
    ;
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