

#include <xc.h>
#include <stdio.h>
#include "CONFIG.h"
#include "I2C.h"
#include "ILI9341.h"
#ifdef _BNO055_H
#define _BNO055_H

void BNO_Init(void);
void BNO_Cal_Routine(void);
#endif