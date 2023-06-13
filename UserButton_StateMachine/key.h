#ifndef __KEY_H_
#define __KEY_H_
#include "stm32l152xe.h"

/* User define constant*/
#define KEY_BUF_SIZE 10
#define KEY_RPT_DLY 2
#define KEY_RPT_START_DLY 10

#define KEY_SCAN_TASK_DLY 50

#define KEY_USER_BUTTON_CODE 1

/* Fucntion prototypes*/
void KeyFlush(void);
uint8_t KeyGetKey(uint16_t to);
uint32_t KeyGetKeyDownTime(void);
/* boolean KeyHit(void);*/
void KeyInit(void);

void KeyInitPort(void);

#endif
