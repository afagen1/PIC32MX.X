//*******************************************************************//
// File: SPI.c                                                       //
// Author: Thomas Mindenhall                                         //
// Project: Backpack Buddy                                           //
//                                                                   //
//                                                                   //
// Description:  Configuration and Use of SPI Modules                //
//                                                                   //
//*******************************************************************//

#include <xc.h>
#include "CONFIG.h"
#include "SPI.h"
char rx;

void SPI1_Init(void) {
    
    ANSELBbits.ANSB15 = 0; //SS1
    ANSELBbits.ANSB14 = 0; //SCLK
    ANSELBbits.ANSB13 = 0; //SDO1
    ANSELBbits.ANSB12 = 0; //TFT_RESET
    
    //PPS SELECT
    SDI1R = 3;  
    RPB13R = 3; //SDO1 (0011)
    RPB15R = 3; //SS1 (0011)
    
    //Set the TRIS for Communication
    TRIS_SCK1 = 0;
    TRIS_SDO1 = 0;
    TRIS_SDI1 = 1;
    TRIS_SS1 = 0;

    SPI1CONbits.ON = 0;                 //Turn off SPI1 for config.
    rx = SPI1BUF;                       //Read the buffer to make clear
    SPI1BRG = 0x14;                     //1MHz Baud at 20MHz PBCLK
    SPI1CONbits.MSTEN = 1;              //Master Mode 
    SPI1CONbits.CKP = 1;                //Idle High, Drive Low
    SPI1CONbits.MCLKSEL = 0;            //PBCLK is used for Baud         
    SPI1CONbits.MSSEN = 0;              //SS control is disabled for SPI Module
    SPI1CONbits.DISSDO = 1;             //SDO control is enabled for SPI Module
    SPI1CONbits.DISSDI = 1;             //SDI control is enabled for SPI Module
    SPI2CONbits.MODE16 = 0;             //No 16bit Mode
    SPI2CONbits.MODE32 = 0;             //No 32bit Mode
    SPI1CON2bits.AUDEN = 0;             //Cleared for 8bit Mode
    SPI1CONbits.SMP = 1;                //Sample at the end
    SPI1CONbits.CKE = 1;                //Trans on active to Idle
    SPI1CONbits.ON = 1;                 //Turn on SPI Module

}

void SPI1_Close(void) {
    //turn off the SSPEN leave the rest the same
    SPI1CONbits.ON = 0;
}

void SPI1_Write(unsigned char data) {
    char dummy;
    dummy = SPI1BUF;                    // Clear the buffer
    //SSP1CON1bits.WCOL = 0;            //If we want to check for collision
    SPI1BUF = data; 
    while (!SPI1STATbits.SPIBUSY);      //While the buffer is not full wait
    dummy = SPI1BUF;                    //Read the buffer
}

unsigned char SPI1_Read(void) {
    SPI1BUF = 0x00;                     //Put something into the register
    while (!SPI1STATbits.SPIBUSY);      //Wait till our buffer is full
    return SPI1BUF;                     //Return the byte we get
}
