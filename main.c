//*******************************************************************//
// File: main.c                                                      //
// Author: Thomas Mindenhall                                         //
// Project: QFC32 Flight Controller                                  //
//                                                                   //
//                                                                   //
// Description: Main program                                         //
//                                                                   //
//*******************************************************************// 

//MCU: PIC32MX270F256B 
//Compiler: XC32 1.44
//Harmony v1_07_01

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Config.h"
#include "EUSART.h"
#include "I2C.h"
#include "ILI9341.h"
#include "SPI.h"

//Globals
int acc_x, acc_y, acc_z;
int gyr_x, gyr_y, gyr_z;
int mag_x, mag_y, mag_z;
int gravity_x, gravity_y, gravity_z;
int linear_acc_x, linear_acc_y, linear_acc_z;

char buffer_1 [40];

//Globals for filtering
int last_acc_x, last_acc_y, last_acc_z;
int last_gyr_x, last_gyr_y, last_gyr_z;
int last_mag_x, last_mag_y, last_mag_z;
int last_gravity_x, last_gravity_y, last_gravity_z;
int last_linear_acc_x, last_linear_acc_y, last_linear_acc_z;

//NMEA Strings
char Baud_Rate_Change_38400 [] = "$PMTK251,38400*27\r\n";
char NMEA_Update_Rate [] = "$PMTK220,1000*1F\r\n";

int main(void) {
    
    //Config Functions
    InitSystem();
    SPI1_Init();
    UART_1_Init(9600);
    I2C_1_Init();
    
    //Null Variables
    acc_x = acc_y = acc_z = 0;
    gyr_x = gyr_y = gyr_z = 0;
    mag_x = mag_y = mag_z = 0;
    gravity_x = gravity_y = gravity_z = 0;
    linear_acc_x = linear_acc_y = linear_acc_z = 0;
    last_acc_x = last_acc_y = last_acc_z = 0;
    last_gyr_x = last_gyr_y = last_gyr_z = 0;
    last_mag_x = last_mag_y = last_mag_z = 0;
    last_gravity_x = last_gravity_y = last_gravity_z = 0;
    last_linear_acc_x = last_linear_acc_y = last_linear_acc_z = 0;
    
    //Boot Message
    TFT_Init();
    sprintf(buffer_1,"Booting...");
//    TFT_Text(buffer_1, 20, 20, BLACK, WHITE);
//    sprintf(buffer_1,"Configuring IMU...");
//    TFT_Text(buffer_1, 60, 20, BLACK, WHITE);
//    I2C_1_Write_Byte(BNO_DEVICE, OPR_MODE, 0x00);

    while (1) {
        LATBbits.LATB0 ^= 1;
        int i;
        for (i = 0; i < 10000000; ++i);
    }
}
/*******************************************************************************
 End of File
 */
