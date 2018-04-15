/*
* This code is to test if the motors on the robot 
* are working as expected.
* In our robot, we have one two motors controlling
* the movements of the wheels. The motors are connected
* to the motor driver board, which takes instructions from
* arduino, and controls the motors.
* The job of motors is to help the robot make movements 
* inside the maze.
* The movements and actions of the robot will depend upon the
* readings given by the IR sensor.
*
* As of current setting of the robot, the left motor takes
* LOW-HIGH combination to make forward movement. and right
* motor takes HIGH-LOW combination to make forward movement.
* Calibrate these settings accordingly for your bot. Calibration
* can be done only through trial and error.
*
* Created by Viral Patel
* April 15, 2018
*/

int mR_Pin1 = 7;
int mR_Pin2 = 8;

int mL_Pin1 = 12;
int mL_Pin2 = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(mR_Pin1, OUTPUT);
  pinMode(mR_Pin2, OUTPUT);
  pinMode(mL_Pin1, OUTPUT);
  pinMode(mL_Pin2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  char m;
  if(Serial.available() > 0) {
    m = Serial.read();
    if(m == 'f') {
      mforward();
    }
    else if(m == 'l') {
      tleft_90(1);
    }
    else if(m == 'r') {
      tright_90(1);
    }
  }
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
