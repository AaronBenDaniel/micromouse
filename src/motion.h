#define ENCODERLEFTA 9
#define ENCODERLEFTB 8
#define ENCODERRIGHTA 17
#define ENCODERRIGHTB 18
#define GEARRATIO 50
#define ENCODERRATIO 12
#define CIRCUMFERENCE 69

#define MOTORLEFTA 35
#define MOTORLEFTB 37
#define MOTORRIGHTA 16
#define MOTORRIGHTB 36

const float trimValue = 19 / 18.4;

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
        return (encoder.getCount() * ENCODERRATIO * GEARRATIO * CIRCUMFERENCE *
                trimValue);
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

motor_t motorR(MOTORRIGHTA, MOTORRIGHTB, ENCODERRIGHTA, ENCODERRIGHTB);
motor_t motorL(MOTORLEFTA, MOTORLEFTB, ENCODERLEFTA, ENCODERLEFTB);

void turn(int32_t speed) {
    motorR.speed = -1 * speed;
    motorL.speed = speed;
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

    Input = getAngle(0);
    Input /= 360;

    myPID.Compute();

    turn(Output);

    motorR.PWMRun();
    motorL.PWMRun();
}

// NEEDS HARDWARE INTERFACE
// Turns the mouse 90 degrees right
void turn_right() {
    // Updates virtual mouse's direction
    mouse.direction = mouse.direction - 1;
    // accounts for overflow
    // I.E. if the direction was 0, turning right makes the direction -1, but we
    // want that to stay within 0-3
    while (mouse.direction < 0) {
        mouse.direction = mouse.direction + 4;
    }

    // This actually commands the motors to turn make the turn
    int16_t offset = getAngle();
    delay(10);
    offset = getAngle() - 180;
    while (getAngle(offset) > 91 || getAngle(offset) < 89) {
        maintainAngle(90, offset);
    }
    delay(300);
}

// NEEDS HARDWARE INTERFACE
void turn_left() {
    mouse.direction = mouse.direction + 1;
    // accounts for overflow
    // I.E. if the direction was 3, turning left makes the direction 4, but we
    // want that to stay within 0-3
    while (mouse.direction > 3) {
        mouse.direction = mouse.direction - 4;
    }
    // This will actually command the motors to turn on in a certain direction
    // for a certain amount of time
}

// This function takes a move as input, positions the mouse to make it, and then
// makes the move
void makeMove(uint8_t move, uint8_t number) {
    // don't do anything if the move in NOMOVE
    if (move == NOMOVE) {
        return;
    }
    // Turns the mouse to face the proper direction
    switch (move) {
        case RIGHT:
            switch (mouse.direction) {
                case UP:
                    turn_right();
                    break;
                case LEFT:
                    turn_right();
                    turn_right();
                    break;
                case DOWN:
                    turn_left();
                    break;
            }
            break;
        case UP:
            switch (mouse.direction) {
                case RIGHT:
                    turn_left();
                    break;
                case LEFT:
                    turn_right();
                    break;
                case DOWN:
                    turn_right();
                    turn_right();
                    break;
            }
            break;
        case LEFT:
            switch (mouse.direction) {
                case RIGHT:
                    turn_right();
                    turn_right();
                    break;
                case UP:
                    turn_left();
                    break;
                case DOWN:
                    turn_right();
                    break;
            }
            break;
        case DOWN:
            switch (mouse.direction) {
                case RIGHT:
                    turn_right();
                    break;
                case UP:
                    turn_right();
                    turn_right();
                    break;
                case LEFT:
                    turn_left();
                    break;
            }
            break;
    }
    // goes forward
    forward(number);
}
