//*******************************************************************//
// File: Delay.c                                                     //
// Author: Thomas Mindenhall, Jackson Moyers                         //
// Project: Backpack Buddy                                           //
//                                                                   //
//                                                                   //
// Description: Delay and Timer Functions                            //
//                                                                   //
//*******************************************************************//

///////////////////////////////////////////////////////////////////////////////
//*****************************Includes**************************************//
///////////////////////////////////////////////////////////////////////////////
#include "Delay.h"
#include "Config.h"

////////////////////////////////////////////////////////////////////////////////
//*******************************FUNCTIONS************************************//
////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 * Description: Initializes Timer 1
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Timer_1_Init(void){
    T1CONbits.ON = 0;       //disable Timer
    T1CONbits.TCKPS = 1;    //1:8 Clock Prescale
    T1CONbits.TCS = 0;      //Timer Clock Source set to internal peripheral clock
}

/******************************************************************************
 * Description: Starts Timer 1
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Timer_1_Start(void){   //turn the timer on
    T1CONbits.ON = 1;
}

/******************************************************************************
 * Description: Stops Timer 1
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Timer_1_Stop(void){    //turn the timer off
     T1CONbits.ON = 0;
}

/******************************************************************************
 * Description: Resets Timer 1 to a value
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Timer_1_Reset(void){   //resets the timer with the value of 63036
    TMR1 = 0xF63C;
}
/******************************************************************************
 * Description: Reads the Timer 1 register value
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: Value of register as an integer.
 ******************************************************************************/
int Timer_1_Read(void){
    return TMR1;
}
/******************************************************************************
 * Description: Zeros Timer 1
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Null_Timer_1 (void){
    TMR1 = 0;
}

/******************************************************************************
 * Description: Uses Timer 1 to create a processor delay in milliseconds
 * 
 * Inputs: Value as an integer (How many millis you want to delay).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Delay_ms(uint16_t ms){
    uint16_t i;
    for(i = 0; i <= ms;i++){
        IFS0bits.T1IF = 0;
        Timer_1_Reset();
        T1CONbits.ON = 1;
        while(!IFS0bits.T1IF);
    }
        
}
/* END OF FILE */