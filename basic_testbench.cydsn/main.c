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

void connect_to_example_1(void);
void connect_to_example_2(void);

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
    connect_to_example_2();
    
    LED_Write(1); CyDelay(50); LED_Write(0); CyDelay(50);
}//END CY_ISR( isr_rx_handle )

void connect_to_example_1(void){
    char putty = UART_1_GetChar();
    if( putty == 'x' ) CySoftwareReset();
    
    // tb data in
    i_Testbench8_LSB_Write( (uint8)putty );
    
    // tb data out
    UART_1_PutChar( o_Testbench8_MSB_Read() );
    UART_1_PutChar( o_Testbench8_LSB_Read() );    
}//END connect_to_example_1()


int char_in_count=0;   
char i_clock=0;
char i_rx=0;
void connect_to_example_2(void){
    
    char byte_in = UART_1_GetChar();
      
    if( byte_in == 'x' && (i_clock == 'x' || i_rx == 'x')){
        // Reset condition
        char_in_count   = 0;
        i_clock         = 0;
        i_rx            = 0;        
    }else if( char_in_count%2 == 0){
        // Set clock_in
        i_clock = byte_in;
        ++char_in_count;
    }else{
        // Set rx_in
        i_rx = byte_in;
        ++char_in_count;
    }
    
    i_Uart_TB_clock_Write( i_clock );
    i_Uart_TB_rx_Write(    i_rx    );

    UART_1_PutChar( char_in_count );
    UART_1_PutChar( i_clock );
    UART_1_PutChar( i_rx );
    
    UART_1_PutChar( o_Uart_TB_RX_DV_Read() );
    UART_1_PutChar( o_Uart_TB_RX_Byte_Read() );
    
    
}//END connect_to_example_1()


/* [] END OF FILE */
