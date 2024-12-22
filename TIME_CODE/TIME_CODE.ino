// #include <AccelStepper.h>

// const int stepX = 2;    // X-axis step pin
// const int dirX = 5;     // X-axis direction pin
// const int stepY = 3;    // Y-axis step pin
// const int dirY = 6;     // Y-axis direction pin
// const int stepZ = 4;    // Z-axis step pin
// const int dirZ = 7;     // Z-axis direction pin
// const int stepA = 9;    // A-axis step pin
// const int dirA = 8;     // A-axis direction pin

// // Define each motor on a separate axis
// AccelStepper stepperX(AccelStepper::DRIVER, stepX, dirX);
// AccelStepper stepperY(AccelStepper::DRIVER, stepY, dirY);
// AccelStepper stepperZ(AccelStepper::DRIVER, stepZ, dirZ);
// AccelStepper stepperA(AccelStepper::DRIVER, stepA, dirA);

// int rotationTimeX = 10, rotationTimeY = 10, rotationTimeZ = 10, rotationTimeA = 10;
// bool motorRunningX = false, motorRunningY = false, motorRunningZ = false, motorRunningA = false;
// unsigned long startTimeX, startTimeY, startTimeZ, startTimeA;
// unsigned long elapsedTimeX, elapsedTimeY, elapsedTimeZ, elapsedTimeA;

// void setup() {
//   stepperX.setMaxSpeed(100);
//   stepperX.setSpeed(00);
//   stepperY.setMaxSpeed(1000);
//   stepperY.setSpeed(500);
//   stepperZ.setMaxSpeed(1000);
//   stepperZ.setSpeed(500);
//   stepperA.setMaxSpeed(1000);
//   stepperA.setSpeed(500);

//   Serial.begin(9600);
//   Serial.println("Commands:");
//   Serial.println("START <X/Y/Z/A> - Start motor on axis");
//   Serial.println("STOP <X/Y/Z/A> - Stop motor on axis");
//   Serial.println("TIME <X/Y/Z/A> <seconds> - Set rotation time on axis");
// }

// void loop() {
//   // Check for Serial input
//   if (Serial.available() > 0) {
//     String command = Serial.readStringUntil('\n');
//     command.trim();

//     if (command.startsWith("START")) {
//       handleStartCommand(command);
//     } else if (command.startsWith("STOP")) {
//       handleStopCommand(command);
//     } else if (command.startsWith("TIME")) {
//       handleTimeCommand(command);
//     } else {
//       Serial.println("Unknown command. Use START <X/Y/Z/A>, STOP <X/Y/Z/A>, or TIME <X/Y/Z/A> <seconds>.");
//     }
//   }

//   // Update motor running and timing for each axis
//   updateMotor(stepperX, motorRunningX, startTimeX, elapsedTimeX, rotationTimeX, "X");
//   updateMotor(stepperY, motorRunningY, startTimeY, elapsedTimeY, rotationTimeY, "Y");
//   updateMotor(stepperZ, motorRunningZ, startTimeZ, elapsedTimeZ, rotationTimeZ, "Z");
//   updateMotor(stepperA, motorRunningA, startTimeA, elapsedTimeA, rotationTimeA, "A");

//   delay(100);
// }

// // Function to handle START command
// void handleStartCommand(String command) {
//   if (command.endsWith("X")) {
//     if (!motorRunningX) {
//       motorRunningX = true;
//       startTimeX = millis();
//       Serial.println("Motor X started.");
//     }
//   } else if (command.endsWith("Y")) {
//     if (!motorRunningY) {
//       motorRunningY = true;
//       startTimeY = millis();
//       Serial.println("Motor Y started.");
//     }
//   } else if (command.endsWith("Z")) {
//     if (!motorRunningZ) {
//       motorRunningZ = true;
//       startTimeZ = millis();
//       Serial.println("Motor Z started.");
//     }
//   } else if (command.endsWith("A")) {
//     if (!motorRunningA) {
//       motorRunningA = true;
//       startTimeA = millis();
//       Serial.println("Motor A started.");
//     }
//   } else {
//     Serial.println("Invalid axis. Use START <X/Y/Z/A>.");
//   }
// }

// // Function to handle STOP command
// void handleStopCommand(String command) {
//   if (command.endsWith("X")) {
//     motorRunningX = false;
//     Serial.println("Motor X stopped.");
//   } else if (command.endsWith("Y")) {
//     motorRunningY = false;
//     Serial.println("Motor Y stopped.");
//   } else if (command.endsWith("Z")) {
//     motorRunningZ = false;
//     Serial.println("Motor Z stopped.");
//   } else if (command.endsWith("A")) {
//     motorRunningA = false;
//     Serial.println("Motor A stopped.");
//   } else {
//     Serial.println("Invalid axis. Use STOP <X/Y/Z/A>.");
//   }
// }

// // Function to handle TIME command
// void handleTimeCommand(String command) {
//   char axis = command.charAt(5);
//   int timeValue = command.substring(7).toInt();
//   if (timeValue <= 0) {
//     Serial.println("Invalid time. Please enter a positive integer.");
//     return;
//   }
  
//   if (axis == 'X') {
//     rotationTimeX = timeValue;
//     Serial.print("Rotation time for X set to ");
//   } else if (axis == 'Y') {
//     rotationTimeY = timeValue;
//     Serial.print("Rotation time for Y set to ");
//   } else if (axis == 'Z') {
//     rotationTimeZ = timeValue;
//     Serial.print("Rotation time for Z set to ");
//   } else if (axis == 'A') {
//     rotationTimeA = timeValue;
//     Serial.print("Rotation time for A set to ");
//   } else {
//     Serial.println("Invalid axis. Use TIME <X/Y/Z/A> <seconds>.");
//     return;
//   }
//   Serial.print(timeValue);
//   Serial.println(" seconds.");
// }

// // Function to update motor status
// void updateMotor(AccelStepper &stepper, bool &motorRunning, unsigned long &startTime, unsigned long &elapsedTime, int rotationTime, String axis) {
//   if (motorRunning) {
//     elapsedTime = (millis() - startTime) / 1000;
//     Serial.print("Elapsed Time for ");
//     Serial.print(axis);
//     Serial.print(": ");
//     Serial.print(elapsedTime);
//     Serial.println(" seconds");

//     if (elapsedTime >= rotationTime) {
//       motorRunning = false;
//       Serial.print("Motor ");
//       Serial.print(axis);
//       Serial.println(" stopped after reaching set time.");
//     } else {
//       stepper.runSpeed();
//     }
//   }
// }

#include <AccelStepper.h>

const int stepX = 2;    // X-axis step pin
const int dirX = 5;     // X-axis direction pin
const int stepY = 3;    // Y-axis step pin
const int dirY = 6;     // Y-axis direction pin
const int stepZ = 4;    // Z-axis step pin
const int dirZ = 7;     // Z-axis direction pin
const int stepA = 9;    // A-axis step pin
const int dirA = 8;     // A-axis direction pin

// Define each motor on a separate axis
AccelStepper stepperX(AccelStepper::DRIVER, stepX, dirX);
AccelStepper stepperY(AccelStepper::DRIVER, stepY, dirY);
AccelStepper stepperZ(AccelStepper::DRIVER, stepZ, dirZ);
AccelStepper stepperA(AccelStepper::DRIVER, stepA, dirA);

int rotationTimeX = 10, rotationTimeY = 10, rotationTimeZ = 10, rotationTimeA = 10;
bool motorRunningX = false, motorRunningY = false, motorRunningZ = false, motorRunningA = false;
unsigned long startTimeX, startTimeY, startTimeZ, startTimeA;
unsigned long elapsedTimeX, elapsedTimeY, elapsedTimeZ, elapsedTimeA;

void setup() {
  // stepperX.setSpeed(500);
  // stepperX.setMaxSpeed(1000);
  stepperX.setSpeed(500);
  stepperY.setMaxSpeed(1000);
  stepperY.setSpeed(500);
  stepperZ.setMaxSpeed(1000);
  stepperZ.setSpeed(500);
  stepperA.setMaxSpeed(1000);
  stepperA.setSpeed(500);

  Serial.begin(9600);
  Serial.println("Commands:");
  Serial.println("START <X/Y/Z/A> - Start motor on axis");
  Serial.println("STOP <X/Y/Z/A> - Stop motor on axis");
  Serial.println("TIME <X/Y/Z/A> <seconds> - Set rotation time on axis");
}

void loop() {
  // Check for Serial input
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("START")) handleStartCommand(command);
    else if (command.startsWith("STOP")) handleStopCommand(command);
    else if (command.startsWith("TIME")) handleTimeCommand(command);
    else {
      Serial.println("Unknown command. Use START <X/Y/Z/A>, STOP <X/Y/Z/A>, or TIME <X/Y/Z/A> <seconds>.");
    }
  }

  // Update motor running and timing for each axis
  updateMotor(stepperX, motorRunningX, startTimeX, elapsedTimeX, rotationTimeX, "X");
  updateMotor(stepperY, motorRunningY, startTimeY, elapsedTimeY, rotationTimeY, "Y");
  updateMotor(stepperZ, motorRunningZ, startTimeZ, elapsedTimeZ, rotationTimeZ, "Z");
  updateMotor(stepperA, motorRunningA, startTimeA, elapsedTimeA, rotationTimeA, "A");

  delay(100);
}

// Function to handle START command
void handleStartCommand(String command) {
  if (command.endsWith("X")) {
    if (!motorRunningX) {
      motorRunningX = true;
      startTimeX = millis();
      Serial.println("Motor X started.");
    }
  } else if (command.endsWith("Y")) {
    if (!motorRunningY) {
      motorRunningY = true;
      startTimeY = millis();
      Serial.println("Motor Y started.");
    }
  } else if (command.endsWith("Z")) {
    if (!motorRunningZ) {
      motorRunningZ = true;
      startTimeZ = millis();
      Serial.println("Motor Z started.");
    }
  } else if (command.endsWith("A")) {
    if (!motorRunningA) {
      motorRunningA = true;
      startTimeA = millis();
      Serial.println("Motor A started.");
    }
  } else {
    Serial.println("Invalid axis. Use START <X/Y/Z/A>.");
  }
}

// Function to handle STOP command
void handleStopCommand(String command) {
  if (command.endsWith("X")) {
    motorRunningX = false;
    Serial.println("Motor X stopped.");
  } else if (command.endsWith("Y")) {
    motorRunningY = false;
    Serial.println("Motor Y stopped.");
  } else if (command.endsWith("Z")) {
    motorRunningZ = false;
    Serial.println("Motor Z stopped.");
  } else if (command.endsWith("A")) {
    motorRunningA = false;
    Serial.println("Motor A stopped.");
  } else {
    Serial.println("Invalid axis. Use STOP <X/Y/Z/A>.");
  }
}

// Function to handle TIME command
void handleTimeCommand(String command) {
  char axis = command.charAt(5);
  int timeValue = command.substring(7).toInt();
  if (timeValue <= 0) {
    Serial.println("Invalid time. Please enter a positive integer.");
    return;
  }
  
  if (axis == 'X') {
    rotationTimeX = timeValue;
    Serial.print("Rotation time for X set to ");
  } else if (axis == 'Y') {
    rotationTimeY = timeValue;
    Serial.print("Rotation time for Y set to ");
  } else if (axis == 'Z') {
    rotationTimeZ = timeValue;
    Serial.print("Rotation time for Z set to ");
  } else if (axis == 'A') {
    rotationTimeA = timeValue;
    Serial.print("Rotation time for A set to ");
  } else {
    Serial.println("Invalid axis. Use TIME <X/Y/Z/A> <seconds>.");
    return;
  }
  Serial.print(timeValue);
  Serial.println(" seconds.");
}

// Function to update motor status
void updateMotor(AccelStepper &stepper, bool &motorRunning, unsigned long &startTime, unsigned long &elapsedTime, int rotationTime, String axis) {
  if (motorRunning) {
    elapsedTime = (millis() - startTime) / 1000;
    Serial.print("Elapsed Time for ");
    Serial.print(axis);
    Serial.print(": ");
    Serial.print(elapsedTime);
    Serial.println(" seconds");

    if (elapsedTime >= rotationTime) {
      motorRunning = false;
      Serial.print("Motor ");
      Serial.print(axis);
      Serial.println(" stopped after reaching set time.");
    } else {
      stepper.runSpeed();
    }
  }
}


// #include <AccelStepper.h>

// const int stepX = 2;    // X-axis step pin
// const int dirX = 5;     // X-axis direction pin
// const int stepY = 3;    // Y-axis step pin
// const int dirY = 6;     // Y-axis direction pin
// const int stepZ = 4;    // Z-axis step pin
// const int dirZ = 7;     // Z-axis direction pin
// const int stepA = 9;    // A-axis step pin
// const int dirA = 8;     // A-axis direction pin

// // Define each motor on a separate axis
// AccelStepper stepperX(AccelStepper::DRIVER, stepX, dirX);
// AccelStepper stepperY(AccelStepper::DRIVER, stepY, dirY);
// AccelStepper stepperZ(AccelStepper::DRIVER, stepZ, dirZ);
// AccelStepper stepperA(AccelStepper::DRIVER, stepA, dirA);

// // Default values for speed, acceleration, and time for each axis
// float speedX = 100, speedY = 100, speedZ = 100, speedA = 100;
// float accelX = 50, accelY = 50, accelZ = 50, accelA = 50;
// int rotationTimeX = 10, rotationTimeY = 10, rotationTimeZ = 10, rotationTimeA = 10;

// bool motorRunningX = false, motorRunningY = false, motorRunningZ = false, motorRunningA = false;
// unsigned long startTimeX, startTimeY, startTimeZ, startTimeA;
// unsigned long elapsedTimeX, elapsedTimeY, elapsedTimeZ, elapsedTimeA;

// void setup() {
//   Serial.begin(9600);
  
//   // Set initial speeds and accelerations
//   setMotorParameters(stepperX, speedX, accelX);
//   setMotorParameters(stepperY, speedY, accelY);
//   setMotorParameters(stepperZ, speedZ, accelZ);
//   setMotorParameters(stepperA, speedA, accelA);

//   Serial.println("Commands:");
//   Serial.println("START <X/Y/Z/A> - Start motor on axis");
//   Serial.println("STOP <X/Y/Z/A> - Stop motor on axis");
//   Serial.println("TIME <X/Y/Z/A> <seconds> - Set rotation time on axis");
//   Serial.println("SPEED <X/Y/Z/A> <value> - Set speed for motor on axis");
//   Serial.println("ACCEL <X/Y/Z/A> <value> - Set acceleration for motor on axis");
// }

// void loop() {
//   // Check for Serial input
//   if (Serial.available() > 0) {
//     String command = Serial.readStringUntil('\n');
//     command.trim();

//     if (command.startsWith("START")) handleStartCommand(command);
//     else if (command.startsWith("STOP")) handleStopCommand(command);
//     else if (command.startsWith("TIME")) handleTimeCommand(command);
//     else if (command.startsWith("SPEED")) handleSpeedCommand(command);
//     else if (command.startsWith("ACCEL")) handleAccelCommand(command);
//     else Serial.println("Unknown command. Use START, STOP, TIME, SPEED, or ACCEL.");
//   }

//   // Update motor running and timing for each axis
//   updateMotor(stepperX, motorRunningX, startTimeX, elapsedTimeX, rotationTimeX, "X");
//   updateMotor(stepperY, motorRunningY, startTimeY, elapsedTimeY, rotationTimeY, "Y");
//   updateMotor(stepperZ, motorRunningZ, startTimeZ, elapsedTimeZ, rotationTimeZ, "Z");
//   updateMotor(stepperA, motorRunningA, startTimeA, elapsedTimeA, rotationTimeA, "A");

//   delay(100);
// }

// void handleStartCommand(String command) {
//   if (command.endsWith("X")) startMotor(motorRunningX, startTimeX, "X");
//   else if (command.endsWith("Y")) startMotor(motorRunningY, startTimeY, "Y");
//   else if (command.endsWith("Z")) startMotor(motorRunningZ, startTimeZ, "Z");
//   else if (command.endsWith("A")) startMotor(motorRunningA, startTimeA, "A");
// }

// void handleStopCommand(String command) {
//   if (command.endsWith("X")) stopMotor(motorRunningX, "X");
//   else if (command.endsWith("Y")) stopMotor(motorRunningY, "Y");
//   else if (command.endsWith("Z")) stopMotor(motorRunningZ, "Z");
//   else if (command.endsWith("A")) stopMotor(motorRunningA, "A");
// }

// void handleTimeCommand(String command) {
//   char axis = command.charAt(5);
//   int timeValue = command.substring(7).toInt();
//   if (timeValue <= 0) {
//     Serial.println("Invalid time. Please enter a positive integer.");
//     return;
//   }
  
//   if (axis == 'X') rotationTimeX = timeValue;
//   else if (axis == 'Y') rotationTimeY = timeValue;
//   else if (axis == 'Z') rotationTimeZ = timeValue;
//   else if (axis == 'A') rotationTimeA = timeValue;
//   Serial.print("Rotation time for ");
//   Serial.print(axis);
//   Serial.print(" set to ");
//   Serial.print(timeValue);
//   Serial.println(" seconds.");
// }

// void handleSpeedCommand(String command) {
//   char axis = command.charAt(6);
//   float speedValue = command.substring(8).toFloat();
//   if (axis == 'X') speedX = speedValue, setMotorParameters(stepperX, speedX, accelX);
//   else if (axis == 'Y') speedY = speedValue, setMotorParameters(stepperY, speedY, accelY);
//   else if (axis == 'Z') speedZ = speedValue, setMotorParameters(stepperZ, speedZ, accelZ);
//   else if (axis == 'A') speedA = speedValue, setMotorParameters(stepperA, speedA, accelA);
//   Serial.print("Speed for ");
//   Serial.print(axis);
//   Serial.print(" set to ");
//   Serial.println(speedValue);
// }

// void handleAccelCommand(String command) {
//   char axis = command.charAt(6);
//   float accelValue = command.substring(8).toFloat();
//   if (axis == 'X') accelX = accelValue, setMotorParameters(stepperX, speedX, accelX);
//   else if (axis == 'Y') accelY = accelValue, setMotorParameters(stepperY, speedY, accelY);
//   else if (axis == 'Z') accelZ = accelValue, setMotorParameters(stepperZ, speedZ, accelZ);
//   else if (axis == 'A') accelA = accelValue, setMotorParameters(stepperA, speedA, accelA);
//   Serial.print("Acceleration for ");
//   Serial.print(axis);
//   Serial.print(" set to ");
//   Serial.println(accelValue);
// }

// void startMotor(bool &motorRunning, unsigned long &startTime, const char *axis) {
//   if (!motorRunning) {
//     motorRunning = true;
//     startTime = millis();
//     Serial.print("Motor ");
//     Serial.print(axis);
//     Serial.println(" started.");
//   }
// }

// void stopMotor(bool &motorRunning, const char *axis) {
//   motorRunning = false;
//   Serial.print("Motor ");
//   Serial.print(axis);
//   Serial.println(" stopped.");
// }

// void updateMotor(AccelStepper &stepper, bool &motorRunning, unsigned long &startTime, unsigned long &elapsedTime, int rotationTime, const char *axis) {
//   if (motorRunning) {
//     elapsedTime = (millis() - startTime) / 1000;
//     if (elapsedTime >= rotationTime) {
//       motorRunning = false;
//       Serial.print("Motor ");
//       Serial.print(axis);
//       Serial.println(" stopped after reaching set time.");
//     } else {
//       stepper.runSpeed();
//     }
//   }
// }

// void setMotorParameters(AccelStepper &stepper, float speed, float accel) {
//   stepper.setMaxSpeed(speed);
//   stepper.setAcceleration(accel);
// }
