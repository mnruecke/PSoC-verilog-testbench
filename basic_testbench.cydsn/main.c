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
void connect_to_example_2b(void);

CY_ISR_PROTO( isr_rx_handle );

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    isr_rx_StartEx( isr_rx_handle );
    
    Timer_1_Start();
  
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
    
    connect_to_example_2b();
    
    LED_Write(1); CyDelayUs(500); LED_Write(0); CyDelayUs(500);
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
char i_rx=1;
void connect_to_example_2(void){
    
    char byte_in = UART_1_GetChar();
      
    if( byte_in == 'x' && (i_clock == 'x' || i_rx == 'x')){
        // Reset condition
        char_in_count   = 0;
        i_clock         = 0;
        i_rx            = 1;        
    }else if( char_in_count%2 == 0){
        // Set clock_in
        i_clock = byte_in;
        ++char_in_count;
    }else{
        // Set rx_in
        i_rx = byte_in;
        ++char_in_count;
    }
    
    i_Uart_TB_clock_Write( i_clock  );
    i_Uart_TB_rx_Write(    i_rx     );

    UART_1_PutChar( char_in_count   );
    UART_1_PutChar( i_clock         );
    UART_1_PutChar( i_rx            );
    
    UART_1_PutChar( o_Uart_TB_RX_DV_Read()  );
    UART_1_PutChar( o_Uart_TB_RX_Byte_Read());
    
    
}//END connect_to_example_2()

void connect_to_example_2b(void){ 
    
    // Flash memory timing
    
    const int clock_rate_mhz = 79;
    const int data_size = 256;
    
    char uart_msg[80];
    uint8 data[ data_size ];
    for(int i=0; i < data_size; ++i)
        data[i]=i;
    
    Timer_1_WriteCounter( 1000000000 );
    long long tic, toc;
    tic = Timer_1_ReadCounter();
    //CyDelayUs(10); // Test calibration
    // DUT:
    Em_EEPROM_1_Write( (uint8*)data, (const uint8*) 0x10000, data_size);
    toc = Timer_1_ReadCounter();
    
    
    sprintf( uart_msg, "%d.", (int)((tic-toc)/clock_rate_mhz) );       
    UART_1_PutString( uart_msg );
    sprintf( uart_msg, "%2d us\n\r", (int)((100*(tic-toc))/79
                                         - ((tic-toc)/clock_rate_mhz)*100));       
    UART_1_PutString( uart_msg );

}//END connect_to_example_2b()


/* [] END OF FILE */
