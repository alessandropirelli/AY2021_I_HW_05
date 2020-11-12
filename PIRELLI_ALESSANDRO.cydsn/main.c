
// Include required header files
#include "I2C_Interface.h"
#include "project.h"
#include "stdio.h"
#include "Registers.h"
#include "Interrupts.h"

#define ZYXDA 0x08
#define ZYXOR 0x80

#define EEPROM_ADC_MODE_ADDR 0x00

#define DATA 6               //2 bytes for each acceleration (X,Y,Z)
#define DIM_DATA 1+DATA+1    //Header + Data + Tail          
#define HEADER 0XA0
#define TAIL 0XC0

#define mg 0.0098  

uint8_t mode;
ErrorCode error;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
            
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    EEPROM_Start();
    EEPROM_UpdateTemperature();
    
    mode = EEPROM_ReadByte(EEPROM_ADC_MODE_ADDR);        //Recover latest Control register 1 setting from EEPROM
    
    if((mode != LIS3DH_1HZ_MODE_CTRL_REG1) &&           //If at start up the value read from EEPROM isn't one of the accepted ones 
       (mode != LIS3DH_10HZ_MODE_CTRL_REG1) &&          //(e.g. first time using the device, control register 1 value not yet stored in EEPROM)
       (mode != LIS3DH_25HZ_MODE_CTRL_REG1) &&          //(check Registers.h for settings' meaning)
       (mode != LIS3DH_50HZ_MODE_CTRL_REG1) &&
       (mode != LIS3DH_100HZ_MODE_CTRL_REG1) &&
       (mode != LIS3DH_200HZ_MODE_CTRL_REG1))           
    {
        mode = LIS3DH_1HZ_MODE_CTRL_REG1;               //"Reboot" starting from minimum sampling frequency
    }
    
    I2C_Interface_Start();
    UART_Start();   
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
/*-------------------------DEBUGGING-------------------------*/    
/*    
    char message[50];         // String to print out messages on the UART

    for (int i = 0 ; i < 128; i++)        // Check which devices are present on the I2C bus
    {
        if (I2C_Interface_IsDeviceConnected(i))
        {
            sprintf(message, "Device 0x%02X is connected\r\n", i);    // print out the address is hex format
            UART_PutString(message); 
        }       
    }
*/ 
/*-----------------------------------------------------------*/    
    
    /*      Write Control Register 4     */
    
    uint8 ctrl_reg4 = LIS3DH_HR_4G;  //possible values are LIS3DH_HR_2G, LIS3DH_HR_4G, LIS3DH_HR_8G, LIS3DH_HR_16G (check Registers.h)
    
    error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        ctrl_reg4);
    
/*-------------------------DEBUGGING-------------------------*/   
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
/*-----------------------------------------------------------*/
     
    /*      Write Control Register 1     */
    
    error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        mode);
   
/*-------------------------DEBUGGING-------------------------*/    
/*
    uint8_t ctrl_reg1;
    error = I2C_Interface_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                       LIS3DH_CTRL_REG1, 
                                       &ctrl_reg1);
    if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg4);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }   
    };    
*/    
/*-----------------------------------------------------------*/            
        
    uint8_t status_reg; 
    
    int16 x_value;
    float x_conv;
    
    int16 y_value;
    float y_conv;
    
    int16 z_value;
    float z_conv;
    
    /*      Auto-adjust sensitivity on FS range previously choosen (based on datasheet)     */
    
    float sensitivity;
    
    switch(ctrl_reg4)
    {
        case LIS3DH_HR_2G:
                sensitivity = 1*mg;
                break;               
            case LIS3DH_HR_4G:
                sensitivity = 2*mg;
                break;
            case LIS3DH_HR_8G:
                sensitivity = 4*mg;
                break;
            case LIS3DH_HR_16G:
                sensitivity = 12*mg;
                break; 
            default:
                break;
    }
    
    uint8_t DataArray[DATA];
    uint8_t OutArray[DIM_DATA];
       
    OutArray[0] = HEADER; 
    OutArray[DIM_DATA-1] = TAIL;
    
    isr_Dbc_StartEx(Change_Freq);
    
    /*      Update status_reg       */
    
    error = I2C_Interface_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                       LIS3DH_STS_REG,
                                       &status_reg);
    
    for(;;)
    {                         
        error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,      //Update sampling frequency (also causes a slight delay in the cycle useful for stability)
                                            LIS3DH_CTRL_REG1,
                                            mode);
        
        /*-------------------------DEBUGGING-------------------------*/
/* 
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 correctly set as: 0x%02X\r\n", ctrl_reg1);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        };  
*/
        /*-----------------------------------------------------------*/
        
        if ((status_reg & ZYXDA) == ZYXDA)                   //If new set of data from accelerometer is available
        {
            /*      Read acceleration data      */
        
            error = I2C_Interface_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_OUT_X_L,
                                                    DATA,
                                                    DataArray);
            if(error == NO_ERROR)
            {       
                /*      Convert X axis acceleration      */
                
                x_value = (int16)((DataArray[0] | (DataArray[1]<<8)))>>4;       //Acceleration data is 12 bits (High resolution mode) stored in 2 bytes left adjusted
                x_conv = (float) (x_value);
                x_conv *= sensitivity;
                x_value = (int16) (x_conv * 1000);                              //Multiplying by 1000 keeps 3 most significant decimals 
                
                /*      Convert Y axis acceleration      */
                
                y_value = (int16)((DataArray[2] | (DataArray[3]<<8)))>>4;
                y_conv = (float) (y_value);
                y_conv *= sensitivity;
                y_value = (int16) (y_conv * 1000);
                
                /*      Convert Z axis acceleration      */
                
                z_value = (int16)((DataArray[4] | (DataArray[5]<<8)))>>4;
                z_conv = (float) (z_value);           
                z_conv *= sensitivity;
                z_value = (int16) (z_conv * 1000);
                
                /*      Build array to send data        */
                
                OutArray[1] = (uint8_t)(x_value & 0xFF);
                OutArray[2] = (uint8_t)(x_value >> 8);
                OutArray[3] = (uint8_t)(y_value & 0xFF);
                OutArray[4] = (uint8_t)(y_value >> 8);
                OutArray[5] = (uint8_t)(z_value & 0xFF);
                OutArray[6] = (uint8_t)(z_value >> 8);
               
                /*      Send data      */
                
                UART_PutArray(OutArray, DIM_DATA);
                
                /*      Update status_reg       */
                
                error = I2C_Interface_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                   LIS3DH_STS_REG,
                                                   &status_reg);                
            }    
            else
            {
//                UART_PutString("Error occurred during I2C comm to read acceleration output registers\r\n");   //Debugging
            }                       
        }
        else                                    //If data from accelerometer isn't available yet
        {
            /*      Update status_reg       */
            
            error = I2C_Interface_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                               LIS3DH_STS_REG,
                                               &status_reg);
        }  
    }
}

/* [] END OF FILE */