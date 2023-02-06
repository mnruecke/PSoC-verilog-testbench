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
serialPort = '\\\\.\\COM10' # USBFS 8; UART 22 
baudrate = 115200
time_out = 1


# Testbench input data
i_tb_input     = bytes([ 0b_1010_1111 ])


""" END - main settings """


""" interact with testbench on PSoC """
try: # open and interact with serial port 

    ser = serial.Serial( serialPort, baudrate, timeout=time_out )
    ser.flushInput()
    
    
    for repetition_i in range( 1 ):
    
        print( "tb input port:\t{}".format( 
                    bin(int.from_bytes( i_tb_input, 'big'))
        ))
        
        ser.write( i_tb_input )
        time.sleep( 0.005 )     
        o_tb_output = ser.read(1)        
        
        print( "tb output port:\t{}".format(
                    bin(int.from_bytes( o_tb_output, 'big' ))
        ))
                    

finally: # close serial port
    ser.close()
    

           
    
            
                    