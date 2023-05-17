// #include <AFMotor.h>
// AF_DCMotor M1(1);
// AF_DCMotor M2(2);
// AF_DCMotor M3(3);
// AF_DCMotor M4(4);

// void setup() {
//   M1.setSpeed(255);
//   M3.setSpeed(255);
//   M4.setSpeed(255);
//   M2.setSpeed(150);
//   M3.setSpeed(255);
//   M4.setSpeed(255);
// }

// void loop() {
//   M1.run(FORWARD);
//   M2.run(FORWARD);
//   M3.run(FORWARD);
//   M4.run(FORWARD);
//   // delay(2000);
//   // M1.run(BACKWARD);
//   // M2.run(BACKWARD);
//   // delay(2000);
//   // M1.run(RELEASE);
//   // M2.run(RELEASE);
//   // delay(500);
// }

#include <Servo.h>
#include <IRremote.h>

Servo doorServo;
Servo obstacleServo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int openingThreshold = 55;
int closingThreshold = 90;

enum MotorPins {
  // Motor A connections
  enA = 8,
  in1 = 7,
  in2 = 6,
  // Motor B connections
  enB = 5,
  in3 = 4,
  in4 = 3
};

enum UtralsonicSensors {
  levelTrigPin1 = 48,
  levelEchoPin1 = 49,

  levelTrigPin2 = 50,
  levelEchoPin2 = 51,

  doorTrigPin = 52,
  doorEchoPin = 53
};


const int infaredPin = 11;
const int obstacleServoPin = 10;
const int doorServoPin = 9;

IRrecv irrecv(infaredPin);
decode_results remote_data;

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

  // Set all the motor control pins to outputs
  pinMode(MotorPins::enA, OUTPUT);
  pinMode(MotorPins::enB, OUTPUT);
  pinMode(MotorPins::in1, OUTPUT);
  pinMode(MotorPins::in2, OUTPUT);
  pinMode(MotorPins::in3, OUTPUT);
  pinMode(MotorPins::in4, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(MotorPins::in1, LOW);
  digitalWrite(MotorPins::in2, LOW);
  digitalWrite(MotorPins::in3, LOW);
  digitalWrite(MotorPins::in4, LOW);

  obstacleServo.attach(obstacleServoPin);  // attaches the servo on the pin to the servo object
  doorServo.attach(doorServoPin);

  irrecv.enableIRIn();
  irrecv.blink13(true);


  Serial.begin(9600);  // Starts the serial communication
}
int driveAngle = 93;

void loop() {
  if (irrecv.decode(&remote_data)) {
    Serial.println(remote_data.value, HEX);
    irrecv.resume();
  }
  return;

  // int distance = get_distance(UtralsonicSensors::levelTrigPin1, UtralsonicSensors::levelEchoPin1);
  // Serial.print(F("Testing ultral: "));
  // Serial.println(distance);
  // delay(1000);

  // obstacleServo.write(120);
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



// This function lets you control spinning direction of motors
void directionControl() {
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(MotorPins::enA, 255);
  analogWrite(MotorPins::enB, 255);

  // Turn on motor A & B
  digitalWrite(MotorPins::in1, HIGH);
  digitalWrite(MotorPins::in2, LOW);
  digitalWrite(MotorPins::in3, HIGH);
  digitalWrite(MotorPins::in4, LOW);
  delay(2000);

  // // Now change motor directions
  digitalWrite(MotorPins::in1, LOW);
  digitalWrite(MotorPins::in2, HIGH);
  digitalWrite(MotorPins::in3, LOW);
  digitalWrite(MotorPins::in4, HIGH);
  delay(2000);

  // // Turn off motors
  digitalWrite(MotorPins::in1, LOW);
  digitalWrite(MotorPins::in2, LOW);
  digitalWrite(MotorPins::in3, LOW);
  digitalWrite(MotorPins::in4, LOW);
  delay(1000);
}

// This function lets you control speed of the motors
void speedControl() {
  // Turn on motors
  digitalWrite(MotorPins::in1, LOW);
  digitalWrite(MotorPins::in2, HIGH);
  digitalWrite(MotorPins::in3, LOW);
  digitalWrite(MotorPins::in4, HIGH);

  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++) {
    analogWrite(MotorPins::enA, i);
    analogWrite(MotorPins::enB, i);
    delay(20);
  }

  // Decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i) {
    analogWrite(MotorPins::enA, i);
    analogWrite(MotorPins::enB, i);
    delay(20);
  }

  // Now turn off motors
  digitalWrite(MotorPins::in1, LOW);
  digitalWrite(MotorPins::in2, LOW);
  digitalWrite(MotorPins::in3, LOW);
  digitalWrite(MotorPins::in4, LOW);
}



// Sensor pins pin D6 LED output, pin A0 analog Input

// #define ledPin 6
// #define sensorPin A0

// void setup() {
//   Serial.begin(9600);
//   pinMode(ledPin, OUTPUT);
//   digitalWrite(ledPin, LOW);
// }


// void loop() {
//   Serial.print("Analog output: ");
//   Serial.println(readSensor());
//   delay(500);
// }

// //  This function returns the analog data to calling function

// int readSensor() {

//   unsigned int sensorValue = analogRead(sensorPin);  // Read the analog value from sensor
//   unsigned int outputValue = map(sensorValue, 0, 1023, 0, 255);  // map the 10-bit data to 8-bit data

//   if (outputValue > 65)
//     analogWrite(ledPin, outputValue);  // generate PWM signal
//   else
//     digitalWrite(ledPin, LOW);

//   return outputValue;  // Return analog moisture value
// }
