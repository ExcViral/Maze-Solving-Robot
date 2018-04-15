/*
* This code is to find the turning parameter 'd'.
* In our robot, we have one two motors controlling
* the movements of the wheels. For turing, we operate motors in
* opposite directions, so we can turn on the axis of the motor.
*
* As of current setting of the robot, we donot have stepper motor,
* or any kind of rotation measuring device. So, we cannot make
* accurate turns. We are making turns my turning on and off the
* motors for certain time period (d) to achieve the desired turn
* of angle 90 degrees.
* This code will help to find the turning parameter 'd' using trial
* and error.
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
  Serial.println("Serial is ready!");
}

void loop() {
  float d;
  delay(100);
  if(Serial.available() > 0) {
    d = Serial.parseFloat();
    Serial.println(d);
    if(d > 0) {
      tleft_90(d);
      delay(1000);
      stop_m();
      delay(1000);
      tright_90(d);
      delay(1000);
      stop_m();
    }
  }
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

void tleft_90(int d) {
  digitalWrite(mR_Pin1, LOW);
  digitalWrite(mR_Pin2, HIGH);
  digitalWrite(mL_Pin1, LOW);
  digitalWrite(mL_Pin2, HIGH);  
  int delays = d * 1000;
  delay(delays);
  stop_m();
}

void tright_90(int d) {
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
