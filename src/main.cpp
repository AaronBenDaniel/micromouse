#include <Arduino.h>
#include <Adafruit_BNO08x.h>    //IMU
#include <Adafruit_NeoPixel.h>  // Status Indicator
#include <ESP32Encoder.h>  // https://github.com/madhephaestus/ESP32Encoder.git
#include <cppQueue.h>      // Navigation
#include <PID_v1.h>        // PID

#include "general.h"
#include "IMU.h"
#include "ToF.h"
#include "PID.h"
#include "motion.h"
#include "navigation.h"

// Edit these defines to alter the behavior of the mouse
#define START_Y 11
#define START_X 0
#define START_DIRECTION RIGHT
#define GOAL_Y 9
#define GOAL_X 10

void setup() {
    pixelInit();
    setColor(YELLOW);
    IMUInit();
    pinMode(0, INPUT_PULLUP);

    RotationalPID.SetMode(AUTOMATIC);
    RotationalPID.SetOutputLimits(-255, 255);
    RotationalPID.SetSampleTime(10);
    LinearLPID.SetMode(AUTOMATIC);
    LinearLPID.SetOutputLimits(-40, 255);
    LinearLPID.SetSampleTime(10);
    LinearRPID.SetMode(AUTOMATIC);
    LinearRPID.SetOutputLimits(-255, 255);
    LinearRPID.SetSampleTime(10);

    Serial.begin(19200);
    initVirtualMaze();  // FOR DEVELOPMENT PURPOSES ONLY

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
    setColor(BLUE);
    delay(3000);  // FOR DEVELOPMENT PURPOSES ONLY REMOVE EVENTUALLY
    setColor(GREEN);
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

    forward(1);
    // turn(TURN_RIGHT);
    // turn(TURN_RIGHT);
    // forward(1);
    // turn(TURN_LEFT);
    // turn(TURN_LEFT);

    // if (millis() - lastUpdate > 1000) {
    // turn(TURN_RIGHT);
    // turn(TURN_LEFT);
    // turn(TURN_LEFT);
    // turn(TURN_AROUND);
    // delay(1000);
    //     lastUpdate = millis();
    // }
    // delay(100);

    // struct xyPair_t goalPos;
    // goalPos.y = GOAL_Y;
    // goalPos.x = GOAL_X;

    // struct xyPair_t startPos;
    // startPos.y = START_Y;
    // startPos.x = START_X;

    // buttonCheckpoint();

    // // navigate(goalPos);

    // // buttonCheckpoint();

    // // navigate(startPos);

    // // buttonCheckpoint();

    // // sprint(goalPos);

    // setColor(GREEN);

    // while (1) {
    //   delay(1);
    //   Serial.println(getAngle());
    // }
}