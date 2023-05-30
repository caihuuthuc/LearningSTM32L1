#include "stm32l152xe.h"
#include "stm32l1xx.h"

#define LED_NUM                      (1)

const unsigned long led_mask[] = {1ul << 5};

volatile uint32_t msTicks;                                 // counts 1ms timeTicks

void SystemCoreClockConfigure(void);
void SysTick_Handler(void);
void Delay (uint32_t dlyTicks);

int32_t  LED_Initialize   (void);
int32_t  LED_Uninitialize (void);
int32_t  LED_On           (uint32_t num);
int32_t  LED_Off          (uint32_t num);
int32_t  LED_SetOut       (uint32_t val);
uint32_t LED_GetCount     (void);


int32_t  Buttons_Initialize   (void);
int32_t  Buttons_Uninitialize (void);
uint32_t Buttons_GetState     (void);
uint32_t Buttons_GetCount     (void);


void SysTick_Handler(void) {
  msTicks++;
}

int main() {
  int32_t max_num = LED_GetCount();
  int32_t num = 0;

  SystemCoreClockConfigure();                              // configure HSI as System Clock
  SystemCoreClockUpdate();

  LED_Initialize();
  Buttons_Initialize();

  SysTick_Config(SystemCoreClock / 1000);                  // SysTick 1 msec interrupts
  LED_On(num);                                           // Turn specified LED on
  Delay(500);  
  
  while (1);

  // for (;;) {
  //   LED_On(num);                                           // Turn specified LED on
  //   Delay(500);                                            // Wait 500ms
  //   while (Buttons_GetState() & (1 << 0));                 // Wait while holding USER button
  //   LED_Off(num);                                          // Turn specified LED off
  //   Delay(500);                                            // Wait 500ms
  //   while (Buttons_GetState() & (1 << 0));                 // Wait while holding USER button

  //   num++;                                                 // Change LED number
  //   if (num >= max_num) {
  //     num = 0;                                             // Restart with first LED
  //   }
  // }

}

/*----------------------------------------------------------------------------
 * Delay: delays a number of Systicks
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}


/*----------------------------------------------------------------------------
 * SystemCoreClockConfigure: configure SystemCoreClock using HSI
                             (HSE is not populated on Nucleo board)
 *----------------------------------------------------------------------------*/
void SystemCoreClockConfigure(void) {

  RCC->CR |= ((uint32_t)RCC_CR_HSION);                     // Enable HSI
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                  // Wait for HSI Ready

  RCC->CFGR = RCC_CFGR_SW_HSI;                             // HSI is system clock
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);  // Wait for HSI used as system clock

  // PLL configuration: PLLCLK = (HSI * 6)/3 = 32 MHz
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC |
                 RCC_CFGR_PLLMUL |
                 RCC_CFGR_PLLDIV  );
  RCC->CFGR |=  (RCC_CFGR_PLLSRC_HSI |
                 RCC_CFGR_PLLMUL4    |
                 RCC_CFGR_PLLDIV2     );

  FLASH->ACR |= FLASH_ACR_ACC64;                           // Enable 64-bit access
  FLASH->ACR |= FLASH_ACR_PRFTEN;                          // Enable Prefetch Buffer
  FLASH->ACR |= FLASH_ACR_LATENCY;                         // Flash 1 wait state

  RCC->APB1ENR |= RCC_APB1ENR_PWREN;                       // Enable the PWR APB1 Clock
  PWR->CR = PWR_CR_VOS_0;                                  // Select the Voltage Range 1 (1.8V)
  while((PWR->CSR & PWR_CSR_VOSF) != 0);                   // Wait for Voltage Regulator Ready

  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                         // HCLK = SYSCLK
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;                        // PCLK1 = HCLK
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;                        // PCLK2 = HCLK

  RCC->CR &= ~RCC_CR_PLLON;                                // Disable PLL

  RCC->CR |= RCC_CR_PLLON;                                 // Enable PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();           // Wait till PLL is ready

  RCC->CFGR &= ~RCC_CFGR_SW;                               // Select PLL as system clock source
  RCC->CFGR |=  RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // Wait till PLL is system clock src
}

// int32_t LED_Initialize (void) {

//   RCC->AHBENR |=  RCC_AHBENR_GPIOAEN;                   /* Enable GPIOA clock         */

//   /* Configure LED (PA.5) pins as push-pull outputs, No pull-up, pull-down */
//   GPIOA->MODER   &= ~((3ul << 2*5));
//   GPIOA->MODER   |=  ((1ul << 2*5));
//   GPIOA->OTYPER  &= ~((1ul <<   5));
//   GPIOA->OSPEEDR &= ~((3ul << 2*5));
//   GPIOA->OSPEEDR |=  ((1ul << 2*5));
//   GPIOA->PUPDR   &= ~((3ul << 2*5));
//   return (0);
// }

// int32_t LED_On (uint32_t num) {
//   int32_t retCode = 0;
//   if (num < LED_NUM) {
//     GPIOA->BSRR = MASK_PORT5;
//   } else {
//     retCode = -1;
//   }
//   return retCode;
// }

// int32_t LED_Off (uint32_t num) {
//   int32_t retCode = 0;
//   if (num < LED_NUM) {
//     GPIOA->BSRR &= ~MASK_PORT5;
//   } else {
//     retCode = -1;
//   }
//   return retCode;
// }


/**
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Initialize (void) {

  RCC->AHBENR |=  (1ul << 0);                   /* Enable GPIOA clock         */

  /* Configure LED (PA.5) pins as push-pull outputs, No pull-up, pull-down */
  GPIOA->MODER   &= ~((3ul << 2*5));
  GPIOA->MODER   |=  ((1ul << 2*5));
  GPIOA->OTYPER  &= ~((1ul <<   5));
  GPIOA->OSPEEDR &= ~((3ul << 2*5));
  GPIOA->OSPEEDR |=  ((1ul << 2*5));
  GPIOA->PUPDR   &= ~((3ul << 2*5));
  return (0);
}


/**
  \fn          int32_t LED_Uninitialize (void)
  \brief       De-initialize I/O interface for LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Uninitialize (void) {

  return (0);
}


/**
  \fn          int32_t LED_On (uint32_t num)
  \brief       Turn on requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_On (uint32_t num) {
  int32_t retCode = 0;
  if (num < LED_NUM) {
    GPIOA->BSRR = (led_mask[num]);
  } else {
    retCode = -1;
  }
  return retCode;
}


/**
  \fn          int32_t LED_Off (uint32_t num)
  \brief       Turn off requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Off (uint32_t num) {
  int32_t retCode = 0;
  if (num < LED_NUM) {
    GPIOA->BSRR = (led_mask[num] << 16);
  } else {
    retCode = -1;
  }
  return retCode;
}


/**
  \fn          int32_t LED_SetOut (uint32_t val)
  \brief       Write value to LEDs
  \param[in]   val  value to be displayed on LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_SetOut (uint32_t val) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (val & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
  return (0);
}


/**
  \fn          uint32_t LED_GetCount (void)
  \brief       Get number of LEDs
  \return      Number of available LEDs
*/
uint32_t LED_GetCount (void) {
  return LED_NUM;
}


#define NUM_KEYS  1                     /* Number of available keys           */

/* Keys for NUCLEO Board */
#define USER    1


/**
  \fn          int32_t Buttons_Initialize (void)
  \brief       Initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Initialize (void) {

  RCC->AHBENR |=  (1ul << 2);                   /* Enable GPIOC clock         */

  GPIOC->MODER   &= ~(3ul << 2*13);
  GPIOC->OSPEEDR &= ~(3ul << 2*13);
  GPIOC->OSPEEDR |=  (1ul << 2*13);
  GPIOC->PUPDR   &= ~(3ul << 2*13);
  return (0);
}


/**
  \fn          int32_t Buttons_Uninitialize (void)
  \brief       De-initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Uninitialize (void) {

  GPIOC->MODER &= ~(3ul << 2*13);
  return (0);
}


/**
  \fn          uint32_t Buttons_GetState (void)
  \brief       Get buttons state
  \returns     Buttons state
*/
uint32_t Buttons_GetState (void) {

  uint32_t val = 0;

  if ((GPIOC->IDR & (1ul << 13)) == 0) {
    /* USER button */
    val |= USER;
  }

  return (val);
}


/**
  \fn          uint32_t Buttons_GetCount (void)
  \brief       Get number of available buttons
  \return      Number of available buttons
*/
uint32_t Buttons_GetCount (void) {
  return (NUM_KEYS);
}


