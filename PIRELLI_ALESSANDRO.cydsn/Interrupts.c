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

#include "Interrupts.h"
#include "GV.h"
#include "I2C_Interface.h"

ErrorCode error;

CY_ISR(Debounce){
    if( Pin_Button_Read() == PRESSED){
        state++;
        if(state == 6){
            state = 0;
        } 
    }
    
    EEPROM_WriteByte(state, 0x00);   //Save state's latest value in the EEPROM's register
    
}

/* [] END OF FILE */
