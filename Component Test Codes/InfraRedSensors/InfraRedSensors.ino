/*
* This code is to test if the infrared sensors on the robot 
* are working as expected.
* In our robot, we have one infrared sensor at the back.
* The job of IR sensor is to tell the robot whether it is at the 
* center of the block or not.
* The movements and actions of the robot will depend upon the
* readings given by the IR sensor.
*
* IR sensor gives HIGH reading when on Black surface.
* On non-black surface, the sensor gives LOW reading.
*
* Created by Viral Patel
* April 14, 2018
*/

int irPin = 6; // Connect IR sensor to pin 6 

void setup() {
  pinMode(irPin, INPUT);
  Serial.begin(9600); // Begin serial communication
}

void loop() {
  int flag = checkCenter();
  
  if(flag == HIGH) {
    Serial.println("The robot is at the center!");
  }
  else {
    Serial.println("The robot is not at center!");
  }
}

/*
* Function to check whether the robot has reached the center
* of the maze block or not. 
* For simplicity, we have assumed that the maze will be 
* will have a black filled circle at the center of each block
* in the maze. The job of the IR sensor is to detect whenever 
* the robot reached the center of any block inside the maze.
* This technique will help us to implement localization,
* i.e. the robot will be able to know which square block it 
* is currently in. This sensor will also assist the robot to 
* make turns without colliding with any of the walls.
*
* Function : 
*   Return type : int, returns LOW when robot reaches center, 
*     and HIGH if robot if not at center.
*   Params : none
*/

int checkCenter() {
  int ir_reading;
  ir_reading = digitalRead(irPin);
  return ir_reading;
}
