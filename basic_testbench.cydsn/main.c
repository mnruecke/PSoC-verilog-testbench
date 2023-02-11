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

struct tb_inputs{
    int     char_in_count;   
    char    i_clock;
    char    i_rx;   
} tb_inputs_ex2;

void connect_to_example_1(void);
void connect_to_example_2( struct tb_inputs * );
void connect_to_example_3_flash(void);

CY_ISR_PROTO( isr_uart_byte_received );

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    isr_rx_StartEx( isr_uart_byte_received );
    
    Timer_1_Start();
  
    char uart_msg[80];
    sprintf( uart_msg, "PSOC running...\n\r" );
    UART_1_PutString( uart_msg );

    for(;;)
    {
        /* Place your application code here. */



        
    }//END for(;;)
}//END main()


CY_ISR( isr_uart_byte_received ){ 
    
    // Hookup test routine:
    
    //connect_to_example_1();
    connect_to_example_2( &tb_inputs_ex2 );
    //connect_to_example_3_flash();
    
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


void connect_to_example_2( struct tb_inputs * tb_inputs_ex2 ){
    
    // Verilog code testbench hookup
    
    char byte_in = UART_1_GetChar();
      
    if( byte_in == 'x'
        &&
        (tb_inputs_ex2-> i_clock == 'x' || tb_inputs_ex2-> i_rx == 'x')
       ){
        // Reset condition
        tb_inputs_ex2-> char_in_count   = 0;
        tb_inputs_ex2-> i_clock         = 0;
        tb_inputs_ex2-> i_rx            = 1;        
    }else if( tb_inputs_ex2-> char_in_count%2 == 0){
        // Set clock_in
        tb_inputs_ex2->   i_clock = byte_in;
        ++tb_inputs_ex2-> char_in_count;
    }else{
        // Set rx_in
        tb_inputs_ex2->   i_rx = byte_in;
        ++tb_inputs_ex2-> char_in_count;
    }
    
    i_Uart_TB_clock_Write( tb_inputs_ex2-> i_clock  );
    i_Uart_TB_rx_Write(    tb_inputs_ex2-> i_rx     );

    UART_1_PutChar( tb_inputs_ex2-> char_in_count   );
    UART_1_PutChar( tb_inputs_ex2-> i_clock         );
    UART_1_PutChar( tb_inputs_ex2-> i_rx            );
    
    UART_1_PutChar( o_Uart_TB_RX_DV_Read()  );
    UART_1_PutChar( o_Uart_TB_RX_Byte_Read());
    
    
}//END connect_to_example_2()

void connect_to_example_3_flash(void){ 
    
    // Flash memory evaluation
    
    const int clock_rate_mhz = 79;
    const int byte_count = 25600;
    const int timer_counter_reset = 1000000000;
    
    char uart_msg[80];
    uint8 dummy_data[ byte_count ];
    for(int i=0; i < byte_count; ++i)
        dummy_data[i]=i;
    
    Timer_1_WriteCounter( timer_counter_reset );
    long long tic, toc;
    tic = Timer_1_ReadCounter();
    //CyDelayUs(10); // Verify timer calibration
    // DUT:
    Em_EEPROM_1_Write( (uint8*) dummy_data,
                       (const uint8*) 0x10000,
                        byte_count
                      );
    toc = Timer_1_ReadCounter();
    
    sprintf( uart_msg, "Written flash bytes: %d\n\r",
             byte_count
            );   
    UART_1_PutString( uart_msg );
    sprintf( uart_msg, "flash write duration: %d.",
             (int)((tic-toc)/clock_rate_mhz)
            );       
    UART_1_PutString( uart_msg );
    sprintf( uart_msg, "%2d us\n\r",
             (int)((100*(tic-toc))/79 - ((tic-toc)/clock_rate_mhz)*100)
            );       
    UART_1_PutString( uart_msg );

}//END connect_to_example_3_flash()


/* [] END OF FILE */
