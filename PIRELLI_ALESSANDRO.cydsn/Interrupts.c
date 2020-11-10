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

#define VALID 5     //Represents the minum amount of time the button has to be pressed for the pressing to be validated (since the Debouncer_Timer period is set at 100ms --> 5 = 500ms)

#define LIS3DH_1HZ_MODE_CTRL_REG1 0x17
#define LIS3DH_200HZ_MODE_CTRL_REG1 0x67
#define STEP 0x10       //Settings of ctrl_reg1 for sampling freq are 0x17 (1Hz), 0x27(5Hz), 0x37 (10Hz), 0x47, 0x57, 0x67 (200Hz)

#define EEPROM_ADC_MODE_ADDR 0x00

#include "Interrupts.h"
#include "Registers.h"
#include "ErrorCodes.h"
#include "I2C_Interface.h"
#include "stdio.h"


int count;

/*The following function both acts as a debouncer (for the pressure of the button) and updates the sampling frequency when the button is pressed*/

CY_ISR(Debounce){   
    Timer_Debouncer_ReadStatusRegister();
    if( Pin_Button_Read() == PRESSED ){
        count++;   
        if(count >= 100){       //Overflow protection
            count = VALID;
        }
    }
    if(Pin_Button_Read() == NOT_PRESSED){                  
        if(count >= VALID){       
            if( mode == LIS3DH_200HZ_MODE_CTRL_REG1 ){
                mode = LIS3DH_1HZ_MODE_CTRL_REG1;
                count=0;
            }else{
                mode = mode + STEP;         //Move to the next sampling freq      
                count =0;
            }
        EEPROM_WriteByte(mode, EEPROM_ADC_MODE_ADDR);                   //Save latest sampling freq value in the EEPROM's assigned register
        error = I2C_Interface_WriteRegister(LIS3DH_DEVICE_ADDRESS,      //Update sampling frequency
                                            LIS3DH_CTRL_REG1,
                                            mode);
        }else{
            count=0;
        }
    }          
/*-------------------------DEBUGGING-------------------------*/
/*    
    error = I2C_Interface_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                       LIS3DH_CTRL_REG1,
                                       &ctrl_reg1);  
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1 : 0x%02X\r\n", ctrl_reg1);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
    };  
*/
/*-----------------------------------------------------------*/    
}


/* [] END OF FILE */
