/*
 * hal_f5529.h
 *
 *  Created on: Feb 8, 2018
 *      Author: Chris
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef HAL_F5529_H_
#define HAL_F5529_H_


void HAL_SetupLED();
void HAL_SetupTimer();
void HAL_SetupInterrupt();
void HAL_ToggleLED();
void HAL_SetupPWM(float initDuty);
void HAL_ChangeDutyCycle(float brightness);
void HAL_InitUART();
char HAL_GetUARTChar();
void HAL_PutUARTChar(char c);
uint32_t HAL_GetTimerValue();
void HAL_IncrementBrightness();
void HAL_DecrementBrightness();


#endif /* HAL_F5529_H_ */
