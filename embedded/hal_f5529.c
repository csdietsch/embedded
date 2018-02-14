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

float currentBrightness;

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


    _bis_SR_register(LPM3_bits + GIE); //LPM3 - Global Interrupt Enabled


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

    currentBrightness = brightness;

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

void HAL_InitUART(){
    P4SEL = BIT4+BIT5;
    UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
     UCA1CTL1 |= UCSSEL_1;                     // CLK = ACLK
     UCA1BR0 = 0x03;                           // 32kHz/9600=3.41 (see User's Guide)
     UCA1BR1 = 0x00;                           //
     UCA1MCTL = UCBRS_3+UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0
     UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
     UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

    // __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, interrupts enabled
     __no_operation();                         // For debugger
}


char HAL_GetUARTChar(){
    if (UCA1IFG & UCTXIFG) { //if the UART interrupt flag is raised
        return UCA1RXBUF; //then return the value of the receive buffer
    }
}


void HAL_PutUARTChar(char c){
        while (!(UCA1IFG & UCTXIFG)); //wait until UART is not transmitting
        UCA1TXBUF = c; //place the passed character into the transmit buffer
        return 0;
}

void HAL_IncrementBrightness(){

    HAL_ChangeDutyCycle(currentBrightness + .1);

}

void HAL_DecrementBrightness(){

    HAL_ChangeDutyCycle(currentBrightness - .1);

}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
 //nop

}

//#pragma vector=USCI_A0_VECTOR
//__interrupt void USCI_A0_ISR(void)
//{
//  switch(__even_in_range(UCA0IV,4))
//  {
//  case 0:break;                             // Vector 0 - no interrupt
//  case 2:                                   // Vector 2 - RXIFG
//      while (!(UCA0IFG&UCTXIFG));             // USCI_A1 TX buffer ready?
//         UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
//         break;
//  case 4:break;                             // Vector 4 - TXIFG
//  default: break;
//  }
//}


/**
 *
 *
 int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  P3SEL = BIT3+BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD

  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
  UCA0BR0 = 0x0D;                           // 2400 (see User's Guide)
  UCA0BR1 = 0x00;                           //
  UCA0MCTL |= UCBRS_6+UCBRF_0;              // Modulation UCBRSx=6, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, interrupts enabled
  __no_operation();                         // For debugger
}

 */

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA1IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA1TXBUF = UCA1RXBUF;                  // TX -> RXed character
    if(UCA1RXBUF == '+'){
        HAL_IncrementBrightness();
    }else if(UCA1RXBUF == '-'){
        HAL_DecrementBrightness();
    }
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}



