
// Include required header files
#include "I2C_Interface.h"
#include "project.h"
#include "stdio.h"
#include "Registers.h"
#include "Interrupts.h"

#define DATA 6               //2 bytes for each acceleration (X,Y,Z)
#define DIM_DATA 1+DATA+1    //Header + Data + Tail
#define mg 0.0098            
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
    
    uint8 ctrl_reg4 = LIS3DH_HR_2G;  //possible values are LIS3DH_HR_2G, LIS3DH_HR_4G, LIS3DH_HR_8G, LIS3DH_HR_16G (check Registers.h)
    
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
        
    int16 x_value;
    float x_conv;
    
    int16 y_value;
    float y_conv;
    
    int16 z_value;
    float z_conv;
    
    /*      Auto-adjust sensitivity on FS range choosen (based on datasheet)     */
    
    float sensitivity;
    
    switch(ctrl_reg4){
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
    
    isr_TDb_StartEx(Debounce);
    
    for(;;)
    {
        CyDelay(10);
        
        /*      Read acceleration data      */
        error = I2C_Interface_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                LIS3DH_OUT_X_L,
                                                DATA,
                                                DataArray);
        if(error == NO_ERROR){
            
            /*      Convert X axis acceleration      */
            x_value = (int16)((DataArray[0] | (DataArray[1]<<8)))>>4;
            x_conv = (float) (x_value);
            x_conv *= sensitivity;
            x_value = (int16) (x_conv * 1000);
            
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
            
            /*      Build array to send data      */
            OutArray[1]=(uint8_t)(x_value & 0xFF);
            OutArray[2]=(uint8_t)(x_value >> 8);
            OutArray[3]=(uint8_t)(y_value & 0xFF);
            OutArray[4]=(uint8_t)(y_value >> 8);
            OutArray[5]=(uint8_t)(z_value & 0xFF);
            OutArray[6]=(uint8_t)(z_value >> 8);
            
            /*      Send data      */
            UART_PutArray(OutArray, DIM_DATA);            
        }       
        else
        {
//           UART_PutString("Error occurred during I2C comm to read acceleration output registers\r\n");   //Debugging
        }                  
    }
}


/* [] END OF FILE */