#define ENCODER_LEFT_A 8
#define ENCODER_LEFT_B 9
#define ENCODER_RIGHT_A 17
#define ENCODER_RIGHT_B 18
#define GEAR_RATIO 50
#define ENCODER_RATIO 12
#define CIRCUMFERENCE 71

#define MOTOR_LEFT_A 35
#define MOTOR_LEFT_B 37
#define MOTOR_RIGHT_A 16
#define MOTOR_RIGHT_B 36

#define CORRECTION_LOW 178
#define CORRECTION_HIGH 182

const float ENCODER_TRIM_VALUE = 19 / 18.4;
const float BACK_TURN_TRIM = 1.2;
const float RIGHT_TURN_TRIM = 1.15;

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
        ledcAttach(motorA, 1000, 8);
        ledcAttach(motorB, 1000, 8);
        stop();
    }

    float getDistance() {
        return (encoder.getCount() * ENCODER_RATIO * GEAR_RATIO *
                CIRCUMFERENCE * ENCODER_TRIM_VALUE);
    }

    int32_t getCount() { return (encoder.getCount()); }

    void forward(uint8_t amount = 255) {
        ledcWrite(motorA, 0);
        ledcWrite(motorB, amount);
    }
    void back(uint8_t amount = 255) {
        ledcWrite(motorA, amount);
        ledcWrite(motorB, 0);
    }
    void stop() {
        ledcWrite(motorA, 0);
        ledcWrite(motorB, 0);
    }

    void writeSpeed(int32_t amount) {
        speed = amount;
        if (speed < 0) {
            if (speed < -255) speed = -255;
            back(speed * -1);
        } else {
            if (speed > 255) speed = 255;
            forward(speed);
        }
    }

   private:
    uint8_t motorA, motorB;
    ESP32Encoder encoder;
};

motor_t motorR(MOTOR_RIGHT_A, MOTOR_RIGHT_B, ENCODER_RIGHT_A, ENCODER_RIGHT_B);
motor_t motorL(MOTOR_LEFT_A, MOTOR_LEFT_B, ENCODER_LEFT_A, ENCODER_LEFT_B);

void allStop() {
    motorL.stop();
    motorR.stop();
}

void rotate(int32_t speed) {
    if (speed > 0)
        motorR.writeSpeed(-1 * speed * RIGHT_TURN_TRIM * BACK_TURN_TRIM);
    else
        motorR.writeSpeed(-1 * speed * RIGHT_TURN_TRIM);
    if (speed < 0)
        motorL.writeSpeed(speed * BACK_TURN_TRIM);
    else
        motorL.writeSpeed(speed);
}

void maintainAngle(int16_t target, int16_t offset = 0) {
    RotationalSetpoint = target;
    RotationalSetpoint /= 360;

    RotationalInput = getAngle(offset);
    RotationalInput /= 360;

    RotationalPID.Compute();

    rotate(RotationalOutput / 4);
}

void turn(int8_t direction) {
    // This actually commands the motors to turn make the turn
    int16_t offset = globalOffset;
    if (mouse.direction == LEFT)
        offset -= 0;
    else if (mouse.direction == UP)
        offset -= 90;
    else if (mouse.direction == RIGHT)
        offset -= 180;
    else if (mouse.direction == DOWN)
        offset -= 270;

    // INTERMISSION
    // Updates virtual mouse's direction
    mouse.direction -= direction;
    // accounts for overflow
    // I.E. if the direction was 0, turning right makes the direction -1, but we
    // want that to stay within 0-3
    while (mouse.direction < 0) mouse.direction += 4;
    while (mouse.direction > 3) mouse.direction -= 4;
    Serial.print("\nCurrent direction: ");
    printMove(mouse.direction);
    Serial.print("\n");
    // END INTERMISSION

    uint32_t start = millis();
    int16_t target;

    if (direction == TURN_RIGHT)
        target = 90;
    else if (direction == TURN_LEFT)
        target = 270;
    else if (direction == TURN_AROUND) {
        target = 270;
        offset += 90;
    } else if (direction == TURN_MAINTAIN)
        target = 180;
    while (millis() - start < 1500) {
        maintainAngle(target, offset);
    }
}

void move(int16_t distance) {
    const uint8_t tolerance = 5;
    const uint8_t target = MAZE_CELL_SIZE * 5 / 8;

    const uint16_t offset = 280;
    int16_t rotationOffset = getAngle() + 180;
    int32_t encoderOffsetL = motorL.getCount();
    int16_t LeftDist[3];
    int16_t RightDist[3];
    if (distance > 0) {
        while (motorL.getCount() - encoderOffsetL + offset <
               distance * GEAR_RATIO * ENCODER_RATIO / CIRCUMFERENCE) {
            int16_t target;
            LeftDist[0] = ToF_Left.getDistance();
            RightDist[0] = ToF_Right.getDistance();

            LeftDist[2] = LeftDist[0];
            RightDist[2] = RightDist[0];

            if (abs(LeftDist[0] - LeftDist[1]) < 6 ||
                abs(LeftDist[0] - LeftDist[2]) < 6)
                LeftDist[1] = LeftDist[0];
            else
                LeftDist[0] = LeftDist[1];

            if (abs(RightDist[0] - RightDist[1]) < 6 ||
                abs(RightDist[0] - RightDist[2]) < 6)
                RightDist[1] = RightDist[0];
            else
                RightDist[0] = RightDist[1];

            target = 180 + (LeftDist[0] - RightDist[0]);

            if (target > CORRECTION_HIGH)
                target = CORRECTION_HIGH;
            else if (target < CORRECTION_LOW)
                target = CORRECTION_LOW;

            int16_t correction = 10 * (getAngle(rotationOffset) - target);
            motorL.writeSpeed(128 + correction);
            motorR.writeSpeed(128 - correction);

            Serial.print("Left: ");
            Serial.print(LeftDist[0]);
            Serial.print(" Right: ");
            Serial.print(RightDist[0]);
            Serial.print(" Difference: ");
            int16_t diff = LeftDist[0] - RightDist[0];
            Serial.print((diff<0)?'-':'+');
            if (abs(diff) < 100) Serial.print('0');
            if (abs(diff) < 10) Serial.print('0');
            Serial.print(abs(diff));
            Serial.print(" Correction: ");
            Serial.print(correction);

            Serial.print("\n");
        }
    } else {
        failure(4);
    }
    motorL.stop();
    delay(10);
    motorR.stop();
    delay(500);
}

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

    move(number * MAZE_CELL_SIZE);
}

// This function takes a move as input, positions the mouse to make it, and
// then makes the move
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
    turn(TURN_MAINTAIN);
}