
// Include required header files
#include "I2C_Interface.h"
#include "project.h"
#include "stdio.h"
#include "RegisterAddress.h"
#include "Interrupts.h"

#define DATA 6
#define DIM_DATA 1+DATA+1
#define SENSITIVITY 0.0098
#define HEADER 0XA0
#define TAIL 0XC0

uint8_t mode;
ErrorCode error;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
            
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    EEPROM_Start();
    EEPROM_UpdateTemperature();
    
    mode = EEPROM_ReadByte(0x0000);        //Recover latest state value from EEPROM
    
    I2C_Interface_Start();
    UART_Start();
    Timer_Debouncer_Start();
    
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    /*
    // String to print out messages on the UART
    char message[50];

    
    // Check which devices are present on the I2C bus
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Interface_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_PutString(message); 
        }
        
    }
    */
    
    /******************************************/
    /*            I2C Reading                 */
    /******************************************/
    
    /* Read WHO AM I REGISTER register */
    
    uint8_t who_am_i_reg;
    error = I2C_Interface_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                       LIS3DH_WHO_AM_I_REG_ADDR, 
                                       &who_am_i_reg);
    
    
    /*
    if (error == NO_ERROR)
    {
        sprintf(message, "WHO AM I REG: 0x%02X [Expected: 0x33]\r\n", who_am_i_reg);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm\r\n");   
    }
    */
     
   
    /******************************************/
    /*            I2C Writing                 */
    /******************************************/
    
    
    /*      Write Control Register 4     */
    
    uint8 ctrl_reg4 = LIS3DH_MODE_ADC;
    
    error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        ctrl_reg4);
    /*
    if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 4 successfully written as: 0x%02X\r\n", ctrl_reg4);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 4\r\n");   
        }
    
    */
     /*      Write Control Register 1     */
    
      
    error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        mode);
   /*
    uint8_t ctrl_reg1;
    error = I2C_Interface_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                       LIS3DH_CTRL_REG1, 
                                       &ctrl_reg1);
   */
    
    /*
    if (error == NO_ERROR)
    {
        sprintf(message, "Control Register 1: 0x%02X \r\n", ctrl_reg1);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm\r\n");   
    };    
    */    
          
        
    uint16 x_value;
    float x;
    float x_conv;
    
    uint16 y_value;
    int16_t y;
    float y_conv;
    
    uint16 z_value;
    int16_t z;
    float z_conv;
    
    float sensitivity = SENSITIVITY;
    
    uint8_t DataArray[DATA];
    uint8_t OutArray[DIM_DATA];
       
    OutArray[0] = HEADER; 
    OutArray[DIM_DATA-1] = TAIL;
    
    isr_TDb_StartEx(Debounce);
    
    for(;;)
    {
        CyDelay(10);
               
        error = I2C_Interface_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                LIS3DH_OUT_X_L,
                                                DATA,
                                                DataArray);
        if(error == NO_ERROR){
        
            x_value = (int16)((DataArray[0] | (DataArray[1]<<8)))>>4;
            x = (float) (x_value);         
            x_conv = x * sensitivity;
            x_value = (int16_t) (x_conv * 1000);
            
            y_value = (int16)((DataArray[2] | (DataArray[3]<<8)))>>4;
            y = (float) (y_value);
            y_conv = y * sensitivity;
            y_value = (int16_t) (y_conv * 1000);
            
            z_value = (int16)((DataArray[4] | (DataArray[5]<<8)))>>4;
            z = (float) (z_value);           
            z_conv = z * sensitivity;
            z_value = (int16_t) (z_conv * 1000);
                      
            OutArray[1]=(uint8_t)(x_value & 0xFF);
            OutArray[2]=(uint8_t)(x_value >> 8);
            OutArray[3]=(uint8_t)(y_value & 0xFF);
            OutArray[4]=(uint8_t)(y_value >> 8);
            OutArray[5]=(uint8_t)(z_value & 0xFF);
            OutArray[6]=(uint8_t)(z_value >> 8);
            
            UART_PutArray(OutArray, DIM_DATA);
                    
        }
        
//        else
//        {
//           UART_PutString("Error occurred during I2C comm to read OUT_X / temperature output registers\r\n");   
//        }       
        
        
        
        
    }
}


/* [] END OF FILE */