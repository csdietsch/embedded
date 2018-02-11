/*
 * hal_f5529.c
 *
 *  Created on: Feb 8, 2018
 *      Author: csdietsch
 */

#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>




void HAL_SetupLED()
{
    P1DIR = 0xFF;
    P1REN = 0xFF;
    P1OUT = 0x00;
    P1SEL = 0xFF;
}

void HAL_SetupTimer(){
    TA0CTL = TASSEL_2 + MC_2; //timer control: SMCLK (1 MHz), up mode
}

void HAL_SetupInterrupt(){
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 | MC_2; //timer control: SMCLK (1 MHz), up mode
    TA0CCR0 = 50000;
}

void HAL_SetupPWM(uint32_t initDuty){
    TA0CCTL0 = CCIE;
    TA0CCTL1 = OUTMOD_7;
    TA0CCR0 = 999;
    TA0CTL = TASSEL_2 | MC_1; //timer control: SMCLK (1 MHz), up mode
    TA0CCR1 = initDuty; //start as 50%


    _bis_SR_register(LPM0_bits + GIE); //LPM3 - Global Interrupt Enabled


}

void HAL_ToggleLED(){

    P1OUT ^= BIT2;

}

void HAL_LEDOn(){

    P1OUT |= BIT2;

}

void HAL_LEDOff(){

    P1OUT &= ~(BIT0);

}

void HAL_ChangeDutyCycle(uint32_t brightness){

    TA0CCR1 = brightness;

}

uint32_t HAL_GetTimerValue(){
    return TA0R;
}



#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{

    static uint8_t up = 1;

    TA0CTL &= ~TAIFG;

    if(TA0CCR1 >= 999){
        up = 0;
    }else if(TA0CCR1 == 0){
        up = 1;
    }

    if(up == 1){

        TA0CCR1++;

    }else{
        TA0CCR1--;
    }

}

