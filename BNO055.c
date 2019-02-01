
#include "BNO055.h"

//Globals
int acc_x, acc_y, acc_z;
int gyr_x, gyr_y, gyr_z;
int mag_x, mag_y, mag_z;
int gravity_x, gravity_y, gravity_z;
int linear_acc_x, linear_acc_y, linear_acc_z;
//For Filtering
int last_acc_x, last_acc_y, last_acc_z;
int last_gyr_x, last_gyr_y, last_gyr_z;
int last_mag_x, last_mag_y, last_mag_z;
int last_gravity_x, last_gravity_y, last_gravity_z;
int last_linear_acc_x, last_linear_acc_y, last_linear_acc_z;

int i;
char buffer_1[20];

void Null_IMU_Values(void) {
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


}

void BNO_Init(void) {
    sprintf(buffer_1, "Configuring IMU...");
    TFT_Text(buffer_1, 20, 40, BLACK, WHITE);
    I2C_1_Write_Byte(BNO_DEVICE, OPR_MODE, 0x00);
    I2C_1_Write_Byte(BNO_DEVICE, TEMP_SOURCE, 0x01);
    I2C_1_Write_Byte(BNO_DEVICE, OPR_MODE, 0x0C);
    sprintf(buffer_1, "IMU Configured...");
    TFT_Text(buffer_1, 20, 60, BLACK, WHITE);

}

void BNO_Cal_Routine(void) {
    char sys_cal, acc_cal, mag_cal, gyr_cal;
    sprintf(buffer_1, "Calibrating...");
    TFT_Text(buffer_1, 20, 80, BLACK, WHITE);
    sprintf(buffer_1, "S : G : A : M");
    TFT_Text(buffer_1, 20, 100, BLACK, WHITE);
    
    while (gyr_cal != 0xFF) {

        gyr_cal = I2C_1_Read_Byte(BNO_DEVICE, CALIB_STATUS);
        sys_cal = acc_cal = mag_cal = gyr_cal;
        sys_cal >> 6;
        acc_cal &= 0x0C;
        acc_cal >> 2;
        mag_cal &= 0x03;
        gyr_cal &= 0x30;
        gyr_cal >> 4;
        sprintf(buffer_1, "%d, %d, %d, %d", sys_cal, gyr_cal, acc_cal, mag_cal);
        TFT_Text(buffer_1, 20, 120, BLACK, WHITE);
        for (i = 0; i < 10000; i++);
    }

}