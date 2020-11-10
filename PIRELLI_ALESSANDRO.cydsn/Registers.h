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

/**
*   \brief 7-bit I2C address of the slave device.
*/
#define LIS3DH_DEVICE_ADDRESS 0x18

/**
*   \brief Address of the Control register 1
*/
#define LIS3DH_CTRL_REG1 0x20


/**
*   \brief Address of the Control register 4
*/
#define LIS3DH_CTRL_REG4 0x23

/**
*   \Control register 4 settings
*/
#define LIS3DH_HR_2G 0x88   //High resolution mode, +-2g full scale range --> sensitivity 1mg/digit

#define LIS3DH_HR_4G 0x98   //High resolution mode, +-4g full scale range

#define LIS3DH_HR_8G 0xA8   //High resolution mode, +-8g full scale range

#define LIS3DH_HR_16G 0xB8  //High resolution mode, +-16g full scale range

/**
*   \brief Address of the acceleration output registers
*/
#define LIS3DH_OUT_X_L 0x28

#define LIS3DH_OUT_X_H 0x29

#define LIS3DH_OUT_Y_L 0x2A

#define LIS3DH_OUT_Y_H 0x2B

#define LIS3DH_OUT_Z_L 0x2C

#define LIS3DH_OUT_Z_H 0x2D

/* [] END OF FILE */
