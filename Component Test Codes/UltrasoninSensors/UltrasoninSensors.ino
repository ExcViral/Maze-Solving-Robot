/*
* This code is to test if the ultrasonic sensors on the robot are working
* as expected.
* In our robot, we have three ultrasonic sensors, one on the
* front, and one each on left and right side of the robot.
* The ultrasonic sensors have to detect if there is a wall
* present at respective side of the robot.
*
* current setting of the robot as of 14 Apr 2018:
*   Chassis dimensions: 13 cm x 15 cm
*   Expected single block dimensions of maze: 25 cm x 25 cm
* So, if we place the robot at the centre of the centre of 
* the block, we need the clearance of 6 cm each on left &
* right side, and 6 cm clearance on the front.
*
* Created by Viral Patel
* April 14, 2018
*/

// Define Pin numbers for sensors
// Right sensors
const int r_trigPin = 2;
const int r_echoPin = 3;
// left sensors
const int l_trigPin = 4;
const int l_echoPin = 5;
// Front sensors
const int f_trigPin = 9;
const int f_echoPin = 10;

// Define Variables
// long duration;
// int distance;

void setup() {

  pinMode(r_trigPin, OUTPUT); // Sets trigpin as output
  pinMode(r_echoPin,INPUT); // Sets echopin as input
  pinMode(l_trigPin, OUTPUT); // Sets trigpin as output
  pinMode(l_echoPin,INPUT); // Sets echopin as input
  pinMode(f_trigPin, OUTPUT); // Sets trigpin as output
  pinMode(f_echoPin,INPUT); // Sets echopin as input
  
  Serial.begin(9600); // Begin Serial communication
}

void loop() {
  
  int l, r, f;
  l = m_ls();
  r = m_rs();
  f = m_fs();
  
  Serial.print("Obstacle Distance, l = ");
  Serial.print(l);
  Serial.print(" r = ");
  Serial.print(r);
  Serial.print(" f = "); 
  Serial.println(f); 
}

/*
* Functions to measure if wall is within 6 cms of any of
* the robot's side.
* Function naming convention: 'm' stands for measure,
* 'ls','rs','fs' stands for left sensor, right sensor
* and front sensor respectively.
* 
* Function:
*   Return type: int, returns 1 if there is wall within 6 cm,
*    and 0 if wall is not within 6 cm.
*   Params : none
*/

int m_ls() {
  
  long duration;
  int distance;
  int flag;
  // first clear the trigPin
  digitalWrite(l_trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(l_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(l_trigPin, LOW);
  
  // Read the travel time of pulse
  duration = pulseIn(l_echoPin, HIGH);
  // Calculate the distance based on duration
  distance = duration*0.034/2;
  delayMicroseconds(10);

  if(distance < 8) {
    flag = 1;
  }
  else {
    flag = 0;
  } 

  return flag;  
}

int m_rs() {
  
  long duration;
  int distance;
  int flag;
  
  // first clear the trigPin
  digitalWrite(r_trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(r_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(r_trigPin, LOW);
  
  // Read the travel time of pulse
  duration = pulseIn(r_echoPin, HIGH);
  // Calculate the distance based on duration
  distance = duration*0.034/2;
  delayMicroseconds(10);

  if(distance < 8) {
    flag = 1;
  }
  else {
    flag = 0;
  }
  
  return flag;  
}

int m_fs() {
  
  long duration;
  int distance;
  int flag;
  // first clear the trigPin
  digitalWrite(f_trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(f_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(f_trigPin, LOW);
  
  // Read the travel time of pulse
  duration = pulseIn(f_echoPin, HIGH);
  // Calculate the distance based on duration
  distance = duration*0.034/2;
  delayMicroseconds(10);
  
  if(distance < 8) {
    flag = 1;
  }
  else {
    flag = 0;
  }
 
  return flag;  
}
