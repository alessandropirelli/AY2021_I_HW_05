/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "project.h"

/*      brief 7-bit I2C address of the slave device     */

#define LIS3DH_DEVICE_ADDRESS 0x18

/*      brief Address of the Control register 1     */

#define LIS3DH_CTRL_REG1 0x20

/*      Control register 1 settings        */

#define LIS3DH_1HZ_MODE_CTRL_REG1 0x17          //Sampling freq 1Hz
#define LIS3DH_10HZ_MODE_CTRL_REG1 0x27         //Sampling freq 10Hz
#define LIS3DH_25HZ_MODE_CTRL_REG1 0x37         //Sampling freq 25Hz
#define LIS3DH_50HZ_MODE_CTRL_REG1 0x47         //Sampling freq 50Hz
#define LIS3DH_100HZ_MODE_CTRL_REG1 0x57        //Sampling freq 100Hz
#define LIS3DH_200HZ_MODE_CTRL_REG1 0x67        //Sampling freq 200Hz

/*      brief Address of the Status register        */

#define LIS3DH_STS_REG 0x27

/*      brief Address of the Control register 4     */

#define LIS3DH_CTRL_REG4 0x23

/*      Control register 4 settings     */

#define LIS3DH_HR_2G 0x88   //High resolution mode, +-2g full scale range --> sensitivity 1mg/digit
#define LIS3DH_HR_4G 0x98   //High resolution mode, +-4g full scale range --> sensitivity 2mg/digit
#define LIS3DH_HR_8G 0xA8   //High resolution mode, +-8g full scale range --> sensitivity 4mg/digit
#define LIS3DH_HR_16G 0xB8  //High resolution mode, +-16g full scale range --> sensitivity 12mg/digit

/*      brief Address of the acceleration output registers      */

#define LIS3DH_OUT_X_L 0x28     //X acc least significant bits
#define LIS3DH_OUT_X_H 0x29     //X acc most significant bits
#define LIS3DH_OUT_Y_L 0x2A     //Y acc least significant bits
#define LIS3DH_OUT_Y_H 0x2B     //Y acc most significant bits    
#define LIS3DH_OUT_Z_L 0x2C     //Z acc least significant bits
#define LIS3DH_OUT_Z_H 0x2D     //Z acc most significant bits

/* [] END OF FILE */
