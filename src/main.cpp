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
    myPID.SetMode(AUTOMATIC);
    myPID.SetOutputLimits(-255, 255);
    myPID.SetSampleTime(10);

    Serial.begin(19200);
    initVirtualMaze();  // FOR DEVELOPMENT PURPOSES ONLY

    // initialize memory to all 0
    for (uint8_t y = 0; y < MAXMAZESIZE; y++) {
        for (uint8_t x = 0; x < MAXMAZESIZE; x++) {
            memory.matrix[y][x] = 0;
        }
    }

    // // FOR TESTING PURPOSES ONLY
    // for (uint8_t y = 0; y < MAXMAZESIZE; y++) {
    //   for (uint8_t x = 0; x < MAXMAZESIZE; x++) {
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
}

long lastEncoder = 0;
long lastUpdate = 0;
int16_t lastSpeed;
float target = 0;

void loop() {
    if (millis() - lastUpdate > 5000) {
        turn_right();
        turn_right();
        lastUpdate = millis();
    }
    delay(100);
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