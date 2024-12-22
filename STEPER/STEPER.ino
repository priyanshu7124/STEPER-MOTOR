#include <AccelStepper.h>

// Pin Definitions
#define MOTOR_X_ENABLE_PIN 8
#define MOTOR_X_STEP_PIN 2
#define MOTOR_X_DIR_PIN 5

#define MOTOR_Y_ENABLE_PIN 8
#define MOTOR_Y_STEP_PIN 3
#define MOTOR_Y_DIR_PIN 6

#define MOTOR_Z_ENABLE_PIN 8
#define MOTOR_Z_STEP_PIN 4
#define MOTOR_Z_DIR_PIN 7

#define MOTOR_A_ENABLE_PIN 8
#define MOTOR_A_STEP_PIN 12
#define MOTOR_A_DIR_PIN 13

// Create motor objects
AccelStepper motorX(1, MOTOR_X_STEP_PIN, MOTOR_X_DIR_PIN);
AccelStepper motorY(1, MOTOR_Y_STEP_PIN, MOTOR_Y_DIR_PIN);
AccelStepper motorZ(1, MOTOR_Z_STEP_PIN, MOTOR_Z_DIR_PIN);
AccelStepper motorA(1, MOTOR_A_STEP_PIN, MOTOR_A_DIR_PIN);

// Variables for common speed and acceleration
int commonSpeed = 600;       // Default max speed for all motors
int commonAcceleration = 500; // Default acceleration for all motors

void setup()
{
  // Initialize pins and motors
  pinMode(MOTOR_X_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_Y_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_Z_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_A_ENABLE_PIN, OUTPUT);

  motorX.setEnablePin(MOTOR_X_ENABLE_PIN);
  motorY.setEnablePin(MOTOR_Y_ENABLE_PIN);
  motorZ.setEnablePin(MOTOR_Z_ENABLE_PIN);
  motorA.setEnablePin(MOTOR_A_ENABLE_PIN);

  motorX.setPinsInverted(false, false, true);  // Direction pin inversion for anticlockwise
  motorY.setPinsInverted(false, false, true);
  motorZ.setPinsInverted(false, false, true);
  motorA.setPinsInverted(false, false, true);

  // Set common speed and acceleration for all motors
  motorX.setAcceleration(commonAcceleration);
  motorX.setMaxSpeed(commonSpeed);
  motorY.setAcceleration(commonAcceleration);
  motorY.setMaxSpeed(commonSpeed);
  motorZ.setAcceleration(commonAcceleration);
  motorZ.setMaxSpeed(commonSpeed);
  motorA.setAcceleration(commonAcceleration);
  motorA.setMaxSpeed(commonSpeed);

  motorX.enableOutputs();
  motorY.enableOutputs();
  motorZ.enableOutputs();
  motorA.enableOutputs();

  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial) {} // Wait for Serial Monitor to open (needed for some boards)
}

void loop()
{
  // Check if data is available to read from Serial Monitor
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove any whitespace/newline

    // Handle different commands from the GUI
    if (command.startsWith("S")) {
      commonSpeed = command.substring(1).toInt();
      motorX.setMaxSpeed(commonSpeed);
      motorY.setMaxSpeed(commonSpeed);
      motorZ.setMaxSpeed(commonSpeed);
      motorA.setMaxSpeed(commonSpeed);
      Serial.print("New speed set to: ");
      Serial.println(commonSpeed);
    }
    else if (command.startsWith("A")) {
      commonAcceleration = command.substring(1).toInt();
      motorX.setAcceleration(commonAcceleration);
      motorY.setAcceleration(commonAcceleration);
      motorZ.setAcceleration(commonAcceleration);
      motorA.setAcceleration(commonAcceleration);
      Serial.print("New acceleration set to: ");
      Serial.println(commonAcceleration);
    }

    // Handle motor movements
    else if (command == "X") {
      motorX.move(300); // Move 300 steps clockwise
      motorX.runToPosition();
    }
    else if (command == "x") {
      motorX.move(-300); // Move 300 steps anticlockwise
      motorX.runToPosition();
    }

    else if (command == "Y") {
      motorY.move(300); // Move 300 steps clockwise
      motorY.runToPosition();
    }
    else if (command == "y") {
      motorY.move(-300); // Move 300 steps anticlockwise
      motorY.runToPosition();
    }

    else if (command == "Z") {
      motorZ.move(300); // Move 300 steps clockwise
      motorZ.runToPosition();
    }
    else if (command == "z") {
      motorZ.move(-300); // Move 300 steps anticlockwise
      motorZ.runToPosition();
    }

    else if (command == "A") {
      motorA.move(300); // Move 300 steps clockwise
      motorA.runToPosition();
    }
    else if (command == "a") {
      motorA.move(-300); // Move 300 steps anticlockwise
      motorA.runToPosition();
    }

    // Handle Start and Stop
    else if (command == "START") {
      Serial.println("Motors started.");
    }
    else if (command == "STOP") {
      Serial.println("Motors stopped.");
    }
    else {
      Serial.println("Invalid command");
    }
  }
}
