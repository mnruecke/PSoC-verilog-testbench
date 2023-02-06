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
    clock = 0
    for half_clk in range(5):
        
        
        if o_byte[0] % 2 == 0:
            ser.write( bytes([clock]))
            clock   = not clock
            o_byte_ = ser.read(5) 
        else:
            ser.write( b'\x01' )   
            o_byte = ser.read(5)     
            
        print( o_byte[0],
               o_byte[1],
               o_byte[2],
               o_byte[3],
               o_byte[4],
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
    

           
    
            
                    