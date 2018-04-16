# This is the driver file for the maze solver robot
# This code is meant to be run on a rapsberry pi
# Please refer to [https://github.com/ExcViral/Maze-Solving-Robot] for more information

import serial

# open serial communication
ser = serial.Serial('/dev/ttyACM0',9600)

# below are the driver function definations
def moveForward():
	ch = 'a'
	ser.write(ch.encode())

def takeSensorReadings():
	ch = 'b'
	ser.write(ch.encode())

def checkIfCenter():
	ch = 'c'
	ser.write(ch.encode())

def turnLeft():
	ch = 'd'
	ser.write(ch.encode())

def turnRight():
	ch = 'e'
	ser.write(ch.encode())

def turnBack():
	ch = 'f'
	ser.write(ch.encode())

def brake():
	ch = 'g'
	ser.write(ch.encode())

def driver():
	checkIfCenter()
	readSerial = ser.readLine()
	print(readSerial)

while 1:
	driver()