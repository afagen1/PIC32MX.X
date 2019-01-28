//*******************************************************************//
// File: Interrupts.c                                                //
// Author: Thomas Mindenhall                                         //
// Project: Backpack Buddy                                           //
//                                                                   //
// Compiler: XC32                                                    //
// MCU: PIC32MX170F256B                                              // 
//                                                                   //
// Description: Contains all the ISR's for the system                //
//                                                                   //
//*******************************************************************// 


#include <xc.h>
#include <sys/attribs.h>


void __ISR(_TIMER_1_VECTOR, ipl0SOFT) Timer_1_ISR(void) {
    
    //Add a pin for heartbeat
    
    IFS0bits.T1IF = 0;
}

void __ISR(_UART_1_VECTOR, ipl7SOFT) UART_1_RX_ISR(void){
    char rx;
    //Echo Code for Debugging
    if(IFS1bits.U1RXIF){
       rx = U1RXREG;
       U2TXREG = rx;
    }
    IFS1bits.U1RXIF = 0;
}

void __ISR(_UART_2_VECTOR, ipl7SOFT) UART_2_RX_ISR(void){
    char rx;
    //Add RX handler
    if(IFS1bits.U2RXIF){
        rx = U2RXREG;
        U1TXREG = rx;
    }
    IFS1bits.U2RXIF = 0;
}