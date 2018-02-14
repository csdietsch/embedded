#include <msp430.h> 
#include <stdio.h>
#include <stdint.h>
#include <hal_f5529.h>
#include <stdbool.h>



/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


    uint32_t u;
    printf("Enter a number: ");
    scanf('%d', &u);
    printf("%d");
    float dutyCycle = .2;

    HAL_SetupLED();
    HAL_SetupPWM(dutyCycle);


   uint8_t up = 1;





    return 0;
}


