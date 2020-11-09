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

#ifndef __INTERRUPTS_H
    #define __INTERRUPTS_H
    
    #include "project.h"
    #include "ErrorCodes.h"
    
    CY_ISR_PROTO(Debounce);
    
    extern uint8_t mode;
    extern ErrorCode error;
    
#endif

/* [] END OF FILE */
