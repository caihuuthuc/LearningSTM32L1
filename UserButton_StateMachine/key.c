#include "key.h"

#define KEY_STATE_UP 1
#define KEY_STATE_DEBOUNCE 2
#define KEY_STATE_RPT_START_DLY 3
#define KEY_STATE_RPT_DLY 4

static uint8_t KeyBuf[KEY_BUF_SIZE]; /* Keyboard buffer*/
static uint8_t KeyBufInIx;			 /* Index into key buffer where next scan code will be inserted */
static uint8_t KeyBufOutIx;			 /* Index into key buffer where next scan code will be removed */
static uint16_t KeyDownTmr;			 /* Count how long key has been pressed */
static uint8_t KeyNRead;			 /* Number of key read from the keyboard */
static uint8_t KeyRptStartDlyCtr;	 /* Number of scan times before auto repeat is started */
static uint8_t KeyRptDlyCtr;		 /* Number of scan times before auto repeat executes again */

static uint8_t KeyScanState; /* Current state of key scanning function 	*/

/* Local function prototypes */
static void KeyBufIn(uint8_t code);
/*static void KeyDecode(void);*/
static char KeyIsKeyDown(void);
static void KeyScanTask(void *data);

/*
*************************************************************************************************************************
* Insert key character into keyboard buffer
*************************************************************************************************************************
*/
static void KeyBufIn(uint8_t code)
{
	/* Enter critical section*/
	if (KeyNRead < KEY_BUF_SIZE)
	{
		KeyNRead++;
		KeyBuf[KeyBufInIx++] = code;
		if (KeyBufInIx > KEY_BUF_SIZE)
		{
			KeyBufInIx = 0;
		}
		/* Exit critical section*/
		/* Post semaphore*/
	}
	else
	{
		/* Exit critical section*/
	}
}

/*
*************************************************************************************************************************
* See if user button pressed
*************************************************************************************************************************
*/
static char KeyIsKeyDown(void)
{
	if (((GPIOC->IDR & GPIO_IDR_IDR_13) == 0) & (1 << 0))
	{ /* User button not pressed if 0															 */
		/* Enter critical section*/
		KeyDownTmr++; /* Update key down timer 																		 */
		/* Exit critical section*/
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*************************************************************************************************************************
* Keyboard initialization
*************************************************************************************************************************
*/
void KeyInit(void)
{
	KeyBufInIx = 0;
	KeyBufOutIx = 0;
	KeyDownTmr = 0;
	/* Create Semaphore*/
	KeyInitPort();
	/* Create rtos task*/
}
/*
*************************************************************************************************************************
* Keyboard scanning task
*************************************************************************************************************************
*/
static void KeyScanTask(void *data)
{
	for (;;)
	{
		/* Delay KEY_SCAN_TASK_DLY between user button scans*/
		switch (KeyScanState)
		{

		case KEY_STATE_UP:
			if (KeyIsKeyDown())
			{
				KeyScanState = KEY_STATE_DEBOUNCE;
				KeyDownTmr = 0;
			}
			break;

		case KEY_STATE_DEBOUNCE:
			if (KeyIsKeyDown())
			{
				KeyBufIn(KEY_USER_BUTTON_CODE);
				KeyRptStartDlyCtr = KEY_RPT_START_DLY; /* Start delay to auto-repeat function */
				KeyScanState = KEY_STATE_RPT_START_DLY;
			}
			else
			{
				KeyScanState = KEY_STATE_UP;
			}
			break;

		case KEY_STATE_RPT_START_DLY:
			if (KeyIsKeyDown())
			{
			}
			else
			{
				KeyScanState = KEY_STATE_DEBOUNCE;
			}
			break;

		case KEY_STATE_RPT_DLY:
			if (KeyIsKeyDown())
			{
			}
			else
			{
				KeyScanState = KEY_STATE_DEBOUNCE;
			}
			break;
		}
	}
}

void KeyInitPort(void)
{
	/* Configure Builtin Black Button at PC13 as GPIO push-pull outputs, No pull-up, pull-down */
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	GPIOC->MODER &= ~GPIO_MODER_MODER13;

	GPIOC->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR13;

	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;

	GPIOA->BSRR = GPIO_BSRR_BS_5;
}
