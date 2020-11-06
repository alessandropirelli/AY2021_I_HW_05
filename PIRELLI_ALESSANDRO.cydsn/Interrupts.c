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


CY_ISR(Debounce){
    if( Pin_Button_Read() == PRESSED){
        state++;
        if(state>_200HZ){
            state=_1HZ;
        } 
    }
}

/* [] END OF FILE */
