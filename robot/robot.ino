/*
* This is the final source file which is to be written in the
* Arduino uno, used in our robot.
* All the functions used in this source file, have been developed
* and tested independently. The files for all these test codes
* can be found in the directory Component Test Codes/
*
* Created by Viral Patel
* April 16, 2018
*/

// mpu 6050

#include <MPU6050.h>
#include <Wire.h>
MPU6050 mpu;

// Timers
unsigned long timer = 0;
float timeStep = 0.01;

// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;
float yaw = 0;

// Pin initializations

// Ultrasonic sensors

// Right sensors
const int r_trigPin = 2;
const int r_echoPin = 3;
// left sensors
const int l_trigPin = 4;
const int l_echoPin = 5;
// Front sensors
const int f_trigPin = 9;
const int f_echoPin = 10;

// IR sensor
const int irPin = 6;

// Motor control board

// Left motor
const int mR_Pin1 = 7;
const int mR_Pin2 = 8;
// Right motor
const int mL_Pin1 = 12;
const int mL_Pin2 = 13;

// Constant declarations

// ultrasonic sensors
const int range = 8; // Range within which wall is to be checked
const int r_freq = 2; // how many readings to take?
// motor control
const float tp = 0.1; // Turn parameter in seconds, used to make 90 deg turns
const float stp = 0.1; // Slight turn parameter, used to make slight turns
const float sstp = 0.5; // Slight turn parameter, used to make sharp slight turns
const int threshold = 4; // Threshold for calibrating forward motion within wall(s)
// ir sensors
const float sd = 0.2; // Stopping delay to allow robot to reach center

// Global logic variables
// flags for turing
int l = 0, r = 0;
int make_move = 0;

void setup() {
  
  // Open serial communication
  Serial.begin(115200);
  
  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  mpu.calibrateGyro();
  mpu.setSleepEnabled(true);

  // Setting up pins
  // Ultrasonic Sensors
  pinMode(r_trigPin, OUTPUT); // Sets trigpin as output
  pinMode(r_echoPin,INPUT); // Sets echopin as input
  pinMode(l_trigPin, OUTPUT); // Sets trigpin as output
  pinMode(l_echoPin,INPUT); // Sets echopin as input
  pinMode(f_trigPin, OUTPUT); // Sets trigpin as output
  pinMode(f_echoPin,INPUT); // Sets echopin as input 
  // IR sensors
  pinMode(irPin, INPUT);
  // Motor board pins
  pinMode(mR_Pin1, OUTPUT);
  pinMode(mR_Pin2, OUTPUT);
  pinMode(mL_Pin1, OUTPUT);
  pinMode(mL_Pin2, OUTPUT);
}

void loop() {
   int rotationAngle = checkGyro();
   /*
   * Function Map
   * a    Move Forward
   * b    Take Sensor Reading
   * c    Check Center
   * d    turnLeft_90
   * e    turnRight_90
   * f    turn_180
   * g    stop motor
   * def  stop motor
   */
  if(Serial.available() > 0) {
    char ch = Serial.read();
    if(ch=='a'){
      make_move = 1;
    }
    else if(ch=='b'){
      takeSensorReading();
    }
    else if(ch=='c'){
      checkCenter();
    }
    else if(ch=='d'){
        yaw = 0;
        mpu.setSleepEnabled(false);
        l = 1;
    }
    else if(ch=='e'){
        yaw = 0;
        mpu.setSleepEnabled(false);
        r = 1;
    }
    else if(ch=='f'){
      turn_180();
    }
    else if(ch=='g'){
      make_move = 0;
      l = 0;
      r = 0;
      stop_m();
    }
    else if(ch=='h'){
      
    }
  }
  if(make_move == 1) {
    move_forward();
  }
  else if(l == 1) {
    tleft_90();
    if(rotationAngle > 60) {
      mleft.stopm();
      mright.stopm();
      l = 0;
      mpu.setSleepEnabled(true);
      yaw = 0;
    }
  }
  else if(r == 1) {
    tright_90();
    if(rotationAngle < -60) {
      mleft.stopm();
      mright.stopm();
      r = 0;
      mpu.setSleepEnabled(true);
    }
  }
}

// Higher level function declarations

// *** function to move forward avoiding walls
void move_forward() {
  
  int l,r, f;
  l = m_ls(); // measure left sensor reading
  r = m_rs(); // measure right sensor reading
  f = m_fs(); // measure front sensor reading
  
  /*Serial.print(l);
  Serial.print("  ");
  Serial.print(r);
  Serial.print("  ");
  Serial.println(f);
  */
  if(f <= threshold) {
    stop_m();
  }
  else if(l < threshold && r < range) {
    calibrate(l, r);
  }
  else if(r < threshold && l < range) {
    calibrate(l, r);
  }
  else if(l < threshold && r > range) {
    calibrate_singlewall_left(l, threshold);
  }
  else if(r < threshold && l > range) {
    calibrate_singlewall_right(r, threshold);
  }
  else if(l < range && r > range) {
    calibrate_singlewall_left(l, threshold);
  }
  else if(r < range && l > range) {
    calibrate_singlewall_right(r, threshold);
  }
  else if(l > range && r > range){
    mforward();
  } 
}
// *** calibrate when both walls are in range
void calibrate(int left_distance,int right_distance){
  if(left_distance<right_distance){
    stop_rm(stp);
  }  
  else if(left_distance>right_distance){
    stop_lm(stp);
  }
}
// *** calibrate if only left wall is in range
void calibrate_singlewall_left(int distance,int threshold){
  if(distance < threshold){
    stop_rm(stp);
  }  
  else if(distance > threshold){
    stop_lm(stp);
  }
}
// *** calibrate if only right wall is in range
void calibrate_singlewall_right(int distance,int threshold){
  if(distance < threshold){
    stop_lm(stp);
  }  
  else if(distance > threshold){
    stop_rm(stp);
  }
}

// Basic level function declarations

// Ultrasonic Sensors

// *** check all three sensor readings and return a string
void takeSensorReading() {
  int f = isWallOnFront();
  int r = isWallOnRight();
  int l = isWallOnLeft();
  int reading = (l*100)+(f*10)+r;
  Serial.println(reading);
}
// *** check if wall is within range on left side
int isWallOnLeft() {
  float flag = 0;
  for(int i = 0; i < r_freq; i++) {
    int distance = m_ls();
    if(distance <= range) {
      flag++;
    }
  }
  flag = flag/r_freq;
  if(flag >= 0.8) {
    return 1;
  }
  return 0;
}
// *** check if wall is within range on right side
int isWallOnRight() {
  float flag = 0;
  for(int i = 0; i < r_freq; i++) {
    int distance = m_rs();
    if(distance <= range) {
      flag++;
    }
  }
  flag = flag/r_freq;
  if(flag >= 0.8) {
    return 1;
  }
  return 0;
}
// *** check if wall is within range in front
int isWallOnFront() {
  float flag = 0;
  for(int i = 0; i < r_freq; i++) {
    int distance = m_fs();
    if(distance <= range) {
      flag++;
    }
  }
  flag = flag/r_freq;
  if(flag >= 0.8) {
    return 1;
  }
  return 0;
}
// *** measure left sensor reading
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
// *** measure right sensor reading
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
// *** measure forward sensor reading
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

// IR Sensors

// *** check if robot is at center of block
// **** returns LOW for black, HIGH for not black
void checkCenter() {
  int ir_reading;
  ir_reading = digitalRead(irPin);
  delay(sd);
  Serial.println(ir_reading);
}

// Motor Control Board

// *** straight forward
void mforward() {
  digitalWrite(mR_Pin1, LOW);
  digitalWrite(mR_Pin2, HIGH);
  digitalWrite(mL_Pin1, HIGH);
  digitalWrite(mL_Pin2, LOW);
}
// *** turn left 90 deg
void tleft_90() {
  digitalWrite(mR_Pin1, LOW);
  digitalWrite(mR_Pin2, HIGH);
  digitalWrite(mL_Pin1, LOW);
  digitalWrite(mL_Pin2, HIGH);  
}
// *** turn right 90 deg
void tright_90() {
  digitalWrite(mR_Pin1, HIGH);
  digitalWrite(mR_Pin2, LOW);
  digitalWrite(mL_Pin1, HIGH);
  digitalWrite(mL_Pin2, LOW);  
}

// *** turn slight left by stopping left motor for duration = d
void stop_lm(float d) {
  digitalWrite(mL_Pin1, LOW);
  digitalWrite(mL_Pin2, LOW);
  int delays = d * 1000;
  delay(delays);
  mforward();
}
// *** turn slight right by stopping left motor for duration = d
void stop_rm(float d) {
  digitalWrite(mR_Pin1, LOW);
  digitalWrite(mR_Pin2, LOW);
  int delays = d * 1000;
  delay(delays);
  mforward();
}
// *** stop motors, simple brake
void stop_m() {
  digitalWrite(mR_Pin1, LOW);
  digitalWrite(mR_Pin2, LOW);
  digitalWrite(mL_Pin1, LOW);
  digitalWrite(mL_Pin2, LOW);  
}

/***
* The below function will be used to check angle of rotation on 
* the axis of turning.
* The code was adapted from: 
* http://www.electronicwings.com/arduino/mpu6050-interfacing-with-arduino-uno
*/

int checkGyro() {
  if(!mpu.getSleepEnabled()) {
    timer = millis();
    // Read normalized values
    Vector norm = mpu.readNormalizeGyro();
    // Calculate Pitch, Roll and Yaw
    pitch = pitch + norm.YAxis * timeStep;
    roll = roll + norm.XAxis * timeStep;
    yaw = yaw + norm.ZAxis * timeStep;
    // Wait to full timeStep period
    Serial.println(yaw);
    delay((timeStep*1000) - (millis() - timer));
    return yaw;
  }
}

