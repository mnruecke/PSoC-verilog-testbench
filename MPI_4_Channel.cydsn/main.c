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

#define LINE_LENGTH         180
#define WAVE_PART_LENGTH    100
#define WAVE_LENGTH         40000 // 100 * 400

#define FLASH_CH1_PTR       (uint8 *) 20000u
#define FLASH_CH2_PTR       (uint8 *) 0x10000
#define FLASH_CH3_PTR       (uint8 *) 0x20000
#define FLASH_CH4_PTR       (uint8 *) 0x30000


struct wave_descriptor
{
    int     wave_length;
    int     wave_part_num;
    uint8   wave_part[WAVE_PART_LENGTH];
    
    uint8 * flash_addr;
    
    float   amplitude;
    float   omega;
    float   phi;
} wave1, wave2, wave3, wave4;


static const char CYCODE flash_userdata[230000]={0};

void flash_consistency(  uint8 * flash_end_config, uint8 * flash_start_data);
CY_ISR_PROTO( isr_rx_handle );
void uart_analog(float value1, float value2);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    char message[LINE_LENGTH];
    
    int i,j,k,l;
    uint8 wave_part[100];
    float value1, value2; 


    wave1.flash_addr = FLASH_CH1_PTR;
    wave1.flash_addr = FLASH_CH2_PTR;
    wave1.flash_addr = FLASH_CH3_PTR;
    wave1.flash_addr = FLASH_CH4_PTR;

    
    UART_1_Start();
    isr_rx_StartEx( isr_rx_handle );
    
    flash_consistency( (uint8 *) flash_userdata, (uint8 *) wave1.flash_addr);
       
     
    sprintf( message, "Hello World! %d\n", sizeof( message ));
    Em_EEPROM_1_Write( (uint8 *) message, wave1.flash_addr, sizeof("Hello World! %d\n") );
    
    UART_1_PutString( (char *) wave1.flash_addr ); UART_1_PutChar( 13 );


    for(;;)
    {
        /* Place your application code here. */

    }
}


void flash_consistency( uint8 * flash_end_config, uint8 * flash_start_data)
{
    if( (uint8 *) flash_end_config > (uint8 *) flash_start_data )
    {
        LED_Write( 1u );
        UART_1_PutString("Flash Access Error!\n"); UART_1_PutChar( 13 );
        UART_1_PutString("System Halts\n"); UART_1_PutChar( 13 );
        while(1);
    }
}


CY_ISR( isr_rx_handle )
{ 
    char ch;
    char msg[LINE_LENGTH];
    int  n_buffer_content;    

    
    n_buffer_content = UART_1_GetRxBufferSize();
    
    
    if( n_buffer_content >= 15)
    {
    
        sprintf( msg, "\n\nBuffer Size: %d\n\n", n_buffer_content );
        UART_1_PutString( msg ); UART_1_PutChar( 13 );
        
        while( 0 < n_buffer_content-- )
            UART_1_PutChar( UART_1_GetChar());
        
        UART_1_ClearRxBuffer();
        
        LED_Write(1); CyDelay(50); LED_Write(0); CyDelay(50);
    }
}





void uart_analog(float value1, float value2)
{
    char analog_bar[LINE_LENGTH + 1];
    float rescale = 2.0;
    
    if( value1/rescale > (float)LINE_LENGTH ) value1 = (float)LINE_LENGTH*rescale;
    if( value2/rescale > (float)LINE_LENGTH ) value2 = (float)LINE_LENGTH*rescale;    
    
    memset( analog_bar, ' ', LINE_LENGTH);
    memset( &analog_bar[(int)(value1/rescale)], '+', 1);
    memset( &analog_bar[(int)(value2/rescale)], 'o', 1);
    UART_1_PutChar( 13 );
    UART_1_PutString( analog_bar );
}

/* [] END OF FILE */
