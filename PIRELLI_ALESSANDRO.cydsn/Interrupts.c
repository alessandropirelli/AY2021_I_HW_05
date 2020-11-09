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

#define NOT_PRESSED 1
#define PRESSED 0

#define LIS3DH_1HZ_MODE_CTRL_REG1 0x17
#define LIS3DH_200HZ_MODE_CTRL_REG1 0x67

#include "Interrupts.h"
#include "RegisterAddress.h"
#include "ErrorCodes.h"
#include "I2C_Interface.h"
#include "stdio.h"


int count;

CY_ISR(Debounce){
    Timer_Debouncer_ReadStatusRegister();
    if( Pin_Button_Read() == PRESSED ){
        count++;       
    }
    if(Pin_Button_Read() == NOT_PRESSED){                  
        if(count>=5){       
            if( mode == LIS3DH_200HZ_MODE_CTRL_REG1 ){
                mode = LIS3DH_1HZ_MODE_CTRL_REG1;
                count=0;
            }else{
                mode = mode + 0x10;
                count =0;
            }
        EEPROM_WriteByte(mode, 0x00);   //Save state latest value in the EEPROM's register
        error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_CTRL_REG1,
                                            mode);
        }else{
            count=0;
        }
    }    
        
        /*
        error = I2C_Interface_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_CTRL_REG1,
                                            &ctrl_reg1);  
    */
        /*if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 : 0x%02X\r\n", ctrl_reg1);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        };*/
}


/* [] END OF FILE */
