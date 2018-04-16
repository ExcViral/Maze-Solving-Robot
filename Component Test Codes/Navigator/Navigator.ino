/*
* This code ensures that the bot navigates from one block to another block
* without crashing into walls. The code tries to give an 'almost' straight
* motion to the bot. 
*
*   current setting of the bot as of 14 Apr 2018:
*   Chassis dimensions: 13 cm x 15 cm
*   Expected single block dimensions of maze: 25 cm x 25 cm
*   
* So, if we place the bot at the centre of the centre of 
* the block, the bot at the centre of the block, it moves
* from one block, maintaing an 'almost' straight line motion.
*
* Tanmay Khandait
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

// Define Variables
// long duration;
// int distance;

// Define delay time for short turns. 
// Shorter the delay_time, smoother the callibration.
const float delay_time = 0.05;

//Right Motor Pins
int mR_Pin1 = 7;
int mR_Pin2 = 8;

//Left Motor Pins
int mL_Pin1 = 12;
int mL_Pin2 = 13;


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
}

void loop() {
  
  int left_distance, right_distance, front_distance;

  //Call Fucntion to read Ultrasonic Sensors
  left_distance = measure_left_sensor();
  right_distance = measure_right_sensor();
  front_distance = measure_front_sensor();
  
  //Read character for manual Stop mechanism
  char s = Serial.read();

  //Callibrate function to ensure striaght line movement
  calibrate(left_distance,right_distance);
  
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


/*  Function that callibrates the motion of the bot inin order to avoid running into walls.
 *  This will also make sure that the bot moves from one block to another block in as straight line as possible.
 *  The theory is that:
 *  
 *  if (distance from left wall > distance from right wall) then
 *  Bot is going towards right wall. We need to stop the motion of left motors in order to turn it to right.
 *  
 *  if (distance from left wall < distance from right wall) then
 *  Bot is going towards left wall. We need to stop the motion of left motors in order to turn it to left.
 *  
 *  Delay_time is the time the motors remain off for. less the dealy_time, more smooth the callibration is.
 *  
 *  Also stop bot if reading from any of the sensors of are more than 25 cm.
 */
void calibrate(int left_distance,int right_distance){
  
  if(left_distance<right_distance){
    stop_rm(delay_time);
  }  
  else if(left_distance>right_distance){
    stop_lm(delay_time);
  }
  else{
    if(left_distance>=25 || right_distance>=25){
      stop_m();
    }
  }
}

int measure_left_sensor() {
  
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

  return distance;  
}

int measure_right_sensor() {
  
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
  
  return distance;  
}

int measure_front_sensor() {
  
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
 
  return distance;  
}


//Motor Move Forward
void mforward() {
  digitalWrite(mR_Pin1, LOW);
  digitalWrite(mR_Pin2, HIGH);
  digitalWrite(mL_Pin1, HIGH);
  digitalWrite(mL_Pin2, LOW);
}

//Function to stop motor
void stop_m() {
  digitalWrite(mR_Pin1, LOW);
  digitalWrite(mR_Pin2, LOW);
  digitalWrite(mL_Pin1, LOW);
  digitalWrite(mL_Pin2, LOW);  
}

//Function to stop left motor for delay time 'd'
void stop_lm(float d) {
  digitalWrite(mL_Pin1, LOW);
  digitalWrite(mL_Pin2, LOW);
  int delays = d * 1000;
  delay(delays);
  mforward();
}

//Function to stop right motor for delay time 'd'
void stop_rm(float d) {
  digitalWrite(mR_Pin1, LOW);
  digitalWrite(mR_Pin2, LOW);
  int delays = d * 1000;
  delay(delays);
  mforward();
}


