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

void mainLoop() {
  motorR.PWMRun();
  motorL.PWMRun();

  const float proportionalGain = 6;

  float angle = getAngle();
  int16_t speedHelper;

  if (target + tolerance < angle) {
    speedHelper = map((angle - target), 0, 360, 0, 255) * proportionalGain * -1;
    if (speedHelper < -255) speedHelper = -255;
    motorR.speed = speedHelper;
    speedHelper = map((angle - target), 0, 360, 0, 255) * proportionalGain;
    if (speedHelper > 255) speedHelper = 255;
    motorL.speed = speedHelper;
  } else if (target - tolerance > angle) {
    speedHelper = map((angle - target), 0, 360, 0, 255) * proportionalGain;
    if (speedHelper < -255) speedHelper = -255;
    motorR.speed = speedHelper;
    speedHelper = map((angle - target), 0, 360, 0, 255) * proportionalGain * -1;
    if (speedHelper > 255) speedHelper = 255;
    motorL.speed = speedHelper;
  } else {
    motorR.speed = 0;
    motorL.speed = 0;
  }

  // if (millis() - lastUpdate > 2000) {
  //   if (target == 90) target = 180;
  //   else target = 90;
  //   lastUpdate = millis();
  // }

  target = 180;

  Serial.println(angle);
  Serial.println(target);

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