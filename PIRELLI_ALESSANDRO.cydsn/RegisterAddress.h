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
*   \brief Address of the WHO AM I register
*/
#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

/**
*   \brief Address of the Status register
*/
#define LIS3DH_STATUS_REG 0x27

/**
*   \brief Address of the Control register 1
*/
#define LIS3DH_CTRL_REG1 0x20

/**
*   \brief  Address of the Temperature Sensor Configuration register
*/
#define LIS3DH_TEMP_CFG_REG 0x1F

/**
*   \brief Address of the Control register 4
*/
#define LIS3DH_CTRL_REG4 0x23

/**
*   \brief Address of the ADC output LSB register
*/
#define LIS3DH_MODE_ADC 0x88   //HR, +-2g scale, sensitivity 1mg/digit

/**
*   \brief Address of the ADC output LSB register
*/
#define LIS3DH_OUT_X_L 0x28

#define LIS3DH_OUT_X_H 0x29

#define LIS3DH_OUT_Y_L 0x2A

#define LIS3DH_OUT_Y_H 0x2B

#define LIS3DH_OUT_Z_L 0x2C

#define LIS3DH_OUT_Z_H 0x2D

/* [] END OF FILE */
