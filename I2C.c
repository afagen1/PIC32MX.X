//*******************************************************************//
// File: I2C.c                                                       //
// Author: Thomas Mindenhall                                         //
// Project: Backpack Buddy                                           //
//                                                                   //
//                                                                   //
// Description: I2C Config and Use                                   //
//                                                                   //
//*******************************************************************// 

#include <proc/p32mx170f256b.h>
#include "I2C.h"
 
//Refer to I2C.h for Address Defines

//RB9 = SDA1
//RB8 = SCL1

void I2C_1_Init(void){
    
    //Set Baud Rates
    //I2C1BRG = 0x015;                //400kHz
    I2C1BRG = 0x060;                //100kHz

    I2C1CONbits.ON = 1;
    I2C1CONbits.DISSLW = 1;
    I2C1CONbits.ACKDT = 1;
    
}

void I2C_2_Init(void){
    
    //Set Baud Rates
    //I2C2BRG = 0x015;                //400kHz
    I2C2BRG = 0x060;                //100kHz

    I2C2CONbits.ON = 1;
    I2C2CONbits.DISSLW = 1;
    
    
}


char I2C_1_Read_Byte(char device_adr, char reg_adr){
    
    char rx;
    
    I2C1CONbits.SEN = 1;                //Start condition
    while (I2C1CONbits.SEN == 1);       //Wait for start to finish
    rx = I2C1TRN;                       //Make sure buffer is clear
    I2C1TRN = device_adr;               //address with R/W set for read
    while (I2C1STATbits.TRSTAT);           // wait until write cycle is complete
    I2C1TRN = reg_adr;
    while (I2C1STATbits.TRSTAT);
    I2C1TRN = device_adr + 1;           //address with R/W set for read
    while (I2C1STATbits.TRSTAT);
    I2C1CONbits.RCEN = 1;               // enable master for 1 byte reception
    while (!I2C1STATbits.RBF);          // wait until byte received
    rx = I2C1RCV;
    I2C1CONbits.ACKDT = 1;              //NACK for last byte.  Use a 0 here to ACK
    I2C1CONbits.ACKEN = 1;              //Send ACK/NACK
    while (I2C1CONbits.ACKEN);
    I2C1CONbits.PEN = 1;                //Stop condition
    while (I2C1CONbits.PEN == 1);       //Wait for stop to finish
    return rx;
}

char I2C_2_Read_Byte(char device_adr, char reg_adr){
    
    char rx;
    
    I2C2CONbits.SEN = 1;                //Start condition
    while (I2C2CONbits.SEN == 1);       //Wait for start to finish
    rx = I2C2TRN;                       //Make sure buffer is clear
    I2C2TRN = device_adr;               //address with R/W set for read
    while (I2C2STATbits.TRSTAT);           // wait until write cycle is complete
    I2C2TRN = reg_adr;
    while (I2C2STATbits.TRSTAT);
    I2C2TRN = device_adr + 1;           //address with R/W set for read
    while (I2C2STATbits.TRSTAT);
    I2C2CONbits.RCEN = 1;               // enable master for 1 byte reception
    while (!I2C2STATbits.RBF);          // wait until byte received
    rx = I2C2RCV;
    I2C2CONbits.ACKDT = 1;              //NACK for last byte.  Use a 0 here to ACK
    I2C2CONbits.ACKEN = 1;              //Send ACK/NACK
    while (I2C2CONbits.ACKEN);
    I2C2CONbits.PEN = 1;                //Stop condition
    while (I2C2CONbits.PEN == 1);       //Wait for stop to finish
    return rx;
}

void I2C_1_Write_Byte(char device_adr, char reg_adr, char value){
    char data;
    
    I2C1CONbits.SEN = 1;                //Start condition
    while (I2C1CONbits.SEN == 1);       //Wait for start to finish
    data = I2C1TRN;                       //Make sure buffer is clear
    I2C1TRN = device_adr;               //address with R/W set for read
    while (I2C1STATbits.TRSTAT);           // wait until write cycle is complete
    I2C1TRN = reg_adr;
    while (I2C1STATbits.TRSTAT);
    I2C1TRN = value;           //address with R/W set for read
    while (I2C1STATbits.TRSTAT);
    I2C1CONbits.PEN = 1;                //Stop condition
    while (I2C1CONbits.PEN == 1);       //Wait for stop to finish
}

void I2C_2_Write_Byte(char device_adr, char reg_adr, char value){
    char data;
    
    I2C2CONbits.SEN = 1;                //Start condition
    while (I2C2CONbits.SEN == 1);       //Wait for start to finish
    data = I2C2RCV;                       //Make sure buffer is clear
    I2C2TRN = device_adr;               //address with R/W set for read
    while (I2C2STATbits.TRSTAT);           // wait until write cycle is complete
    I2C2TRN = reg_adr;
    while (I2C2STATbits.TRSTAT);
    I2C2TRN = value;           //address with R/W set for read
    while (I2C2STATbits.TRSTAT);
    I2C2CONbits.PEN = 1;                //Stop condition
    while (I2C2CONbits.PEN == 1);       //Wait for stop to finish
}
/* *****************************************************************************
 End of File
 */