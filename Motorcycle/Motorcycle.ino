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

// Motor settings
bool motorRunning[4] = {false, false, false, false};
bool motorDirection[4] = {true, true, true, true}; // True = forward, False = backward
int motorSpeeds[4] = {500, 500, 500, 500};         // Speed in steps per second
unsigned long rotationTimes[4] = {10000, 10000, 10000, 10000}; // Rotation durations in milliseconds
unsigned long repetitionTimes[4] = {5000, 5000, 5000, 5000};   // Repetition intervals in milliseconds
unsigned long startTimes[4] = {0, 0, 0, 0};        // Start times for each motor
unsigned long nextRunTimes[4] = {0, 0, 0, 0};      // Next scheduled run times for each motor
int numCycles[4] = {0, 0, 0, 0};                   // Number of cycles for each motor
int currentCycles[4] = {0, 0, 0, 0};               // Current cycle count for each motor

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

  for (int i = 0; i < 4; i++) {
    // Check if it's time to start the motor again
    if (!motorRunning[i] && currentTime >= nextRunTimes[i] && nextRunTimes[i] > 0) {
      if (numCycles[i] == 0 || currentCycles[i] < numCycles[i]) { // Check cycle limit
        motorRunning[i] = true;
        startTimes[i] = currentTime;
        currentCycles[i]++;
        Serial.print("Motor ");
        Serial.print((char)('X' + i));
        Serial.println(" started.");
      }
    }

    // Control motor running logic
    if (motorRunning[i]) {
      unsigned long elapsedTime = currentTime - startTimes[i];

      if (elapsedTime >= rotationTimes[i]) {
        motorRunning[i] = false;
        if (numCycles[i] == 0 || currentCycles[i] < numCycles[i]) {
          nextRunTimes[i] = currentTime + repetitionTimes[i];
        } else {
          nextRunTimes[i] = 0; // Stop scheduling further runs
          Serial.print("Motor ");
          Serial.print((char)('X' + i));
          Serial.println(" completed all cycles.");
        }
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

// Process commands
void processCommand(String command) {
  if (command.startsWith("START")) {
    handleStartCommand(command);
  } else if (command.startsWith("STOP")) {
    handleStopCommand(command);
  } else if (command.startsWith("TIME")) {
    handleTimeCommand(command);
  } else if (command.startsWith("REPEAT")) {
    handleRepeatCommand(command);
  } else if (command.startsWith("DIRECTION")) {
    handleDirectionCommand(command);
  } else if (command.startsWith("SPEED")) {
    handleSpeedCommand(command);
  } else if (command.startsWith("CYCLE")) {
    handleCycleCommand(command);
  } else {
    Serial.println("Unknown command");
  }
}

// Handle START command
void handleStartCommand(String command) {
  int motorIndex = getMotorIndex(command.charAt(6));
  if (motorIndex >= 0) {
    motorRunning[motorIndex] = true;
    startTimes[motorIndex] = millis();
    nextRunTimes[motorIndex] = 0; // Cancel scheduled runs
    currentCycles[motorIndex] = 0; // Reset current cycles
    Serial.print("Motor ");
    Serial.print((char)('X' + motorIndex));
    Serial.println(" started.");
  }
}

// Handle STOP command
void handleStopCommand(String command) {
  int motorIndex = getMotorIndex(command.charAt(5));
  if (motorIndex >= 0) {
    motorRunning[motorIndex] = false;
    nextRunTimes[motorIndex] = 0; // Cancel scheduled runs
    Serial.print("Motor ");
    Serial.print((char)('X' + motorIndex));
    Serial.println(" stopped.");
  }
}

// Handle TIME command
void handleTimeCommand(String command) {
  int motorIndex = getMotorIndex(command.charAt(5));
  int timeValue = command.substring(7).toInt();
  if (motorIndex >= 0 && timeValue > 0) {
    rotationTimes[motorIndex] = timeValue * 1000; // Convert seconds to milliseconds
    Serial.print("Rotation time for Motor ");
    Serial.print((char)('X' + motorIndex));
    Serial.print(" set to ");
    Serial.print(timeValue);
    Serial.println(" seconds.");
  }
}

// Handle REPEAT command
void handleRepeatCommand(String command) {
  int motorIndex = getMotorIndex(command.charAt(7));
  int repeatValue = command.substring(9).toInt();
  if (motorIndex >= 0 && repeatValue > 0) {
    repetitionTimes[motorIndex] = repeatValue * 1000; // Convert seconds to milliseconds
    Serial.print("Repetition time for Motor ");
    Serial.print((char)('X' + motorIndex));
    Serial.print(" set to ");
    Serial.print(repeatValue);
    Serial.println(" seconds.");
  }
}

// Handle DIRECTION command
void handleDirectionCommand(String command) {
  int motorIndex = getMotorIndex(command.charAt(10));
  String direction = command.substring(12);
  if (motorIndex >= 0) {
    motorDirection[motorIndex] = (direction == "FORWARD");
    Serial.print("Direction for Motor ");
    Serial.print((char)('X' + motorIndex));
    Serial.print(" set to ");
    Serial.println(direction);
  }
}

// Handle SPEED command
void handleSpeedCommand(String command) {
  int motorIndex = getMotorIndex(command.charAt(6));
  int speedValue = command.substring(8).toInt();
  if (motorIndex >= 0 && speedValue > 0 && speedValue <= 1000) {
    motorSpeeds[motorIndex] = speedValue;
    steppers[motorIndex]->setSpeed(speedValue);
    Serial.print("Speed for Motor ");
    Serial.print((char)('X' + motorIndex));
    Serial.print(" set to ");
    Serial.print(speedValue);
    Serial.println(" units.");
  }
}

// Handle CYCLE command
void handleCycleCommand(String command) {
  int motorIndex = getMotorIndex(command.charAt(6));
  int cycleValue = command.substring(8).toInt();
  if (motorIndex >= 0 && cycleValue >= 0) {
    numCycles[motorIndex] = cycleValue;
    currentCycles[motorIndex] = 0; // Reset the current cycle count
    Serial.print("Number of cycles for Motor ");
    Serial.print((char)('X' + motorIndex));
    Serial.print(" set to ");
    Serial.println(cycleValue);
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
