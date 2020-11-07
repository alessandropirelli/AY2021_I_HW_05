/**
* \brief Main source file for the I2C-Master project.
*
* In this project we set up a I2C master device with
* to understand the I2C protocol and communicate with a
* a I2C Slave device (LIS3DH Accelerometer).
*
* \author Davide Marzorati
* \date October 14, 2019
*/

// Include required header files
#include "I2C_Interface.h"
#include "project.h"
#include "stdio.h"
#include "GV.h"

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
*   \brief Hex value to set normal mode to the accelerator
*/
#define LIS3DH_NORMAL_MODE_CTRL_REG1 0x47

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
#define LIS3DH_OUT_ADC_3L 0x0C

/**
*   \brief Address of the ADC output MSB register
*/
#define LIS3DH_OUT_ADC_3H 0x0D

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    state = 0;
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    EEPROM_Start();
    EEPROM_UpdateTemperature();
    
    state = EEPROM_ReadByte(0x0000);        //Recover latest state value from EEPROM
    
    
    I2C_Interface_Start();
    UART_Start();
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    // String to print out messages on the UART
    char message[50];

    /*
    // Check which devices are present on the I2C bus
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_Debug_PutString(message); 
        }
        
    }
    */
    
    /******************************************/
    /*            I2C Reading                 */
    /******************************************/
    
    /* Read WHO AM I REGISTER register */
    uint8_t who_am_i_reg;
    ErrorCode error = I2C_Interface_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_WHO_AM_I_REG_ADDR, 
                                                  &who_am_i_reg);
    if (error == NO_ERROR)
    {
        sprintf(message, "WHO AM I REG: 0x%02X [Expected: 0x33]\r\n", who_am_i_reg);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm\r\n");   
    }
    
       
    
    
    /*      I2C Reading Status Register       */
    
    uint8_t status_register; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &status_register);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "STATUS REGISTER: 0x%02X\r\n", status_register);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read status register\r\n");   
    }
    
     
   
    /******************************************/
    /*            I2C Writing                 */
    /******************************************/
    
        
    UART_Debug_PutString("\r\nWriting new values..\r\n");
    
    if (ctrl_reg1 != LIS3DH_NORMAL_MODE_CTRL_REG1)
    {
        ctrl_reg1 = LIS3DH_NORMAL_MODE_CTRL_REG1;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }
    
    /******************************************/
    /*     Read Control Register 1 again      */
    /******************************************/

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1 after overwrite operation: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    /******************************************/
    /*   I2C Reading Temperature sensor reg   */
    /******************************************/
    uint8_t tmp_cfg_reg;
    uint8_t ctrl_reg4;
    
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_TEMP_CFG_REG,
                                        &tmp_cfg_reg);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "TEMPERATURE CONFIG REGISTER: 0x%02X\r\n", tmp_cfg_reg);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read temperature config register\r\n");   
    }
    
    tmp_cfg_reg = 0xC0; // must be changed to the appropriate value
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_TEMP_CFG_REG,
                                         tmp_cfg_reg);
    
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_TEMP_CFG_REG,
                                        &tmp_cfg_reg);
    
    
    if (error == NO_ERROR)
    {
        sprintf(message, "TEMPERATURE CONFIG REGISTER after being updated: 0x%02X\r\n", tmp_cfg_reg);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read temperature config register\r\n");   
    }
    
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CTRL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read CTRL REG 4\r\n");   
    }
    
    ctrl_reg4 = 0x80; // must be changed to the appropriate value
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
                                         ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CTRL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to write CTRL REG 4\r\n");   
    }
    
    int16_t OutTemp;
    uint8_t TemperatureData[2];
    
    for(;;)
    {
        CyDelay(10);
        
        switch (state){
            case 0:
                error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_CTRL_REG1,
                                                    _1HZ);
                break;
                
            case 1:
                error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_CTRL_REG1,
                                                    _10HZ);
                break;
                
            case 2:
                error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_CTRL_REG1,
                                                    _25HZ);    
                break;
                
            case 3:
                error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_CTRL_REG1,
                                                    _50HZ);
                break;
                
            case 4:
                error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_CTRL_REG1,
                                                    _100HZ);
                break;
                
            case 5:
                error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_CTRL_REG1,
                                                    _200HZ);
                break;
                
            default:
                break;
        }
        
        
        
        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_ADC_3L,
                                            &TemperatureData[0]);
        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_ADC_3H,
                                            &TemperatureData[1]);
        if (error == NO_ERROR)
        {
            OutTemp = (int16)((TemperatureData[0] | (TemperatureData[1]<<8)))>>6;
            sprintf(message, "Temperature : %d\r\n", OutTemp);
            UART_Debug_PutString(message);
            
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to read ADC 3 / temperature output registers\r\n");   
        }  
    }
}

/* [] END OF FILE */