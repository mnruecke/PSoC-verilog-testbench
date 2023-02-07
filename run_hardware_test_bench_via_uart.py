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
    
    i_tb_input_lsb  = 0b_1010_1111
    
    for repetition_i in range( 1 ):
    
        print( "tb input port lsb:\t\t{}".format( bin(i_tb_input_lsb)))
        
        ser.write( bytes([ i_tb_input_lsb ]))
        time.sleep( 0.005 )     
        o_tb_output_msb = ser.read(1)        
        o_tb_output_lsb = ser.read(1)        
        
        o_tb_output_msb = int.from_bytes( o_tb_output_msb, 'big' )
        print( "tb output port msb:\t\t{}".format( bin(o_tb_output_msb)))
        o_tb_output_lsb = int.from_bytes( o_tb_output_lsb, 'big' )
        print( "tb output port lsb:\t\t{}".format( bin(o_tb_output_lsb)))
        
        #print( "expected output: \t\t{}".format( bin( i_tb_input_lsb**2 )))    

def run_example_2():
    # Testbench input data, uart rx example
    
    # 1) Reset psoc routine
    def reset_test_routine():
        ser.write( b'x' )   
        o_byte = ser.read(5)
        return o_byte
    
    o_byte = reset_test_routine()
    if o_byte[0] != 0:
        o_byte = reset_test_routine()

    print( o_byte[0],
           o_byte[1],
           o_byte[2],
           o_byte[3],
           o_byte[4],
    )
    
    # 2) Generate test pattern for uart rx

    clocks_per_bit = 3
    clock = 0
    clock_count = 0
    r_rx_in = b'\x01'
    tb_test_byte = bytes([ 0b_01010011 ])
    tb_state = ['idle', 'startbit', 'databits', 'stopbit']
    last_clock_level = o_byte[1]
    for tb_clk in range(172):
             
        if tb_clk % 2 == 0:
            clock   = not clock     
            clock_count += clock
            ser.write( bytes([clock]) )
            o_byte_ = ser.read(5) 
        else:
            if clock_count == 10: # generate start bit
                r_rx_in = b'\x00'
            if clock_count == 10 + (1+0)*clocks_per_bit: # send 0xff
                r_rx_in = b'\x01'
            ser.write( r_rx_in )   
            o_byte = ser.read(5)     
            
        if last_clock_level == 0 and o_byte[1] ==1:
            print("clock posedge: -------------------------")
        last_clock_level = o_byte[1]
            
        print( 
               clock_count,
               '\t',        o_byte[1],  # i_Uart_clock state
               'rx: ',      o_byte[2],  # i_Uart_rx state
               '\tDV: ',    o_byte[3],  # o_Uart_DV state
               '\t',    bin(o_byte[4]), # o_Uart_Byte
        )
        



""" END - main settings and definitions """


""" interact with testbench on PSoC """
try: # open and interact with serial port 

    ser = serial.Serial( serialPort, baudrate, timeout=time_out )
    ser.flushInput()
    time.sleep( 0.05 )
    
    run_example_2()            

finally: # close serial port
    ser.close()
    

           
    
            
                    