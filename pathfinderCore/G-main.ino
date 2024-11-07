void mainSetup() {
  pixelInit();
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
  delay(3000);  //FOR DEVELOPMENT PURPOSES ONLY REMOVE EVENTUALLY
}

void mainLoop() {
  // ALL OF THIS IS FOR TESTING ONLY
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

  setColor(GREEN);

  while (1) {
    delay(1);
  }
}