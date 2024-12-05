#include <Arduino.h>
#include <Adafruit_BNO08x.h>    //IMU
#include <Adafruit_NeoPixel.h>  // Status Indicator
#include <ESP32Encoder.h>  // https://github.com/madhephaestus/ESP32Encoder.git
#include <cppQueue.h>      // Navigation
#include <PID_v1.h>        // PID
#include <Wire.h>          // ToF
#include "Adafruit_VL53L0X.h"  // ToF

#include "general.h"
#include "IMU.h"
#include "ToF.h"
#include "PID.h"
#include "motion.h"
#include "navigation.h"

// Edit these defines to alter the behavior of the mouse
#define START_Y 0
#define START_X 0
#define START_DIRECTION RIGHT
#define GOAL_Y 1
#define GOAL_X 1

void setup() {
    Serial.begin(19200);
    delay(1000);
    Serial.println("Serial Init");
    pixelInit();
    setColor(YELLOW);
    IMUInit();
    pinMode(0, INPUT_PULLUP);
    Wire.begin();
    ToF_Left.init();
    ToF_Front.init();
    ToF_Right.init();

    RotationalPID.SetMode(AUTOMATIC);
    RotationalPID.SetOutputLimits(-255, 255);
    RotationalPID.SetSampleTime(10);
    LinearLPID.SetMode(AUTOMATIC);
    LinearLPID.SetOutputLimits(-255, 255);
    LinearLPID.SetSampleTime(10);
    LinearRPID.SetMode(AUTOMATIC);
    LinearRPID.SetOutputLimits(-255, 255);
    LinearRPID.SetSampleTime(10);

    // initVirtualMaze();  // FOR DEVELOPMENT PURPOSES ONLY

    // initialize memory to all 0
    for (uint8_t y = 0; y < MAX_MAZE_SIZE; y++) {
        for (uint8_t x = 0; x < MAX_MAZE_SIZE; x++) {
            memory.matrix[y][x] = 0;
        }
    }

    // // FOR TESTING PURPOSES ONLY
    // for (uint8_t y = 0; y < MAX_MAZE_SIZE; y++) {
    //   for (uint8_t x = 0; x < MAX_MAZE_SIZE; x++) {
    //     memory.matrix[y][x] = maze.matrix[y][x];
    //   }
    // }

    // load start and goal coordinates
    mouse.direction = START_DIRECTION;
    mouse.pos.y = START_Y;
    mouse.pos.x = START_X;
    initGlobalOffset(START_DIRECTION);
}

long lastEncoder = 0;
long lastUpdate = 0;
int16_t lastSpeed;
float target = 0;

void loop() {
    // if (Serial.available()) {
    //     int choice = 0;
    //     char incomingByte;
    //     while (1) {
    //         if (Serial.available() > 0) {
    //             incomingByte = Serial.read();
    //             if (incomingByte < '0' || incomingByte > '9') break;
    //             choice = choice * 10 + incomingByte - '0';
    //         }
    //     }
    //     if (incomingByte == 'p') {
    //         LinearKp = choice;
    //         LinearPID.SetTunings(choice, LinearKi, LinearKd);
    //     } else if (incomingByte == 'i') {
    //         LinearKi = choice;
    //         LinearPID.SetTunings(LinearKp, choice, LinearKd);
    //     } else if (incomingByte == 'd') {
    //         LinearKd = choice;
    //         LinearPID.SetTunings(LinearKp, LinearKi, choice);
    //     } else
    //         for (int i = 0; i < 1000; i++) Serial.println("WHAT!");
    // }
    // Serial.print("Kp: ");
    // Serial.print(LinearKp);
    // Serial.print(" Ki: ");
    // Serial.print(LinearKi);
    // Serial.print(" Kd: ");
    // Serial.print(LinearKd);
    // Serial.print("\n");

    // Serial.print("Left: ");
    // Serial.print(ToF_Left.detectWall());
    // Serial.print(" Front: ");
    // Serial.print(ToF_Front.detectWall());
    // Serial.print(" Right: ");
    // Serial.print(ToF_Right.detectWall());
    // Serial.print("\n");

    // for (uint8_t i = 0; i < 4; i++) {
    //     forward(1);
    //     turn(TURN_RIGHT);
    // }

    // if (millis() - lastUpdate > 1000) {
    // turn(TURN_RIGHT);
    // turn(TURN_LEFT);
    // turn(TURN_LEFT);
    // turn(TURN_AROUND);
    // delay(1000);
    //     lastUpdate = millis();
    // }
    // delay(100);

    // setColor(GREEN);
    // turn(TURN_RIGHT);
    // turn(TURN_LEFT);
    // setColor(BLUE);
    // turn(TURN_LEFT);
    // turn(TURN_RIGHT);

    // buttonCheckpoint();

    struct xyPair_t goalPos;
    goalPos.y = GOAL_Y;
    goalPos.x = GOAL_X;

    struct xyPair_t startPos;
    startPos.y = START_Y;
    startPos.x = START_X;

    buttonCheckpoint();

    navigate(goalPos);

    // // buttonCheckpoint();

    // // navigate(startPos);

    // // buttonCheckpoint();

    // // sprint(goalPos);

    setColor(GREEN);

    while (1) {
        delay(1);
    }
}