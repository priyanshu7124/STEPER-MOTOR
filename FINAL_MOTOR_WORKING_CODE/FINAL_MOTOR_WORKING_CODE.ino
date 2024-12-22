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
unsigned long rotationTimes[4] = {0, 0, 0, 0};    // Rotation durations in milliseconds
unsigned long repetitionTimes[4] = {0, 0, 0, 0};  // Repetition intervals in milliseconds
unsigned long startTimes[4] = {0, 0, 0, 0};       // Start times for each motor
unsigned long nextRunTimes[4] = {0, 0, 0, 0};     // Next scheduled run times for each motor
int numCycles[4] = {0, 0, 0, 0};                  // Number of cycles for each motor
int currentCycles[4] = {0, 0, 0, 0};              // Current cycle count for each motor

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
                Serial.print("Motor ");
                Serial.print((char)('X' + i));
                Serial.println(" stopped.");

                if (numCycles[i] == 0 || currentCycles[i] < numCycles[i]) {
                    nextRunTimes[i] = currentTime + repetitionTimes[i];
                } else {
                    nextRunTimes[i] = 0; // Stop scheduling further runs
                    Serial.print("Motor ");
                    Serial.print((char)('X' + i));
                    Serial.println(" completed all cycles.");
                }
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

// Helper functions remain the same as the original code...

// ...


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

// Handle TIME command in HH:MM:SS
void handleTimeCommand(String command) {
    int motorIndex = getMotorIndex(command.charAt(5));
    if (motorIndex >= 0) {
        String timeStr = command.substring(7);
        int hours = timeStr.substring(0, 2).toInt();
        int minutes = timeStr.substring(3, 5).toInt();
        int seconds = timeStr.substring(6, 8).toInt();
        rotationTimes[motorIndex] = (hours * 3600UL + minutes * 60 + seconds) * 1000;
        Serial.print("Rotation time for Motor ");
        Serial.print((char)('X' + motorIndex));
        Serial.print(" set to ");
        Serial.print(hours);
        Serial.print(":");
        Serial.print(minutes);
        Serial.print(":");
        Serial.println(seconds);
    }
}

// Handle REPEAT command in HH:MM:SS
void handleRepeatCommand(String command) {
    int motorIndex = getMotorIndex(command.charAt(7));
    if (motorIndex >= 0) {
        String repeatStr = command.substring(9);
        int hours = repeatStr.substring(0, 2).toInt();
        int minutes = repeatStr.substring(3, 5).toInt();
        int seconds = repeatStr.substring(6, 8).toInt();
        repetitionTimes[motorIndex] = (hours * 3600UL + minutes * 60 + seconds) * 1000;
        Serial.print("Repetition time for Motor ");
        Serial.print((char)('X' + motorIndex));
        Serial.print(" set to ");
        Serial.print(hours);
        Serial.print(":");
        Serial.print(minutes);
        Serial.print(":");
        Serial.println(seconds);
    }
}

// Handle DIRECTION command
void handleDirectionCommand(String command) {
    int motorIndex = getMotorIndex(command.charAt(10)); // Get motor index
    if (motorIndex >= 0) {
        String direction = command.substring(12);
        if (direction.equalsIgnoreCase("FORWARD")) {
            motorDirection[motorIndex] = true;
        } else if (direction.equalsIgnoreCase("BACKWARD")) {
            motorDirection[motorIndex] = false;
        }
        Serial.print("Direction for Motor ");
        Serial.print((char)('X' + motorIndex));
        Serial.print(" set to ");
        Serial.println(direction);
    }
}

// Handle SPEED command
void handleSpeedCommand(String command) {
    int motorIndex = getMotorIndex(command.charAt(6));
    if (motorIndex >= 0) {
        int speed = command.substring(8).toInt();
        if (speed >= 1 && speed <= 1000) {
            motorSpeeds[motorIndex] = speed;
            steppers[motorIndex]->setSpeed(speed);
            Serial.print("Speed for Motor ");
            Serial.print((char)('X' + motorIndex));
            Serial.print(" set to ");
            Serial.println(speed);
        } else {
            Serial.println("Invalid speed. Must be between 1 and 1000.");
        }
    }
}

// Handle CYCLE command
void handleCycleCommand(String command) {
    int motorIndex = getMotorIndex(command.charAt(6));
    if (motorIndex >= 0) {
        int cycles = command.substring(8).toInt();
        numCycles[motorIndex] = cycles;
        Serial.print("Number of cycles for Motor ");
        Serial.print((char)('X' + motorIndex));
        Serial.print(" set to ");
        Serial.println(cycles);
    }
}

// Helper function to get motor index from character
int getMotorIndex(char motor) {
    switch (motor) {
        case 'X': return 0;
        case 'Y': return 1;
        case 'Z': return 2;
        case 'A': return 3;
        default: return -1;
    }
}
