//*******************************************************************//
// File: SPI.h                                                       //
// Author: Thomas Mindenhall                                         //
// Project: Backpack Buddy                                           //
//                                                                   //
//                                                                   //
// Description:  Configuration and Use of SPI Modules                //
//                                                                   //
//*******************************************************************//

#ifndef _SPI_H    /* Guard against multiple inclusion */
#define _SPI_H

///////////////////////////////////////////////////////////////////////////////
//*****************************Includes**************************************//
///////////////////////////////////////////////////////////////////////////////
#include <xc.h>
#include "CONFIG.h"

////////////////////////////////////////////////////////////////////////////////
//*******************************Defines**************************************//
////////////////////////////////////////////////////////////////////////////////
#define TRIS_SCK1   TRISBbits.TRISB14        
#define TRIS_SDO1   TRISBbits.TRISB13
#define TRIS_SDI1   TRISBbits.TRISB11
#define TRIS_SS1    TRISBbits.TRISB15

////////////////////////////////////////////////////////////////////////////////
//*****************************Prototypes*************************************//
////////////////////////////////////////////////////////////////////////////////
void SPI1_Init(void);
void SPI1_Close(void);
void SPI1_Write(uint8_t data);
unsigned char SPI1_Read(void);

#endif

/* *****************************************************************************
 End of File
 */
