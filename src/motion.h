#define ENCODER_LEFT_A 9
#define ENCODER_LEFT_B 8
#define ENCODER_RIGHT_A 17
#define ENCODER_RIGHT_B 18
#define GEAR_RATIO 50
#define ENCODER_RATIO 12
#define CIRCUMFERENCE 69

#define MOTOR_LEFT_A 35
#define MOTOR_LEFT_B 37
#define MOTOR_RIGHT_A 16
#define MOTOR_RIGHT_B 36

const float TRIM_VALUE = 19 / 18.4;
const float TURN_TRIM = 1.1;

class motor_t {
   public:
    int32_t speed;

    motor_t(uint8_t motora, uint8_t motorb, uint8_t encoderA,
            uint8_t encoderB) {
        speed = 0;
        motorA = motora;
        motorB = motorb;
        encoder.attachFullQuad(encoderA, encoderB);
        encoder.setCount(0);
        pinMode(motorA, OUTPUT);
        pinMode(motorB, OUTPUT);
        digitalWrite(motorA, LOW);
        digitalWrite(motorB, LOW);
    }

    float getDistance() {
        return (encoder.getCount() * ENCODER_RATIO * GEAR_RATIO *
                CIRCUMFERENCE * TRIM_VALUE);
    }

    int getCount() { return (encoder.getCount()); }

    void forward() {
        digitalWrite(motorA, LOW);
        digitalWrite(motorB, HIGH);
    }
    void back() {
        digitalWrite(motorA, HIGH);
        digitalWrite(motorB, LOW);
    }
    void stop() {
        digitalWrite(motorA, LOW);
        digitalWrite(motorB, LOW);
    }

    void PWMRun() {
        uint16_t onTime;
        if (speed < 0) {
            if (speed < -255)
                onTime = 1000;
            else
                onTime = map(speed * -1, 0, 255, 0, 1000);
            back();
        } else {
            if (speed > 255)
                onTime = 1000;
            else
                onTime = map(speed, 0, 255, 0, 1000);
            forward();
        }
        uint16_t offTime = 1000 - onTime;
        delayMicroseconds(onTime);
        stop();
        delayMicroseconds(offTime);
    }

   private:
    uint8_t motorA, motorB;
    ESP32Encoder encoder;
};

motor_t motorR(MOTOR_RIGHT_A, MOTOR_RIGHT_B, ENCODER_RIGHT_A, ENCODER_RIGHT_B);
motor_t motorL(MOTOR_LEFT_A, MOTOR_LEFT_B, ENCODER_LEFT_A, ENCODER_LEFT_B);

void rotate(int32_t speed) {
    motorR.speed = -1 * speed;
    motorL.speed = speed;
    if (motorR.speed < 0) motorR.speed *= TURN_TRIM;
    if (motorL.speed < 0) motorL.speed *= TURN_TRIM;
}

// NEEDS HARDWARE INTERFACE
// Moves the mouse forward a given number of cells
void forward(uint8_t number) {
    for (uint8_t i = 0; i < number; i++) {
        // Update the mouse's virtual position
        switch (mouse.direction) {
            case RIGHT:
                mouse.pos.x++;
                break;
            case UP:
                mouse.pos.y--;
                break;
            case LEFT:
                mouse.pos.x--;
                break;
            case DOWN:
                mouse.pos.y++;
                break;
        }
    }
    delay(200);  // TESTING ONLY PLEASE REMOVE THIS LINE
    // This will actually command the motors to turn on for a certain amount of
    // time
}

void maintainAngle(int16_t target, int16_t offset = 0) {
    Setpoint = target;
    Setpoint /= 360.0;

    Input = getAngle(offset);
    Input /= 360;

    myPID.Compute();

    rotate(Output);

    motorR.PWMRun();
    motorL.PWMRun();
}

void turn(int8_t direction) {
    // This actually commands the motors to turn make the turn
    int16_t offset = globalOffset;
    if (mouse.direction == RIGHT)
        offset += 180;
    else if (mouse.direction == UP)
        offset += 90;
    else if (mouse.direction == LEFT)
        offset += 0;
    else if (mouse.direction == DOWN)
        offset -= 90;

    uint32_t start = millis();
    int16_t target;

    if (direction == TURN_RIGHT)
        target = 90;
    else if (direction == TURN_LEFT)
        target = 270;
    else if (direction == TURN_AROUND) {
        target = 270;
        offset += 90;
    }
    while (millis() - start < 2500) {
        maintainAngle(target, offset);
    }

    // Updates virtual mouse's direction
    mouse.direction += direction;
    // accounts for overflow
    // I.E. if the direction was 0, turning right makes the direction -1, but we
    // want that to stay within 0-3
    while (mouse.direction < 0) mouse.direction += 4;
    while (mouse.direction > 3) mouse.direction -= 4;
}

// This function takes a move as input, positions the mouse to make it, and then
// makes the move
void makeMove(uint8_t move, uint8_t number) {
    // don't do anything if the move in NOMOVE
    if (move == NO_MOVE) {
        return;
    }
    // Turns the mouse to face the proper direction
    switch (move) {
        case RIGHT:
            switch (mouse.direction) {
                case UP:
                    turn(TURN_RIGHT);
                    break;
                case LEFT:
                    turn(TURN_AROUND);
                    break;
                case DOWN:
                    turn(TURN_LEFT);
                    break;
            }
            break;
        case UP:
            switch (mouse.direction) {
                case RIGHT:
                    turn(TURN_LEFT);
                    break;
                case LEFT:
                    turn(TURN_RIGHT);
                    break;
                case DOWN:
                    turn(TURN_AROUND);
                    break;
            }
            break;
        case LEFT:
            switch (mouse.direction) {
                case RIGHT:
                    turn(TURN_AROUND);
                    break;
                case UP:
                    turn(TURN_LEFT);
                    break;
                case DOWN:
                    turn(TURN_RIGHT);
                    break;
            }
            break;
        case DOWN:
            switch (mouse.direction) {
                case RIGHT:
                    turn(TURN_RIGHT);
                    break;
                case UP:
                    turn(TURN_AROUND);
                    break;
                case LEFT:
                    turn(TURN_LEFT);
                    break;
            }
            break;
    }
    // goes forward
    forward(number);
}
