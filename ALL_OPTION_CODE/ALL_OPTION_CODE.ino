#include <AccelStepper.h>

// Define pins for each motor
const int stepPins[] = {2, 3, 4, 12};    // X, Y, Z, A step pins
const int dirPins[] = {5, 6, 7, 13};     // X, Y, Z, A direction pins
const int enablePin = 8;                 // Shared enable pin for all motors

// Initialize AccelStepper objects
AccelStepper stepperX(AccelStepper::DRIVER, stepPins[0], dirPins[0]);
AccelStepper stepperY(AccelStepper::DRIVER, stepPins[1], dirPins[1]);
AccelStepper stepperZ(AccelStepper::DRIVER, stepPins[2], dirPins[2]);
AccelStepper stepperA(AccelStepper::DRIVER, stepPins[3], dirPins[3]);

AccelStepper* steppers[] = {&stepperX, &stepperY, &stepperZ, &stepperA};

// Motor settings for individual control
bool motorRunning[4] = {false, false, false, false};
bool motorDirection[4] = {true, true, true, true}; // True = forward, False = backward
int motorSpeeds[4] = {500, 500, 500, 500};         // Speed in steps per second
unsigned long rotationTimes[4] = {10000, 10000, 10000, 10000}; // Rotation durations in milliseconds
unsigned long repetitionTimes[4] = {5000, 5000, 5000, 5000};   // Repetition intervals in milliseconds
unsigned long startTimes[4] = {0, 0, 0, 0};        // Start times for each motor
unsigned long nextRunTimes[4] = {0, 0, 0, 0};      // Next scheduled run times for each motor

// Universal mode settings
bool allMotorsMode = false; // Flag for all motors mode
bool allMotorDirection = true;
int allMotorSpeed = 500;
unsigned long allRotationTime = 10000;
unsigned long allRepetitionTime = 5000;
unsigned long allStartTime = 0;
unsigned long allNextRunTime = 0;

void setup() {
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); // Enable the motor drivers

  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    steppers[i]->setMaxSpeed(1000);
    steppers[i]->setSpeed(motorSpeeds[i]);
  }

  Serial.println("Arduino 4-Axis Motor Controller Ready");
}

void loop() {
  // Process incoming serial commands
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    processCommand(command);
  }

  unsigned long currentTime = millis();

  if (allMotorsMode) {
    handleAllMotors(currentTime);
  } else {
    handleIndividualMotors(currentTime);
  }
}

void handleIndividualMotors(unsigned long currentTime) {
  for (int i = 0; i < 4; i++) {
    // Check if it's time to start the motor again
    if (!motorRunning[i] && currentTime >= nextRunTimes[i] && nextRunTimes[i] > 0) {
      motorRunning[i] = true;
      startTimes[i] = currentTime;
      Serial.print("Motor ");
      Serial.print((char)('X' + i));
      Serial.println(" started.");
    }

    // Control motor running logic
    if (motorRunning[i]) {
      unsigned long elapsedTime = currentTime - startTimes[i];

      if (elapsedTime >= rotationTimes[i]) {
        motorRunning[i] = false;
        nextRunTimes[i] = currentTime + repetitionTimes[i];
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

void handleAllMotors(unsigned long currentTime) {
  if (!motorRunning[0] && currentTime >= allNextRunTime) {
    for (int i = 0; i < 4; i++) {
      motorRunning[i] = true;
    }
    allStartTime = currentTime;
    Serial.println("All motors started.");
  }

  if (motorRunning[0]) {
    unsigned long elapsedTime = currentTime - allStartTime;

    if (elapsedTime >= allRotationTime) {
      for (int i = 0; i < 4; i++) {
        motorRunning[i] = false;
      }
      allNextRunTime = currentTime + allRepetitionTime;
      Serial.println("All motors stopped.");
    } else {
      for (int i = 0; i < 4; i++) {
        steppers[i]->setSpeed(allMotorDirection ? allMotorSpeed : -allMotorSpeed);
        steppers[i]->runSpeed();
      }
    }
  }
}

// Process commands
void processCommand(String command) {
  if (command == "START ALL") {
    allMotorsMode = true;
    for (int i = 0; i < 4; i++) {
      motorRunning[i] = false;
    }
    allNextRunTime = 0;
    Serial.println("All motors mode enabled.");
  } else if (command == "STOP ALL") {
    allMotorsMode = false;
    for (int i = 0; i < 4; i++) {
      motorRunning[i] = false;
    }
    Serial.println("All motors mode disabled.");
  } else if (command.startsWith("TIME ALL")) {
    allRotationTime = command.substring(9).toInt() * 1000;
    Serial.print("All motors rotation time set to ");
    Serial.print(allRotationTime / 1000);
    Serial.println(" seconds.");
  } else if (command.startsWith("REPEAT ALL")) {
    allRepetitionTime = command.substring(11).toInt() * 1000;
    Serial.print("All motors repetition time set to ");
    Serial.print(allRepetitionTime / 1000);
    Serial.println(" seconds.");
  } else if (command.startsWith("DIRECTION ALL")) {
    String direction = command.substring(14);
    allMotorDirection = (direction == "FORWARD");
    Serial.print("All motors direction set to ");
    Serial.println(allMotorDirection ? "FORWARD" : "BACKWARD");
  } else if (command.startsWith("SPEED ALL")) {
    allMotorSpeed = command.substring(10).toInt();
    if (allMotorSpeed > 0 && allMotorSpeed <= 1000) {
      Serial.print("All motors speed set to ");
      Serial.print(allMotorSpeed);
      Serial.println(" units.");
    }
  } else {
    // Process individual motor commands
    processIndividualCommand(command);
  }
}

void processIndividualCommand(String command) {
  if (command.startsWith("START")) {
    int motorIndex = getMotorIndex(command.charAt(6));
    if (motorIndex >= 0) {
      motorRunning[motorIndex] = true;
      startTimes[motorIndex] = millis();
      nextRunTimes[motorIndex] = 0; // Cancel scheduled runs
      Serial.print("Motor ");
      Serial.print((char)('X' + motorIndex));
      Serial.println(" started.");
    }
  } else if (command.startsWith("STOP")) {
    int motorIndex = getMotorIndex(command.charAt(5));
    if (motorIndex >= 0) {
      motorRunning[motorIndex] = false;
      nextRunTimes[motorIndex] = 0; // Cancel scheduled runs
      Serial.print("Motor ");
      Serial.print((char)('X' + motorIndex));
      Serial.println(" stopped.");
    }
  } else if (command.startsWith("TIME")) {
    int motorIndex = getMotorIndex(command.charAt(5));
    int timeValue = command.substring(7).toInt();
    if (motorIndex >= 0 && timeValue > 0) {
      rotationTimes[motorIndex] = timeValue * 1000; // Convert seconds to ms
      Serial.print("Rotation time for Motor ");
      Serial.print((char)('X' + motorIndex));
      Serial.print(" set to ");
      Serial.print(timeValue);
      Serial.println(" seconds.");
    }
  } else if (command.startsWith("REPEAT")) {
    int motorIndex = getMotorIndex(command.charAt(7));
    int repeatValue = command.substring(9).toInt();
    if (motorIndex >= 0 && repeatValue > 0) {
      repetitionTimes[motorIndex] = repeatValue * 1000; // Convert seconds to ms
      Serial.print("Repetition time for Motor ");
      Serial.print((char)('X' + motorIndex));
      Serial.print(" set to ");
      Serial.print(repeatValue);
      Serial.println(" seconds.");
    }
  } else if (command.startsWith("DIRECTION")) {
    int motorIndex = getMotorIndex(command.charAt(10));
    String direction = command.substring(12);
    if (motorIndex >= 0) {
      motorDirection[motorIndex] = (direction == "FORWARD");
      Serial.print("Direction for Motor ");
      Serial.print((char)('X' + motorIndex));
      Serial.print(" set to ");
      Serial.println(motorDirection[motorIndex] ? "FORWARD" : "BACKWARD");
    }
  } else if (command.startsWith("SPEED")) {
    int motorIndex = getMotorIndex(command.charAt(6));
    int speedValue = command.substring(8).toInt();
    if (motorIndex >= 0 && speedValue > 0 && speedValue <= 1000) {
      motorSpeeds[motorIndex] = speedValue;
      steppers[motorIndex]->setSpeed(motorDirection[motorIndex] ? motorSpeeds[motorIndex] : -motorSpeeds[motorIndex]);
      Serial.print("Speed for Motor ");
      Serial.print((char)('X' + motorIndex));
      Serial.print(" set to ");
      Serial.print(speedValue);
      Serial.println(" units.");
    }
  }
}

// Helper: Get motor index from axis (X, Y, Z, A)
int getMotorIndex(char axis) {
  if (axis == 'X') return 0;
  if (axis == 'Y') return 1;
  if (axis == 'Z') return 2;
  if (axis == 'A') return 3;
  return -1;
}
