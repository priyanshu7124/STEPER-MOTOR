#include <AccelStepper.h>

// Define stepper motors on X, Y, Z, and A axes
AccelStepper stepperX(AccelStepper::DRIVER, 2, 5);  // X-axis motor pins
AccelStepper stepperY(AccelStepper::DRIVER, 3, 6);  // Y-axis motor pins
AccelStepper stepperZ(AccelStepper::DRIVER, 4, 7);  // Z-axis motor pins
AccelStepper stepperA(AccelStepper::DRIVER, 8, 9);  // A-axis motor pins

// Variables to hold rotation times for each motor
int rotationTimeX = 0;
int rotationTimeY = 0;
int rotationTimeZ = 0;
int rotationTimeA = 0;

// Flags to track motor states
bool isRunningX = false;
bool isRunningY = false;
bool isRunningZ = false;
bool isRunningA = false;

// Common rotation time for all motors
int commonRotationTime = 0;
bool startAllMotors = false;
bool stopAllMotors = false;

// Timer variables
unsigned long startTimeX = 0;
unsigned long startTimeY = 0;
unsigned long startTimeZ = 0;
unsigned long startTimeA = 0;

const int stepPin = 2;
const int dirPin = 3;

// AccelStepper motorX(1, stepPin, dirPin); // Motor X (driver type: 1 = DRIVER)

void setup() {
    Serial.begin(9600);

    // Set motor speeds and accelerations
    stepperX.setSpeed(500);
    // stepperX.setAcceleration(100);
    stepperX.setMaxSpeed(1000);
    stepperY.setMaxSpeed(1000);
    stepperY.setAcceleration(500);
    stepperZ.setMaxSpeed(1000);
    stepperZ.setAcceleration(500);
    stepperA.setMaxSpeed(1000);
    stepperA.setAcceleration(500);
}

void loop() {
    checkSerialCommands();

    // Check if start or stop all command was received
    if (startAllMotors) {
        startMotor("X", commonRotationTime);
        startMotor("Y", commonRotationTime);
        startMotor("Z", commonRotationTime);
        startMotor("A", commonRotationTime);
        startAllMotors = false;
    }
    if (stopAllMotors) {
        stopMotor("X");
        stopMotor("Y");
        stopMotor("Z");
        stopMotor("A");
        stopAllMotors = false;
    }

    updateMotor("X", stepperX, isRunningX, startTimeX, rotationTimeX);
    updateMotor("Y", stepperY, isRunningY, startTimeY, rotationTimeY);
    updateMotor("Z", stepperZ, isRunningZ, startTimeZ, rotationTimeZ);
    updateMotor("A", stepperA, isRunningA, startTimeA, rotationTimeA);
}

// Function to check and handle serial commands
void checkSerialCommands() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();

        if (command.startsWith("TIME ALL")) {
            commonRotationTime = command.substring(9).toInt();
            Serial.println("Set common time for all motors: " + String(commonRotationTime) + " seconds");
        } 
        else if (command.startsWith("START ALL")) {
            startAllMotors = true;
            Serial.println("Starting all motors");
        } 
        else if (command.startsWith("STOP ALL")) {
            stopAllMotors = true;
            Serial.println("Stopping all motors");
        } 
        else if (command.startsWith("TIME")) {
            String axis = command.substring(5, 6);
            int time = command.substring(7).toInt();
            setRotationTime(axis, time);
        } 
        else if (command.startsWith("START")) {
            String axis = command.substring(6);
            startMotor(axis, getRotationTime(axis));
        } 
        else if (command.startsWith("STOP")) {
            String axis = command.substring(5);
            stopMotor(axis);
        }
    }
}

// Function to set rotation time for a specific motor
void setRotationTime(String axis, int time) {
    if (axis == "X") rotationTimeX = time;
    else if (axis == "Y") rotationTimeY = time;
    else if (axis == "Z") rotationTimeZ = time;
    else if (axis == "A") rotationTimeA = time;

    Serial.println("Set rotation time for Motor " + axis + ": " + String(time) + " seconds");
}

// Function to get rotation time for a specific motor
int getRotationTime(String axis) {
    if (axis == "X") return rotationTimeX;
    else if (axis == "Y") return rotationTimeY;
    else if (axis == "Z") return rotationTimeZ;
    else if (axis == "A") return rotationTimeA;
    return 0;
}

// Function to start a motor
void startMotor(String axis, int time) {
    if (axis == "X") {
        isRunningX = true;
        startTimeX = millis();
        stepperX.setSpeed(500);  // Set speed
    } else if (axis == "Y") {
        isRunningY = true;
        startTimeY = millis();
        stepperY.setSpeed(500);  // Set speed
    } else if (axis == "Z") {
        isRunningZ = true;
        startTimeZ = millis();
        stepperZ.setSpeed(500);  // Set speed
    } else if (axis == "A") {
        isRunningA = true;
        startTimeA = millis();
        stepperA.setSpeed(500);  // Set speed
    }

    Serial.println("Started Motor " + axis + " for " + String(time) + " seconds");
}

// Function to stop a motor
void stopMotor(String axis) {
    if (axis == "X") {
        isRunningX = false;
        stepperX.stop();
    } else if (axis == "Y") {
        isRunningY = false;
        stepperY.stop();
    } else if (axis == "Z") {
        isRunningZ = false;
        stepperZ.stop();
    } else if (axis == "A") {
        isRunningA = false;
        stepperA.stop();
    }

    Serial.println("Stopped Motor " + axis);
}

// Function to update motor status based on elapsed time
void updateMotor(String axis, AccelStepper &stepper, bool &isRunning, unsigned long &startTime, int rotationTime) {
    if (isRunning) {
        stepper.runSpeed();
        int elapsedTime = (millis() - startTime) / 1000;
        Serial.println("Elapsed Time for Motor " + axis + ": " + String(elapsedTime) + " seconds");

        if (elapsedTime >= rotationTime) {
            stopMotor(axis);
            Serial.println("Motor " + axis + " stopped after reaching set time");
        }
    }
}       

