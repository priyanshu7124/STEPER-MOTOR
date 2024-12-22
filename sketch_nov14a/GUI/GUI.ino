#include <AccelStepper.h>
#include <TimeLib.h>

// Define pins for each motor on CNC Shield
const int stepPins[] = {2, 3, 4, 9};      // X, Y, Z, A step pins
const int dirPins[] = {5, 6, 7, 8};       // X, Y, Z, A direction pins

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

  // Initialize motors
  for (int i = 0; i < 4; i++) {
    steppers[i]->setMaxSpeed(1000);          // Set maximum speed
    steppers[i]->setSpeed(motorSpeeds[i]);   // Set initial speed
  }

  Serial.println("Arduino Motor Controller Ready");
}

void loop() {
  // Process incoming serial commands
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove any trailing whitespace
    processCommand(command);
  }

  // Update motor states
  for (int i = 0; i < 4; i++) {
    unsigned long currentTime = millis();

    // Check if it's time to start the next cycle
    if (!motorRunning[i] && nextRunTime[i] > 0 && currentTime >= nextRunTime[i]) {
      motorRunning[i] = true;
      startTimes[i] = currentTime; // Record start time
      Serial.print("Motor ");
      Serial.print((char)('X' + i));
      Serial.println(" started.");
    }

    // Handle running motors
    if (motorRunning[i]) {
      unsigned long elapsedTime = currentTime - startTimes[i];

      if (elapsedTime >= rotationTimes[i]) {
        motorRunning[i] = false; // Stop motor after set time
        nextRunTime[i] = currentTime + repeatTimes[i]; // Schedule the next run
        Serial.print("Motor ");
        Serial.print((char)('X' + i));
        Serial.println(" stopped.");
      } else {
        // Keep running the motor
        steppers[i]->setSpeed(motorDirection[i] ? motorSpeeds[i] : -motorSpeeds[i]);
        steppers[i]->runSpeed();
      }
    }
  }
}

// Process a single command from the GUI
void processCommand(String command) {
  if (command.startsWith("START ALL")) {
    for (int i = 0; i < 4; i++) {
      motorRunning[i] = true;
      startTimes[i] = millis();
      nextRunTime[i] = 0;
      Serial.print("Motor ");
      Serial.print((char)('X' + i));
      Serial.println(" started.");
    }
  } else if (command.startsWith("STOP ALL")) {
    for (int i = 0; i < 4; i++) {
      motorRunning[i] = false;
      nextRunTime[i] = 0;
      Serial.print("Motor ");
      Serial.print((char)('X' + i));
      Serial.println(" stopped.");
    }
  } else if (command.startsWith("START")) {
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
  } else {
    Serial.println("Unknown command");
  }
}

// Handle START command
void handleStartCommand(String command) {
  int motorIndex = getMotorIndex(command.charAt(6));
  if (motorIndex >= 0) {
    motorRunning[motorIndex] = true;
    startTimes[motorIndex] = millis(); // Record start time
    nextRunTime[motorIndex] = 0; // Reset next run time
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
    nextRunTime[motorIndex] = 0; // Clear next run time
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
    repeatTimes[motorIndex] = repeatValue * 1000; // Convert seconds to milliseconds
    Serial.print("Repeat interval for Motor ");
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
    if (direction == "FORWARD") {
      motorDirection[motorIndex] = true;
      Serial.print("Direction for Motor ");
      Serial.print((char)('X' + motorIndex));
      Serial.println(" set to FORWARD.");
    } else if (direction == "BACKWARD") {
      motorDirection[motorIndex] = false;
      Serial.print("Direction for Motor ");
      Serial.print((char)('X' + motorIndex));
      Serial.println(" set to BACKWARD.");
    }
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

// Helper: Get motor index from axis (X, Y, Z, A)
int getMotorIndex(char axis) {
  if (axis == 'X') return 0;
  if (axis == 'Y') return 1;
  if (axis == 'Z') return 2;
  if (axis == 'A') return 3;
  return -1;
}
