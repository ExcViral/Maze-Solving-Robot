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
const int r_freq = 20; // how many readings to take?
// motor control
const float tp = 0.8; // Turn parameter in seconds, used to make 90 deg turns
const float stp = 0.1; // Slight turn parameter, used to make slight turns
const float sstp = 0.5; // Slight turn parameter, used to make sharp slight turns
const int threshold = 4; // Threshold for calibrating forward motion within wall(s)
// ir sensors
const float sd = 0.2; // Stopping delay to allow robot to reach center
void setup() {
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

  // Open Serial communication
  Serial.begin(9600); 
  Serial.println("Serial is ready!!");
}

void loop() {
   /*
   * Switch Case Function Map
   * a    Move Forward
   * b    Take Sensor Reading
   * c    Check Center
   * d    turnLeft_90
   * e    turnRight_90
   * f    turn_180
   * g    stop motor
   * def  stop motor
   */
  char ch = Serial.read();
  switch (ch){
    case 'a':
        move_forward();
      break;
    case 'b':
        takeSensorReading();
      break;
    case 'c':
        checkCenter();
      break;
    case 'd':
        tleft_90();
      break;  
    case 'e':
      tright_90();
      break;
    case 'f':
      turn_180();
      break;
    case 'g':
      stop_m();
      break;
    default:
      // stop_m();
      break;
  } 
}

// Higher level function declarations

// *** function to move forward avoiding walls
void move_forward() {
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
    stop_rm(sstp);
  }  
  else if(distance > threshold){
    stop_lm(sstp);
  }
}
// *** calibrate if only right wall is in range
void calibrate_singlewall_right(int distance,int threshold){
  if(distance < threshold){
    stop_lm(sstp);
  }  
  else if(distance > threshold){
    stop_rm(sstp);
  }
}

// Basic level function declarations

// Ultrasonic Sensors

// *** check all three sensor readings and return a string
void takeSensorReading() {
  int f = isWallOnFront();
  int r = isWallOnRight();
  int l = isWallOnLeft();
  int reading = (f*100)+(r*10)+l;
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
  int delays = tp * 1000; // turn for duration = tp, then stop
  delay(delays);
  stop_m();
}
// *** turn right 90 deg
void tright_90() {
  digitalWrite(mR_Pin1, HIGH);
  digitalWrite(mR_Pin2, LOW);
  digitalWrite(mL_Pin1, HIGH);
  digitalWrite(mL_Pin2, LOW);  
  int delays = tp * 1000; // turn for duration = tp, then stop
  delay(delays);
  stop_m();
}
// *** turn 180 degrees
void turn_180() {
  digitalWrite(mR_Pin1, HIGH);
  digitalWrite(mR_Pin2, LOW);
  digitalWrite(mL_Pin1, HIGH);
  digitalWrite(mL_Pin2, LOW);  
  int delays = 2 * tp * 1000;
  delay(delays);
  stop_m();
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
