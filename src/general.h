Adafruit_NeoPixel pixel(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Defines macros to use for better code readability
// also for easily changing the parameters of the maze
#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3
#define NO_MOVE 4
#define TURN_RIGHT 1
#define TURN_LEFT -1
#define TURN_AROUND 2
#define TURN_MAINTAIN 0
#define MAX_MAZE_SIZE 5
#define MAX_DISTANCE 255
#define MAZE_CELL_SIZE 90

// Neopixel color consts
const uint8_t GREEN = 0, BLUE = 1, YELLOW = 2, RED = 3, OFF = 4;

// .......look this is either genius or stupid
uint8_t bias[4][4] = {{RIGHT, DOWN, UP, LEFT},
                      {UP, LEFT, RIGHT, DOWN},
                      {LEFT, UP, DOWN, RIGHT},
                      {DOWN, RIGHT, LEFT, UP}};

// initialize structs

struct xyPair_t {
    int8_t x;
    int8_t y;
};

struct mouse_t {
    struct xyPair_t pos;
    int8_t direction;
};

struct neighbors_t {
    struct xyPair_t direction[4];
};

struct matrix_t {
    uint8_t matrix[MAX_MAZE_SIZE][MAX_MAZE_SIZE];
};

// // This maze is only used for development purposes
// // The mouse will never access this maze during operation
// struct matrix_t maze;
// void initVirtualMaze() {  // Call this function before anything else to load
// the
//                           // hardcoded Serial maze into the maze struct
//     uint8_t mazeArray[MAX_MAZE_SIZE][MAX_MAZE_SIZE] = {
//         {3, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 2},
//         {2, 3, 3, 1, 1, 0, 0, 3, 1, 1, 2, 1, 2},
//         {2, 0, 1, 1, 1, 1, 2, 2, 3, 2, 2, 2, 2},
//         {2, 3, 1, 1, 1, 0, 1, 0, 2, 0, 0, 2, 2},
//         {2, 1, 1, 1, 1, 2, 3, 1, 0, 1, 2, 2, 2},
//         {3, 2, 1, 1, 1, 2, 3, 1, 1, 1, 2, 0, 2},
//         {2, 1, 1, 1, 0, 2, 0, 3, 1, 1, 2, 3, 2},
//         {2, 1, 1, 3, 2, 2, 1, 2, 3, 2, 2, 2, 2},
//         {2, 1, 2, 2, 2, 1, 1, 0, 2, 2, 2, 0, 2},
//         {3, 1, 0, 2, 2, 2, 1, 1, 0, 0, 3, 2, 2},
//         {3, 2, 3, 0, 2, 2, 2, 1, 1, 3, 2, 2, 2},
//         {2, 0, 1, 0, 2, 2, 1, 1, 1, 0, 2, 0, 2},
//         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}};
//     for (uint8_t y = 0; y < MAX_MAZE_SIZE; y++) {
//         for (uint8_t x = 0; x < MAX_MAZE_SIZE; x++) {
//             maze.matrix[y][x] = mazeArray[y][x];
//         }
//     }
// }

// Development function for printing a matrix in a human-readable manner
void printMatrix(struct matrix_t matrix) {
    Serial.println("Matrix:");
    for (uint8_t y = 0; y < MAX_MAZE_SIZE; y++) {
        Serial.print("{");
        for (uint8_t x = 0; x < MAX_MAZE_SIZE; x++) {
            if (matrix.matrix[y][x] != MAX_DISTANCE) {
                if (matrix.matrix[y][x] < 10) Serial.print("0");
                Serial.print(matrix.matrix[y][x]);
                Serial.print(", ");
            } else {
                Serial.print("99, ");
            }
        }
        Serial.println("},");
    }
}

// Development function for printing a move
// Moves are stored as integers 0 to 4
// In the code, #defines are used, but those don't work in the terminal
// This function converts them into readable strings
void printMove(uint8_t move) {
    switch (move) {
        case RIGHT:
            Serial.print("Right");
            break;
        case UP:
            Serial.print("Up");
            break;
        case LEFT:
            Serial.print("Left");
            break;
        case DOWN:
            Serial.print("Down");
            break;
        case NO_MOVE:
            Serial.print("No Move");
            break;
    }
}

// This function returns a struct of type neighbors_t that contains four
// structs of type xyPair_t that contain coordinate pairs of the neighboring
// cells to the input cell.
struct neighbors_t neighbors(struct xyPair_t cell) {
    struct neighbors_t neighbors;

    neighbors.direction[RIGHT].y = cell.y;
    neighbors.direction[RIGHT].x = cell.x + 1;

    neighbors.direction[DOWN].y = cell.y + 1;
    neighbors.direction[DOWN].x = cell.x;

    neighbors.direction[UP].y = cell.y - 1;
    neighbors.direction[UP].x = cell.x;

    neighbors.direction[LEFT].y = cell.y;
    neighbors.direction[LEFT].x = cell.x - 1;

    return (neighbors);
}

// Neopixel functions
void setColor(uint8_t color) {
    switch (color) {
        case RED:
            pixel.setPixelColor(0, 255, 0, 0);
            break;
        case GREEN:
            pixel.setPixelColor(0, 0, 255, 0);
            break;
        case BLUE:
            pixel.setPixelColor(0, 0, 0, 255);
            break;
        case YELLOW:
            pixel.setPixelColor(0, 255, 255, 0);
            break;
        case OFF:
            pixel.setPixelColor(0, 0, 0, 0);
            break;
    }
    pixel.show();
}

void pixelInit() {
    pixel.begin();
    pixel.setPixelColor(0, 0, 0, 0);
    pixel.show();
}

void buttonCheckpoint() {
    int time = millis();
    bool toggle = false;
    while (digitalRead(0) == HIGH) {
        if (millis() - time > 500) {
            if (toggle) {
                setColor(BLUE);
                toggle = false;
            } else {
                setColor(OFF);
                toggle = true;
            }
            time = millis();
        }
    }
    setColor(BLUE);
    delay(3000);
}

// Integer input from Serial
int16_t getInput() {
    int choice = 0;
    while (1) {
        if (Serial.available() > 0) {
            char incomingByte = Serial.read();
            if (incomingByte == '.') break;
            choice = choice * 10 + incomingByte - '0';
        }
    }
    return (choice);
}

void failure(uint8_t code) {
    void allStop();
    allStop();
    Serial.print("ERROR CODE: ");
    Serial.println(code);
    while (1) {
        for (uint8_t i = 0; i < code; i++) {
            setColor(RED);
            delay(500);
            setColor(OFF);
            delay(500);
        }
        delay(1000);
    }
}

// Global structs that need to be accessed by multiple functions
struct matrix_t memory;

struct mouse_t mouse;

int16_t globalOffset;