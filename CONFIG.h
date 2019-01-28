/* ************************************************************************** */
/** Descriptive File Name
  @File CONFIG
    CONFIG.h

  @Summary
 Configuration settings for PIC32MX

  @Description
 Configuration setting for PIC32MX
 * 
 * /* TODO:  Test and work */
 /*
/* ************************************************************************** */

#ifndef _CONFIG_H    /* Guard against multiple inclusion */
#define _CONFIG_H


/* ************************************************************************** */
/*                               INCLUDES                                     */
/* ************************************************************************** */

#include <xc.h>

/* ************************************************************************** */
/*                               CONSTANTS                                    */
/* ************************************************************************** */

#define _XTAL_FREQ 40000000L

static void InitPins(void);
static void InitClock(void);
static void InitInterruptSystem(void); 
void InitSystem(void);

#endif 

/* *****************************************************************************
 End of File
 */
