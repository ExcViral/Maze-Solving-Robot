/*
* This code ensures that the bot navigates from one block to
* another block avoiding crashing into walls of the maze.
*
*   current setting of the bot as of 14 Apr 2018:
*   Chassis dimensions: 13 cm x 15 cm
*   Expected single block dimensions of maze: 25 cm x 25 cm
*   
* So, if we place the bot at the centre of the centre of 
* the block, the bot at the centre of the block, it moves
* from one block, maintaing an 'almost' straight line motion.
*
* Created by Tanmay Khandait, Viral Patel
* April 16, 2018
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

//Right Motor Pins
int mR_Pin1 = 7;
int mR_Pin2 = 8;

//Left Motor Pins
int mL_Pin1 = 12;
int mL_Pin2 = 13;

// Define delay time for short turns. 
// Shorter the delay_time, smoother the callibration.
const float delay_time = 0.05;

// Range for deciding if wall is within range
const int range = 8; // cm

void setup() {

  //Right UltraSonic Sensor
  pinMode(r_trigPin, OUTPUT); // Sets trigpin as output
  pinMode(r_echoPin,INPUT); // Sets echopin as input

  //Left UltraSonic Sensor
  pinMode(l_trigPin, OUTPUT); // Sets trigpin as output
  pinMode(l_echoPin,INPUT); // Sets echopin as input

  //Front UltraSonic Sensor
  pinMode(f_trigPin, OUTPUT); // Sets trigpin as output
  pinMode(f_echoPin,INPUT); // Sets echopin as input

  //Motor Pin Initialization
  pinMode(mR_Pin1, OUTPUT);
  pinMode(mR_Pin2, OUTPUT);
  pinMode(mL_Pin1, OUTPUT);
  pinMode(mL_Pin2, OUTPUT);
  
  Serial.begin(9600); // Begin Serial communication
  Serial.println("Serial Communication is ready!");
}

void loop() {
  
  int left_distance, right_distance, front_distance;
  
  //Read character for manual Stop mechanism
  char s = Serial.read();

  //movefunction to ensure striaght line movement without colliding
  move_forward(4);
  
  //Manual Stop Mechanism
  if(s == 's') {
    stop_m();
    delay(10000);
  }
  
  //Print to Serial Window
  Serial.print("Obstacle Distance, l = ");
  Serial.print(left_distance);
  Serial.print(" r = ");
  Serial.print(right_distance);
  Serial.print(" f = "); 
  Serial.println(front_distance); 
}

/*
* Function to make the robot move forward, avoiding collisions.
* This function uses the callibrate function to realign the robot
* it crosses certain threshold and goes near any wall.
* This function also implements wall following logic.
* This function will stop the robot if the front sensor reads a
* value below the threshold
*/

void move_forward(int threshold) {
  
  int l, r, f;
  l = m_ls(); // measure left sensor reading
  r = m_rs(); // measure right sensor reading
  f = m_fs(); // measure front sensor reading
  
  if(l <= threshold && r <= range) {
    calibrate(l, r);
  }
  else if(r <= threshold && l <= range) {
    calibrate(l, r);
  }
  else if(l <= threshold && r >= range) {
    calibrate_singlewall_left(l, threshold);
  }
  else if(r <= threshold && l >= range) {
    calibrate_singlewall_right(r, threshold);
  }
  else if(f < threshold) {
    stop_m();
  }
  else {
    mforward();
  }
  
}


/* 
* Function that callibrates the motion of the bot so as to avoid
* running into walls, that is, this implements collision avoidance
* alongwith trying to maintain straight motion between walls.
* 
* if (distance from left wall > distance from right wall) then
* Bot is going towards right wall. We need to stop the motion of 
* left motors in order to turn it to right.
*  
* if (distance from left wall < distance from right wall) then
* Bot is going towards left wall. We need to stop the motion of 
* left motors in order to turn it to left.
*  
* Delay_time is the time the motors remain off for. 
* less the dealy_time, more smooth the callibration is.
*/

void calibrate(int left_distance,int right_distance){
  if(left_distance<right_distance){
    stop_rm(delay_time);
  }  
  else if(left_distance>right_distance){
    stop_lm(delay_time);
  }
}

void calibrate_singlewall_left(int distance,int threshold){
  if(distance < threshold){
    stop_rm(delay_time);
  }  
  else if(distance > threshold){
    stop_lm(delay_time);
  }
}

void calibrate_singlewall_right(int distance,int threshold){
  if(distance < threshold){
    stop_lm(delay_time);
  }  
  else if(distance > threshold){
    stop_rm(delay_time);
  }
}

/*
* Functions to measure distance of wall from front, left and right
* the robot's side.
* Function naming convention: 'm' stands for measure,
* 'ls','rs','fs' stands for left sensor, right sensor
* and front sensor respectively.
* 
* Function:
*   Return type: int, returns distance in cm
*   Params : none
*/

int m_ls() {
  
  long duration;
  int distance;
  
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

  return distance;  
}

int m_rs() {
  
  long duration;
  int distance;
  
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
  
  return distance;  
}

int m_fs() {
  
  long duration;
  int distance;
  
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

  return distance;  
}

/*
* Functions to control movements of the robot.
* We have decided to give the robot three types of controls,
* Forward, Turn-90-Left, Turn-90-Right, stop
* Function nomenclature:
*   m : move, t : turn 
* 
* Function :
*   Return type : none
*   Params : none
*/

void mforward() {
  digitalWrite(mR_Pin1, LOW);
  digitalWrite(mR_Pin2, HIGH);
  digitalWrite(mL_Pin1, HIGH);
  digitalWrite(mL_Pin2, LOW);
}

/*
* Function to turn left, right 90 degrees
*   Return type : none
*   Param : d, time after which turning should stop
*
*   Due to unavailability of stepper motors, and 
*   sensors to measure degree of rotation, we had to
*   measure the time taken by robot to make 90 degrees of turns
*   that time is represented by parameter d.
*/

void tleft_90(float d) {
  digitalWrite(mR_Pin1, LOW);
  digitalWrite(mR_Pin2, HIGH);
  digitalWrite(mL_Pin1, LOW);
  digitalWrite(mL_Pin2, HIGH);  
  int delays = d * 1000;
  delay(delays);
  stop_m();
}

void tright_90(float d) {
  digitalWrite(mR_Pin1, HIGH);
  digitalWrite(mR_Pin2, LOW);
  digitalWrite(mL_Pin1, HIGH);
  digitalWrite(mL_Pin2, LOW);  
  int delays = d * 1000;
  delay(delays);
  stop_m();
}

void stop_m() {
  digitalWrite(mR_Pin1, LOW);
  digitalWrite(mR_Pin2, LOW);
  digitalWrite(mL_Pin1, LOW);
  digitalWrite(mL_Pin2, LOW);  
}

/*
* The below functions will be used to make slight turns,
* in either directions, left or right, but not both.
* The turn is achieved by stopping any motor in the direction
* we want to make a turn, for duration 'd'.
* After some trials and errors, we have decided that for slight 
* turns, the paramater sd = 0.1 s will be fine. anything more than
* that will increase turn angle and will result into zig zag motion.
* 
*/

void stop_lm(float d) {
  digitalWrite(mL_Pin1, LOW);
  digitalWrite(mL_Pin2, LOW);
  int delays = d * 1000;
  delay(delays);
  mforward();
}

void stop_rm(float d) {
  digitalWrite(mR_Pin1, LOW);
  digitalWrite(mR_Pin2, LOW);
  int delays = d * 1000;
  delay(delays);
  mforward();
}
