//Edit these defines to alter the behavior of the mouse
#define START_Y 11
#define START_X 0
#define START_DIRECTION RIGHT
#define GOAL_Y 9
#define GOAL_X 10

void mainSetup() {
  pixelInit();
  setColor(YELLOW);
  IMUInit();
  pinMode(0, INPUT_PULLUP);

  Serial.begin(19200);
  initVirtualMaze();  // FOR DEVELOPMENT PURPOSES ONLY

  //initialize memory to all 0
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
  delay(3000);  //FOR DEVELOPMENT PURPOSES ONLY REMOVE EVENTUALLY
  setColor(GREEN);
}

long lastEncoder = 0;
long lastUpdate = 0;
float target = 0;
int16_t integralSum = 0;
uint8_t proportionalGain = 12;
uint16_t integralGain = 1;
cppQueue queue(sizeof(int16_t), 1000, FIFO, false);

void mainLoop() {
  motorR.PWMRun();
  motorL.PWMRun();

  if (Serial.available() > 0)
    integralGain = getInput();

  uint16_t angle = getAngle();

  int16_t queueVar = angle - target;

  queue.push(&queueVar);
  if (queue.isFull()) queue.pop(&queueVar);

  integralSum = integralSum - queueVar + (angle - target);

  int32_t speed = (map((angle - target), 0, 360, 0, 255) * proportionalGain) + (integralSum * integralGain / 1000);

  Serial.print("Integral Sum:");
  Serial.print(integralSum);
  Serial.print(" Integral Gain: ");
  Serial.print(integralGain);
  Serial.print(" Integral Contribution: ");
  Serial.print(integralSum * integralGain / 1000);

  Serial.print(" Angle: ");
  Serial.print(angle);
  Serial.print(" Direction: ");

  if (angle > target) {
    motorR.speed = -1 * speed;
    motorL.speed = speed;

    Serial.print(" Right");
  } else if (angle < target) {
    motorR.speed = -1 * speed;
    motorL.speed = speed;

    Serial.print(" Left");
  } else {
    motorR.speed = 0;
    motorL.speed = 0;

    Serial.print(" Stop");
  }

  // if (millis() - lastUpdate > 5000) {
  //   target += 90;
  //   if (target >= 360) target = 90;
  //   lastUpdate = millis();
  // }

  target = 180;

  Serial.print("\n");
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