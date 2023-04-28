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

int openingThreshold = 55;
int closingThreshold = 90;


enum UtralsonicSensors {
  levelTrigPin1 = 48,
  levelEchoPin1 = 49,

  levelTrigPin2 = 50,
  levelEchoPin2 = 51,

  doorTrigPin = 52,
  doorEchoPin = 53
};



const int obstacleServoPin = 10;
const int doorServoPin = 9;

// defines variables
long duration;
int distance;


int pos = 0;  // variable to store the servo position
bool is_open = false;

void setup() {
  pinMode(UtralsonicSensors::levelTrigPin1, OUTPUT);
  pinMode(UtralsonicSensors::levelEchoPin1, INPUT);
  pinMode(UtralsonicSensors::levelTrigPin2, OUTPUT);
  pinMode(UtralsonicSensors::levelEchoPin2, INPUT);
  pinMode(UtralsonicSensors::doorTrigPin, OUTPUT);
  pinMode(UtralsonicSensors::doorEchoPin, INPUT);
  Serial.begin(9600);                      // Starts the serial communication
  obstacleServo.attach(obstacleServoPin);  // attaches the servo on pin 9 to the servo object
  doorServo.attach(doorServoPin);
}
int driveAngle = 93;

void loop() {

  // doorServo.write(90);
  // return;
  int levelDistance1 = get_distance(UtralsonicSensors::levelTrigPin1, UtralsonicSensors::levelEchoPin1);
  int levelDistance2 = get_distance(UtralsonicSensors::levelTrigPin2, UtralsonicSensors::levelEchoPin2);

  int doorDistance = get_distance(UtralsonicSensors::doorTrigPin, UtralsonicSensors::doorEchoPin);
  int doorPosition = doorServo.read();
  Serial.print(F("Door Position: "));
  Serial.println(doorPosition);

  Serial.print(F("Level Position: "));
  Serial.println(levelDistance1);
  // Serial.print(F("Level Position 2: "));
  // Serial.println(levelDistance2);


  if (doorPosition == driveAngle && doorDistance < openingThreshold) {
    Serial.println(F("Welcome I'm opening the door for you"));
    openDoor();
  } else if (doorPosition == 0) {
    delay(500);  // waits 15 ms for the servo to reach the position
    // test the function with an array of numbers
    int levelDataSet[50];
    int count = sizeof(levelDataSet) / sizeof(int);

    for (int i = 0; i < count; i++) {
      int levelDistance = get_distance(UtralsonicSensors::levelTrigPin1, UtralsonicSensors::levelEchoPin1);
      levelDataSet[i] = levelDistance;
    }
    int mode = calculateMode(levelDataSet, count);
    Serial.print(F("Calculated Mode: "));
    Serial.println(mode);
    if (mode > closingThreshold) {
      Serial.println(F("Closing Bin of the door"));
      closeDoor();
    }
  }


  delay(250);  // waits 15 ms for the servo to reach the position
}



void closeDoor() {
  for (pos = 0; pos <= driveAngle; pos += 1) {  // goes from 0 degrees to 180 degrees
                                                // in steps of 1 degree
    // if (pos == 0 || pos == 90 || pos == 180) {
    //   get_distance(DoorUtralSensor::trigPin, DoorUtralSensor::echoPin);
    // }
    doorServo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(25);             // waits 15 ms for the servo to reach the position
  }
}

void openDoor() {
  for (pos = driveAngle; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    // if (pos == 0 || pos == 90 || pos == 180) {
    //   get_distance(DoorUtralSensor::trigPin, DoorUtralSensor::echoPin);
    // }
    doorServo.write(pos);
    delay(25);  // waits 15 ms for the servo to reach the position
  }
}


int get_distance(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);  // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = duration * 0.034 / 2;

  // Serial.print(F("Distance: "));
  // Serial.println(distance);
  return distance;
}

int getAvergaeLevel(int level1, int level2) {
  if (level1 == 0) {
    return 0;
  }
}

int calculateMode(int numbers[], int count) {
  int mode = 0;
  int maxCount = 0;

  // iterate over each number in the array
  for (int i = 0; i < count; i++) {
    int currentNumber = numbers[i];
    int currentCount = 0;

    // count the number of times the current number appears in the array
    for (int j = 0; j < count; j++) {
      if (numbers[j] == currentNumber) {
        currentCount++;
      }
    }

    // if the current number appears more times than the current mode, update the mode
    if (currentCount > maxCount) {
      mode = currentNumber;
      maxCount = currentCount;
    }
  }

  return mode;
}
