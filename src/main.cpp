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
#define GOAL_Y 4
#define GOAL_X 4

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
    struct xyPair_t goalPos;
    goalPos.y = GOAL_Y;
    goalPos.x = GOAL_X;

    struct xyPair_t startPos;
    startPos.y = START_Y;
    startPos.x = START_X;

    buttonCheckpoint();

    navigate(goalPos);

    buttonCheckpoint();

    navigate(startPos);

    buttonCheckpoint();

    sprint(goalPos);

    while (1) {
        delay(1);
    }
}