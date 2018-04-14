## This code will help you to find the turning parameter d
## This code was meant to be run on a raspberry pi

import serial

# Open serial communication
ser = serial.Serial('/dev/ttyACM0',9600)

while 1:
    
    # Read serial data sent by arduino
    read_serial = ser.readline()
    print(read_serial)
    # Ask value for turning parameter d from user, write it on serial
    b = input("Enter value of d")
    strb = str(b)
    ser.write(strb.encode())
