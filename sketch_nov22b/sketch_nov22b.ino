#include <AccelStepper.h>

// Define pins for each motor on CNC Shield
const int stepPins[] = {2, 3, 4, 12};      // X, Y, Z, A step pins
const int dirPins[] = {5, 6, 7, 13};       // X, Y, Z, A direction pins
const int enablePin = 8;                   // Enable pin for all motors

// Initialize AccelStepper objects for each motor
AccelStepper stepperX(AccelStepper::DRIVER, stepPins[0], dirPins[0]);
AccelStepper stepperY(AccelStepper::DRIVER, stepPins[1], dirPins[1]);
AccelStepper stepperZ(AccelStepper::DRIVER, stepPins[2], dirPins[2]);
AccelStepper stepperA(AccelStepper::DRIVER, stepPins[3], dirPins[3]);

AccelStepper* steppers[] = {&stepperX, &stepperY, &stepperZ, &stepperA};

// Motor settings and state variables
bool motorRunning[4] = {false, false, false, false};
bool motorDirection[4] = {true, true, true, true}; // True = forward, False = backward
int motorSpeeds[4] = {500, 500, 500, 500};         // Speed for each motor
unsigned long startTimes[4] = {0, 0, 0, 0};        // Start time for each motor
unsigned long repeatTimes[4] = {0, 0, 0, 0};       // Repeat interval in milliseconds
unsigned long rotationTimes[4] = {10000, 10000, 10000, 10000}; // Rotation duration in milliseconds
unsigned long nextRunTime[4] = {0, 0, 0, 0};       // Next scheduled run time for each motor

void setup() {
  Serial.begin(9600);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); // Enable all motors

  // Initialize motors
  for (int i = 0; i < 4; i++) {
    steppers[i]->setMaxSpeed(1000);          // Set maximum speed
    steppers[i]->setSpeed(motorSpeeds[i]);   // Set initial speed
  }

  Serial.println("Arduino Motor Controller Ready");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove any trailing whitespace
    processCommand(command);
  }

  for (int i = 0; i < 4; i++) {
    unsigned long currentTime = millis();

    if (!motorRunning[i] && nextRunTime[i] > 0 && currentTime >= nextRunTime[i]) {
      motorRunning[i] = true;
      startTimes[i] = currentTime;
      Serial.print("Motor ");
      Serial.print((char)('X' + i));
      Serial.println(" started.");
    }

    if (motorRunning[i]) {
      unsigned long elapsedTime = currentTime - startTimes[i];
      if (elapsedTime >= rotationTimes[i]) {
        motorRunning[i] = false;
        nextRunTime[i] = currentTime + repeatTimes[i];
        Serial.print("Motor ");
        Serial.print((char)('X' + i));
        Serial.println(" stopped.");
      } else {
        steppers[i]->setSpeed(motorDirection[i] ? motorSpeeds[i] : -motorSpeeds[i]);
        steppers[i]->runSpeed();
      }
    }
  }
}

void processCommand(String command) {
  if (command == "START ALL") {
    for (int i = 0; i < 4; i++) {
      motorRunning[i] = true;
      startTimes[i] = millis();
      nextRunTime[i] = 0;
    }
    Serial.println("All motors started.");
  } else if (command == "STOP ALL") {
    for (int i = 0; i < 4; i++) {
      motorRunning[i] = false;
      nextRunTime[i] = 0;
    }
    Serial.println("All motors stopped.");
  } else if (command.startsWith("SPEED ALL")) {
    int speedValue = command.substring(10).toInt();
    if (speedValue > 0 && speedValue <= 1000) {
      for (int i = 0; i < 4; i++) {
        motorSpeeds[i] = speedValue;
        steppers[i]->setSpeed(speedValue);
      }
      Serial.print("All motors speed set to ");
      Serial.println(speedValue);
    }
  } else if (command.startsWith("TIME ALL")) {
    int timeValue = command.substring(9).toInt();
    if (timeValue > 0) {
      for (int i = 0; i < 4; i++) {
        rotationTimes[i] = timeValue * 1000;
      }
      Serial.print("All motors rotation time set to ");
      Serial.print(timeValue);
      Serial.println(" seconds.");
    }
  } else if (command.startsWith("REPEAT ALL")) {
    int repeatValue = command.substring(11).toInt();
    if (repeatValue > 0) {
      for (int i = 0; i < 4; i++) {
        repeatTimes[i] = repeatValue * 1000;
      }
      Serial.print("All motors repeat interval set to ");
      Serial.print(repeatValue);
      Serial.println(" seconds.");
    }
  } else if (command == "DIRECTION ALL FORWARD") {
    for (int i = 0; i < 4; i++) motorDirection[i] = true;
    Serial.println("All motors set to FORWARD.");
  } else if (command == "DIRECTION ALL BACKWARD") {
    for (int i = 0; i < 4; i++) motorDirection[i] = false;
    Serial.println("All motors set to BACKWARD.");
  }
}
