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


""" main settings """

# Serial port:
serialPort = '\\\\.\\COM10'
baudrate = 115200
time_out = 1


# Testbench input data
i_tb_input_lsb  = 0b_1010_1111


""" END - main settings """


""" interact with testbench on PSoC """
try: # open and interact with serial port 

    ser = serial.Serial( serialPort, baudrate, timeout=time_out )
    ser.flushInput()
    
    
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
                

finally: # close serial port
    ser.close()
    

           
    
            
                    