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

#define STEP 0x10       //Consecutive settings of Control register 1 for sampling freq are 0x10 distant between each other (check Registers.h) 

#define EEPROM_ADC_MODE_ADDR 0x00       //EEPROM register dedicated to store Control register 1 settings

#include "Interrupts.h"
#include "Registers.h"
#include "stdio.h"

CY_ISR(Change_Freq)
{                       
    if( mode == LIS3DH_200HZ_MODE_CTRL_REG1 )    //Max sampling frequency reached
    {
        mode = LIS3DH_1HZ_MODE_CTRL_REG1;        //Restart from minimum sampling frequency
    }
    else
    {
        mode += STEP;           //Move to next sampling freq
    }    
    EEPROM_WriteByte(mode, EEPROM_ADC_MODE_ADDR);       //Save latest sampling freq value in EEPROM's assigned register                         
}


/* [] END OF FILE */
