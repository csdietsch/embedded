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
#include <math.h>
#include <hal_f5529.h>

uint32_t increment;

//max of normalized X should be 1


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

void HAL_SetupPWM(float initDuty){
    TA0CCTL0 = CCIE;
    TA0CCTL1 = OUTMOD_7;
    TA0CCR0 = 999;
    TA0CTL = TASSEL_2 | MC_1; //timer control: SMCLK (1 MHz), up mode
    HAL_ChangeDutyCycle(initDuty);


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


void HAL_ChangeDutyCycle(float brightness){



   // register value that triggers interrupt cycle can go from 0 to 1000
    //hence the max is 9.96
    //expf of 9.96 is 995 or something.. closest "safe" value we can get by using expf

    //here we need to multiply
    uint32_t newBrightness = (uint32_t) expf(brightness*10); //ASSUME brightness is between 0 and 1 and hence multiply by 10 to get the proper number for expf

    TA0CCR1 = newBrightness;

}

uint32_t HAL_GetTimerValue(){
    return TA0R;
}



#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{

//    static uint8_t up = 1;
//
//    TA0CTL &= ~TAIFG;
//
//    if(TA0CCR1 >= 999){
//        TA0CCR1 = 999; //in case it has gone over the max value we want
//        up = 0;
//    }else if(TA0CCR1 == 0){
//       // TA0CCR1 = 0; //i guess this is unnecessary but it doesn't hurt Hfhjlkfdhlkjh
//        up = 1;
//    }

    //this can only go

}

