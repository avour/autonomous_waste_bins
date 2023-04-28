// #include <AFMotor.h>
// AF_DCMotor M1(1);
// AF_DCMotor M2(2);

// void setup() {
//   M1.setSpeed(255);
//   M2.setSpeed(150);
// }

// void loop() {
//   M1.run(FORWARD);
//   M2.run(FORWARD);
//   // delay(2000);
//   // M1.run(BACKWARD);
//   // M2.run(BACKWARD);
//   // delay(2000);
//   // M1.run(RELEASE);
//   // M2.run(RELEASE);
//   // delay(500);
// }

/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>

Servo doorServo;  
Servo obstacleServo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

const int trigPin = 52;
const int echoPin = 53;

const int obstacleServoPin = 10;
const int doorServoPin = 9;

// defines variables
long duration;
int distance;

int pos = 0;  // variable to store the servo position
bool is_open = false;

void setup() {
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input
  Serial.begin(9600);        // Starts the serial communication
  obstacleServo.attach(obstacleServoPin);  // attaches the servo on pin 9 to the servo object
  doorServo.attach(doorServoPin);
}
int driveAngle = 90;

void loop() {
  // if (is_open == true) {
  //   return;
  // }
  doorServo.write(0);
  return;

  for (pos = driveAngle; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    // get_distance(pos);
    doorServo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(25);           // waits 15 ms for the servo to reach the position
  }

  for (pos = 0; pos <= driveAngle; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    // get_distance(pos);
    doorServo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(25);           // waits 15 ms for the servo to reach the position
  } 
  is_open = true;

    delay(250);           // waits 15 ms for the servo to reach the position
}



int get_distance(int pos) {
  //
  if (pos == 0 || pos == 90 || pos == 180) {

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);  // Reads the echoPin, returns the sound wave travel time in microseconds
    distance = duration * 0.034 / 2;

    Serial.print(F("Angle Position: "));
    Serial.println(pos);
    Serial.print(F("Distance: "));
    Serial.println(distance);
    return distance;
  }
}