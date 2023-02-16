#pragma once

#include <msp430.h>
#include <stdbool.h>

// #define OUTPUT 0x01
// #define INPUT 0x00

#define LOCATION_OF_P1DIR 0x0204
#define LOCATION_OF_P1OUT 0x0202
#define LOCATION_OF_P1IE  0x021A
#define LOCATION_OF_P1IES 0x0218

// ===== MEMORY LOCATION TABLE ======
// | PORT   | DIR   | OUT   | IE    | IES   |
// |--------|-------|-------|-------|-------|
// | P1     | 0204h | 0202h | 021Ah | 0218h |
// | P2     | 0205h | 0203h | 021Bh | 0219h |
// | P3     | 0224h | 0222h | 023Ah | 0238h |
// | P4     | 0225h | 0223h | 023Bh | 0239h |
// | P5     | 0244h | 0242h | 025Ah | 0258h |
// | P6     | 0245h | 0243h | 025Bh | 0259h |

/// @brief kill the watchdog timer
void killWatchdogTimer();

/// @brief disable low power lock on GPIO
void unlockGPIO();

/// @brief Set a pin as output P[port].[pin]
/// @param port port containing pin
/// @param pin bit index within register
void setAsOutput(char port, char pin);

/// @brief Set a pin as input P[port].[pin]
/// @param port port containing pin
/// @param pin bit index within register
void setAsInput(char port, char pin);

/// @brief Set the value of pin P[port].[pin] to logic 1
/// @param port port containing pin
/// @param pin @bit index within register
void setPinValue(char port, char pin);

/// @brief Clear the value of pin P[port].[pin] to logic 0
/// @param port port containing pin
/// @param pin @bit index within register
void clearPinValue(char port, char pin);

/// @brief Toggle the value of pin P[port].[pin]
/// @param port port containing pin
/// @param pin bit index within register
void togglePinValue(char port, char pin);

/// @brief Enable interrupts locally on pin P[port].[pin]
/// @param port port containing pin
/// @param pin bit index within register
void enablePinInterrupt(char port, char pin);

/// @brief Set the interrupt trigger edge to the rising edge
/// @param port port containing pin
/// @param pin bit index within register
void setInterruptEdgeRising(char port, char pin);

/// @brief Set the interrupt trigger edge to the falling edge
/// @param port port containing pin
/// @param pin bit index within register
void setInterruptEdgeFalling(char port, char pin);

//================= DEFINITIONS ==================

void setAsOutput(char port, char pin)
{
    char bit = BIT0 << pin;                                                 // select the bit to change by left shifting the logic 1 in BIT0 to its final spot
    port -= 1;                                                              // change from 1-indexed ports to 0-indexed ports
    char* dir = LOCATION_OF_P1DIR + ((port >> 1) << 5) + (port & 1);        // find the memory location of the selected port (3 groups of 2 consecutive memory locations; groups spaced by 20h)
    *dir |= bit;                                                            // set the bit in the memory location to logic 1
}

void setAsInput(char port, char pin)
{
    char bit = BIT0 << pin;                                                 // select the bit to change by left shifting the logic 1 in BIT0 to its final spot
    port -= 1;                                                              // change from 1-indexed ports to 0-indexed ports
    char* dir = LOCATION_OF_P1DIR + ((port >> 1) << 5) + (port & 1);        // find the memory location of the selected port (3 groups of 2 consecutive memory locations; groups spaced by 20h)
    *dir &= ~bit;                                                           // set the bit in the memory location to logic 0
}


void setPinValue(char port , char pin)
{
    char bit = BIT0 << pin;
    port -= 1;
    char* out = LOCATION_OF_P1OUT + ((port >> 1) << 5) + (port & 1);
    *out |= bit;
}

void clearPinValue(char port , char pin)
{
    char bit = BIT0 << pin;
    port -= 1;
    char* out = LOCATION_OF_P1OUT + ((port >> 1) << 5) + (port & 1);
    *out &= ~bit;
}

void togglePinValue(char port, char pin)
{
    char bit = BIT0 << pin;
    port -= 1;
    char* out = LOCATION_OF_P1OUT + ((port >> 1) << 5) + (port & 1);
    *out ^= bit;
}

void enablePinInterrupt(char port, char pin)
{
    char bit = BIT0 << pin;
    port -= 1;
    char* ie = LOCATION_OF_P1IE + ((port >> 1) << 5) + (port & 1);
    *ie |= bit;
}

void setInterruptEdgeRising(char port, char pin)
{
    char bit = BIT0 << pin;
    port -= 1;
    char* ies = LOCATION_OF_P1IES + ((port >> 1) << 5) + (port & 1);
    *ies &= ~bit;
}

void setInterruptEdgeFalling(char port, char pin)
{
    char bit = BIT0 << pin;
    port -= 1;
    char* ies = LOCATION_OF_P1IES + ((port >> 1) << 5) + (port & 1);
    *ies |= bit;
}



void killWatchdogTimer()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
}

void unlockGPIO()
{
    PM5CTL0 &= ~LOCKLPM5;
}