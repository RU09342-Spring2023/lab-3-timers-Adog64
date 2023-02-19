/*
 * Part2.c
 *
 *  Created on: Feb 11, 2023
 *      Author: Russell Trafford
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 */

#include <msp430.h>
#include <stdbool.h>
#include "embedded_utils.h"

#define SLOW_BLINK_PERIOD 50000
#define FAST_BLINK_PERIOD 10000

void gpioInit();
void timerInit();

long int blink_period = SLOW_BLINK_PERIOD;

void main(){

    killWatchdogTimer();

    gpioInit();
    timerInit();

    unlockGPIO();

    __bis_SR_register(LPM3_bits | GIE);

}


void gpioInit(){
    setAsOutput(1,0);
    setAsOutput(6,6);
    setAsInput(2,3);

    clearPinValue(1,0);
    clearPinValue(6,6);

    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low --> High edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled

}

void timerInit(){
    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on
    TB1CTL = TBSSEL_1 | MC_2;
    TB1CCTL0 = CCIE;
    TB1CCR0 = blink_period;
}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
void __attribute__ ((interrupt(PORT2_VECTOR))) Port_2 (void)
{
    P2IFG &= ~BIT3;

    bool risingEdge = !(P2IES & BIT3);
    if (risingEdge)
    {
        blink_period = SLOW_BLINK_PERIOD;
        setInterruptEdgeFalling(2,3);
    }
    else
    {
        blink_period = FAST_BLINK_PERIOD;
        setInterruptEdgeRising(2,3);
    }
}


// Timer B1 interrupt service routine
void __attribute__ ((interrupt(TIMER1_B0_VECTOR))) Timer1_B0_ISR (void)
{
    togglePinValue(1,0);
    TB1CCR0 += blink_period;                         // Add Offset to TB1CCR0
}


