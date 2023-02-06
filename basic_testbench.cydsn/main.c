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
#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



// Declare byte array in flash memory:
static const char CYCODE flash_userdata[230000]={0};

CY_ISR_PROTO( isr_rx_handle );

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    isr_rx_StartEx( isr_rx_handle );
  
    char uart_msg[80];
    sprintf( uart_msg, "PSOC running...\n\r" );
    UART_1_PutString( uart_msg );

    for(;;)
    {
        /* Place your application code here. */

    }//END for(;;)
}//END main()



CY_ISR( isr_rx_handle )
{ 
    char putty = UART_1_GetChar();
    if( putty == 'x' ) CySoftwareReset();
    
    // tb data in
    i_Testbench8_LSB_Write( (uint8)putty );
    
    // tb data out
    UART_1_PutChar( o_Testbench8_MSB_Read() );
    UART_1_PutChar( o_Testbench8_LSB_Read() );
    
    LED_Write(1); CyDelay(50); LED_Write(0); CyDelay(50);
}//END CY_ISR( isr_rx_handle )


/* [] END OF FILE */
