# -*- coding: utf-8 -*-
"""
Created on Fri Jan 27 16:23:17 2023

@author: marti
"""

import serial
import time
import struct
import os
import os.path

import numpy as np
import matplotlib.pyplot as plt


""" main settings and definitions """

# Serial port:
serialPort = '\\\\.\\COM10'
baudrate = 115200
time_out = 1

# Test routines
def run_example_1():
    # Testbench input data, basic example
    
    uw_tb_input_lsb  = 0b_1010_1111
    
    for repetition_i in range( 1 ):
    
        print( "tb input port lsb:\t\t{}".format( bin(i_tb_input_lsb)))
        
        ser.write( bytes([ uw_tb_input_lsb ]))
        time.sleep( 0.005 )     
        ur_tb_output_msb = ser.read(1)        
        ur_tb_output_lsb = ser.read(1)        
        
        ur_tb_output_msb = int.from_bytes( ur_tb_output_msb, 'big' )
        print( "tb output port msb:\t\t{}".format( bin(ur_tb_output_msb)))
        ur_tb_output_lsb = int.from_bytes( ur_tb_output_lsb, 'big' )
        print( "tb output port lsb:\t\t{}".format( bin(ur_tb_output_lsb)))
        
        #print( "expected output: \t\t{}".format( bin( i_tb_input_lsb**2 )))    

def run_example_2():
    # Testbench input data, uart rx example
    # ur_X: uart_read_X; uw_X: uart_write_X
    uw_reset = b'x' 
    
    # 1) Reset psoc routine
    def reset_test_routine():
        ser.write( uw_reset )   
        ur_byte = ser.read(5)
        return ur_byte
    
    ur_byte = reset_test_routine()
    if ur_byte[0] != 0:
        ur_byte = reset_test_routine()

    print( ur_byte[0],
           ur_byte[1],
           ur_byte[2],
           ur_byte[3],
           ur_byte[4],
    )
    
    # 2) Generate test pattern for uart rx
    tb_test_byte = 0b_1111_0011
    
    bit_0_low  = b'\x00'
    bit_0_high = b'\x01'

    tx_start = 10
    data_bit_idx = 0
    clocks_per_bit = 3
    clock = 0
    clock_count = 0
    uw_i_rx = bit_0_high
    last_clock_level = ur_byte[1]
    state = 'idle'
    data_bit_idx = 0
    
    for tb_clk in range(172):
            
        if tb_clk % 2 == 0:
            
            # Generate clock cycle
            clock   = not clock     
            clock_count += clock
            uw_i_clock = bytes([clock])
            ser.write( bytes( uw_i_clock ))
            ur_byte = ser.read(5) 
            
        else:
            
            # Determine state
            if clock_count < tx_start:
                state = 'idle'
                data_bit_idx = 0
            elif (clock_count >= tx_start
                  and clock_count < tx_start + clocks_per_bit
            ):
                state = 'startbit'
                data_bit_idx = 0
            elif (clock_count >= tx_start + clocks_per_bit
                  and clock_count < tx_start + 9*clocks_per_bit
            ):
                state = 'databits'
                data_bit_idx = (clock_count-tx_start) // clocks_per_bit -1
            elif clock_count >= tx_start + 9*clocks_per_bit:
                state = 'stopbit'
                data_bit_idx = 0
                        
            # Set rx bit       
            if state == 'idle':
                uw_i_rx = bit_0_high
            
            if state == 'startbit':
                uw_i_rx = bit_0_low
                
            if state == 'databits':
                uw_i_rx = bytes([(tb_test_byte >> data_bit_idx) & 1])
                
            if state == 'stopbit':
                uw_i_rx = bit_0_high
            
            ser.write( uw_i_rx )   
            ur_byte = ser.read(5)    
            
            
        if last_clock_level == 0 and ur_byte[1] ==1:
            print("clock posedge: -------------------------")
        last_clock_level = ur_byte[1]
            
        #'''
        print( 
               clock_count,
               'i_clock:',      ur_byte[1],  # i_Uart_clock state
               'i_rx:',         ur_byte[2],  # i_Uart_rx state
               'o_DV:',         ur_byte[3],  # o_Uart_DV state
               'o_Byte:', bin(  ur_byte[4]), # o_Uart_Byte
        )
        #'''



""" END - main settings and definitions """


""" interact with testbench on PSoC """
try: # open and interact with serial port 

    ser = serial.Serial( serialPort, baudrate, timeout=time_out )
    ser.flushInput()
    time.sleep( 0.05 )
    
    run_example_2()            

finally: # close serial port
    ser.close()
    

           
    
            
                    