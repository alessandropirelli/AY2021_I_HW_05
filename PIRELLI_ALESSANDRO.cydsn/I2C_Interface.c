/*
* This file includes all the required source code to interface
* the I2C peripheral.
*
* This file needs to be completed.
*/

/**
*   \brief Value returned if device present on I2C bus.
*/
#ifndef DEVICE_CONNECTED
    #define DEVICE_CONNECTED 1
#endif

/**
*   \brief Value returned if device not present on I2C bus.
*/
#ifndef DEVICE_UNCONNECTED
    #define DEVICE_UNCONNECTED 0
#endif

#include "I2C_Interface.h" 
#include "I2C.h"

    ErrorCode I2C_Interface_Start(void) 
    {
        // Start I2C peripheral
        
        I2C_Start();
        
        // Return no error since start function does not return any error
        return NO_ERROR;
    }
    
    
    ErrorCode I2C_Interface_Stop(void)
    {
        // Stop I2C peripheral
        I2C_Stop();
        // Return no error since stop function does not return any error
        return NO_ERROR;
    }

    ErrorCode I2C_Interface_ReadRegister(uint8_t device_address, 
                                            uint8_t register_address,
                                            uint8_t* data)
    {
        // Send start condition
        uint8_t error = I2C_MasterSendStart(device_address,I2C_WRITE_XFER_MODE);
        if (error == I2C_MSTR_NO_ERROR)
        {
            // Write address of register to be read
            error = I2C_MasterWriteByte(register_address);
            if (error == I2C_MSTR_NO_ERROR)
            {
                // Send restart condition
                error = I2C_MasterSendRestart(device_address, I2C_READ_XFER_MODE);
                if (error == I2C_MSTR_NO_ERROR)
                {
                    // Read data without acknowledgement
                    *data = I2C_MasterReadByte(I2C_ACK_DATA);
                }
            }
        }
        // Send stop condition if something went wrong
        I2C_MasterSendStop();
        // Return error code
        return error ? ERROR : NO_ERROR;
    }
    
    ErrorCode I2C_Interface_ReadRegisterMulti(uint8_t device_address,
                                                uint8_t register_address,
                                                uint8_t register_count,
                                                uint8_t* data)
    {
        // Send start condition
        uint8_t error = I2C_MasterSendStart(device_address,I2C_WRITE_XFER_MODE);
        if (error == I2C_MSTR_NO_ERROR)
        {
            // Write address of register to be read with the MSB equal to 1
            register_address |= 0x80; // Datasheet indication for multi read -- autoincrement
            error = I2C_MasterWriteByte(register_address);
            if (error == I2C_MSTR_NO_ERROR)
            {
                // Send restart condition
                error = I2C_MasterSendRestart(device_address, I2C_READ_XFER_MODE);
                if (error == I2C_MSTR_NO_ERROR)
                {
                    // Continue reading until we have register to read
                    uint8_t counter = register_count;
                    while(counter>1)
                    {
                        data[register_count-counter] =
                            I2C_MasterReadByte(I2C_ACK_DATA);
                        counter--;
                    }
                    // Read last data without acknowledgement
                    data[register_count-1]
                        = I2C_MasterReadByte(I2C_NAK_DATA);
                }
            }
        }
        // Send stop condition
        I2C_MasterSendStop();
        // Return error code
        return error ? ERROR : NO_ERROR;
    }
    
    ErrorCode I2C_Interface_WriteRegister(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t data)
    {
        //Start condition
        uint8_t error = I2C_MasterSendStart(device_address, I2C_WRITE_XFER_MODE);
        if (error == I2C_MSTR_NO_ERROR)
        {
            error = I2C_MasterWriteByte(register_address);
            if (error == I2C_MSTR_NO_ERROR)
            {
                error = I2C_MasterWriteByte(data);
            }
        }
        I2C_MasterSendStop();
        return error ? ERROR : NO_ERROR;
    }
        
    
    uint8_t I2C_Interface_IsDeviceConnected(uint8_t device_address)
    {
        // Send a start condition followed by a stop condition
        uint8_t error = I2C_MasterSendStart(device_address, I2C_WRITE_XFER_MODE);
        I2C_MasterSendStop();
        // If no error generated during stop, device is connected
        if (error == I2C_MSTR_NO_ERROR)
        {
            return DEVICE_CONNECTED;
        }
        return DEVICE_UNCONNECTED;
    }

/* [] END OF FILE */
