#define TCAADDR 0x70

class ToF_t {
   public:
    ToF_t(uint8_t initId) { id = initId; }

    void init() {
        tcaselect(id);
        if (!sensor.begin()) {
            Serial.print("Failed to find ToF sensor ");
            Serial.println(id);
            failure(2);
        }
    }

    uint16_t getDistance() {
        VL53L0X_RangingMeasurementData_t measure;
        tcaselect(id);
        sensor.rangingTest(&measure, false);
        if (measure.RangeStatus != 4)
            return (measure.RangeMilliMeter);
        else
            return (9999);
    }

    bool detectWall() { return (getDistance() < MAZE_CELL_SIZE / 2); }

   private:
    uint8_t id;
    Adafruit_VL53L0X sensor;

    void tcaselect(uint8_t i) {
        if (i > 7) return;

        Wire.beginTransmission(TCAADDR);
        Wire.write(1 << i);
        Wire.endTransmission();
    }
};

ToF_t ToF_Front(0);
ToF_t ToF_Right(1);
ToF_t ToF_Left(2);

void addWall(uint8_t direction) {
    while (direction > 3) direction -= 4;
    while (direction < 0) direction += 4;

    if (direction == RIGHT) {
        if (memory.matrix[mouse.pos.y][mouse.pos.x + 1] == 1)
            memory.matrix[mouse.pos.y][mouse.pos.x + 1] = 3;
        else if (memory.matrix[mouse.pos.y][mouse.pos.x + 1] == 0)
            memory.matrix[mouse.pos.y][mouse.pos.x + 1] = 2;
    } else if (direction == UP) {
        if (memory.matrix[mouse.pos.y][mouse.pos.x] == 2)
            memory.matrix[mouse.pos.y][mouse.pos.x] = 3;
        else if (memory.matrix[mouse.pos.y][mouse.pos.x] == 0)
            memory.matrix[mouse.pos.y][mouse.pos.x] = 1;
    } else if (direction == LEFT) {
        if (memory.matrix[mouse.pos.y][mouse.pos.x] == 1)
            memory.matrix[mouse.pos.y][mouse.pos.x] = 3;
        else if (memory.matrix[mouse.pos.y][mouse.pos.x] == 0)
            memory.matrix[mouse.pos.y][mouse.pos.x] = 2;
    } else if (direction == DOWN) {
        if (memory.matrix[mouse.pos.y + 1][mouse.pos.x] == 2)
            memory.matrix[mouse.pos.y + 1][mouse.pos.x] = 3;
        else if (memory.matrix[mouse.pos.y + 1][mouse.pos.x] == 0)
            memory.matrix[mouse.pos.y + 1][mouse.pos.x] = 1;
    } else {
        while (1) {
            setColor(OFF);
            delay(500);
            setColor(RED);
            delay(500);
        }
    }
}

void measure() {
    // // Check what's visible to the mouse and store it into memory
    // // Is there a wall above or to the left of the mouse
    // memory.matrix[mouse.pos.y][mouse.pos.x] =
    //     maze.matrix[mouse.pos.y][mouse.pos.x];
    // // Is there a wall below the mouse
    // if (maze.matrix[mouse.pos.y + 1][mouse.pos.x] == 1 ||
    //     maze.matrix[mouse.pos.y + 1][mouse.pos.x] == 3) {
    //     // store the presence of a wall into memory
    //     if (memory.matrix[mouse.pos.y + 1][mouse.pos.x] != 3) {
    //         if (memory.matrix[mouse.pos.y + 1][mouse.pos.x] == 2) {
    //             memory.matrix[mouse.pos.y + 1][mouse.pos.x] = 3;
    //         } else {
    //             memory.matrix[mouse.pos.y + 1][mouse.pos.x] = 1;
    //         }
    //     }
    // }
    // // Is there a wall to the right of the mouse
    // if (maze.matrix[mouse.pos.y][mouse.pos.x + 1] == 2 ||
    //     maze.matrix[mouse.pos.y][mouse.pos.x + 1] == 3) {
    //     // store the presence of a wall into memory
    //     if (memory.matrix[mouse.pos.y][mouse.pos.x + 1] != 3) {
    //         if (memory.matrix[mouse.pos.y][mouse.pos.x + 1] == 1) {
    //             memory.matrix[mouse.pos.y][mouse.pos.x + 1] = 3;
    //         } else {
    //             memory.matrix[mouse.pos.y][mouse.pos.x + 1] = 2;
    //         }
    //     }
    // }
    if (ToF_Left.detectWall()) addWall(mouse.direction + 1);
    if (ToF_Front.detectWall()) addWall(mouse.direction);
    if (ToF_Right.detectWall()) addWall(mouse.direction - 1);
}