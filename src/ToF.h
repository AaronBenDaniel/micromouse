#define TCAADDR 0x70

class ToF_t {
   public:
    ToF_t(uint8_t initId, uint8_t initOffset) {
        id = initId;
        offset = initOffset;
    }

    void init() {
        tcaselect(id);
        if (!sensor.begin()) {
            Serial.print("Failed to find ToF sensor ");
            Serial.println(id);
            failure(2);
        }
        sensor.startRangeContinuous();
        getDistance();
    }

    uint16_t getDistance(bool forceRead = false) {
        tcaselect(id);
        if (sensor.isRangeComplete()) {
            distance = sensor.readRange();
            if (forceRead) {
                while (!sensor.isRangeComplete());  // Wait until next reading
                                                    // is ready
                distance = sensor.readRange();
            }
        }
        return (distance + offset);
    }

    bool detectWall() {
        uint16_t distance = getDistance(true);
        Serial.println(distance);
        return (distance < MAZE_CELL_SIZE);
    }

   private:
    uint8_t id;
    Adafruit_VL53L0X sensor;
    uint8_t offset;
    uint16_t distance;

    void tcaselect(uint8_t i) {
        if (i > 7) return;

        Wire.beginTransmission(TCAADDR);
        Wire.write(1 << i);
        Wire.endTransmission();
    }
};

ToF_t ToF_Front(0, 29);
ToF_t ToF_Right(1, 10);
ToF_t ToF_Left(2, 18);

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
        failure(4);
    }
}

// Check what's visible to the mouse and store it into memory
void measure() {
    setColor(PURPLE);
    if (ToF_Left.detectWall()) addWall(mouse.direction + 1);
    if (ToF_Front.detectWall()) addWall(mouse.direction);
    if (ToF_Right.detectWall()) addWall(mouse.direction - 1);
    setColor(YELLOW);
}