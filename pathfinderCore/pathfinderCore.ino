//Edit these defines to alter the behavior of the mouse
#define START_Y 0
#define START_X 0
#define START_DIRECTION RIGHT
#define GOAL_Y 10
#define GOAL_X 10

//DO NOT EDIT ANY CODE BELOW THIS LINE

//Defines macros to use for better code readability
//also for easily changing the parameters of the maze
#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3
#define NOMOVE 4
#define MAXMAZESIZE 13
#define MAXDISTANCE 255

// .......look this is either genius or stupid
uint8_t bias[4][4] = {
  { RIGHT, DOWN, UP, LEFT },
  { UP, LEFT, RIGHT, DOWN },
  { LEFT, UP, DOWN, RIGHT },
  { DOWN, RIGHT, LEFT, UP }
};

//initialize structs

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
  uint8_t matrix[MAXMAZESIZE][MAXMAZESIZE];
};

//This maze is only used for development purposes
//The mouse will never access this maze during operation
struct matrix_t maze;
void initVirtualMaze() {  //Call this function before anything else to load the hardcoded debug maze into the maze struct
  uint8_t mazeArray[MAXMAZESIZE][MAXMAZESIZE] = {
    { 3, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 2 },
    { 2, 3, 3, 1, 1, 0, 0, 3, 1, 1, 2, 1, 2 },
    { 2, 0, 1, 1, 1, 1, 2, 2, 3, 2, 2, 2, 2 },
    { 2, 3, 1, 1, 1, 0, 1, 0, 2, 0, 0, 2, 2 },
    { 2, 1, 1, 1, 1, 2, 3, 1, 0, 1, 2, 2, 2 },
    { 3, 2, 1, 1, 1, 2, 3, 1, 1, 1, 2, 0, 2 },
    { 2, 1, 1, 1, 0, 2, 0, 3, 1, 1, 2, 3, 2 },
    { 2, 1, 1, 3, 2, 2, 1, 2, 3, 2, 2, 2, 2 },
    { 2, 1, 2, 2, 2, 1, 1, 0, 2, 2, 2, 0, 2 },
    { 3, 1, 0, 2, 2, 2, 1, 1, 0, 0, 3, 2, 2 },
    { 3, 2, 3, 0, 2, 2, 2, 1, 1, 3, 2, 2, 2 },
    { 2, 0, 1, 0, 2, 2, 1, 1, 1, 0, 2, 0, 2 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 }
  };
  for (uint8_t y = 0; y < MAXMAZESIZE; y++) {
    for (uint8_t x = 0; x < MAXMAZESIZE; x++) {
      maze.matrix[y][x] = mazeArray[y][x];
    }
  }
}

//Global structs that need to be accessed by multiple functions
//initialize memory matrix
struct matrix_t memory;

struct mouse_t mouse;

struct xyPair_t startPos;

struct xyPair_t goalPos;

//
// GENERAL FUCNTIONS FOR READABILITY AND CODE DEDUPLICATION
//

//Development function for printing a matrix in a human-readable manner
void printMatrix(struct matrix_t matrix) {
  Serial.println("Matrix:");
  for (uint8_t y = 0; y < MAXMAZESIZE; y++) {
    Serial.print("{");
    for (uint8_t x = 0; x < MAXMAZESIZE; x++) {
      if (matrix.matrix[y][x] != MAXDISTANCE) {
        Serial.print(matrix.matrix[y][x]);
        Serial.print(", ");
      } else {
        Serial.print(". ");
      }
    }
    Serial.println("},");
  }
}

//Development function for printing a move
//Moves are stored as integers 0 to 4
//In the code, #defines are used, but those don't work in the terminal
//This function converts them into readable strings
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
    case NOMOVE:
      Serial.print("No Move");
      break;
  }
}

//This function returns a struct of type neighbors_t that contains four
//structs of type xyPair_t that contain coordiante pairs of the neighboring cells
//to the input cell.
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

//
// HARDWARE INTERFACE FUNCTIONS
//

//NEEDS HARDWARE INTERFACE
//Moves the mouse forward a given number of cells
void forward(uint8_t number) {
  for (uint8_t i = 0; i < number; i++) {
    //Update the mouse's virtual position
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
  //This will actually command the motors to turn on for a certain amount of time
}

//NEEDS HARDWARE INTERFACE
//Turns the mouse 90 degrees right
void turn_right() {
  mouse.direction = mouse.direction - 1;
  //accounts for overflow
  //I.E. if the direction was 0, turning right makes the direction -1, but we want that to stay within 0-3
  while (mouse.direction < 0) {
    mouse.direction = mouse.direction + 4;
  }
  //This will actually command the motors to turn on in a certain direction for a certain amount of time
}

//NEEDS HARDWARE INTERFACE
void turn_left() {
  mouse.direction = mouse.direction + 1;
  //accounts for overflow
  //I.E. if the direction was 3, turning left makes the direction 4, but we want that to stay within 0-3
  while (mouse.direction > 3) {
    mouse.direction = mouse.direction - 4;
  }
  //This will actually command the motors to turn on in a certain direction for a certain amount of time
}

//This function takes a move as input, positions the mouse to make it, and then makes the move
void makeMove(uint8_t move, uint8_t number) {
  //don't do anything if the move in NOMOVE
  if (move == NOMOVE) {
    return;
  }
  //Turns the mouse to face the proper direction
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
  //goes forward
  forward(number);
}

//NEEDS HARDWARE INTERFACE
void measure() {
  //Temporary development code
  //Check what's visible to the mouse and store it into memory
  //Is there a wall above or to the left of the mouse
  memory.matrix[mouse.pos.y][mouse.pos.x] = maze.matrix[mouse.pos.y][mouse.pos.x];
  //Is there a wall below the mouse
  if (maze.matrix[mouse.pos.y + 1][mouse.pos.x] == 1 || maze.matrix[mouse.pos.y + 1][mouse.pos.x] == 3) {
    //store the presence of a wall into memory
    if (memory.matrix[mouse.pos.y + 1][mouse.pos.x] != 3) {
      if (memory.matrix[mouse.pos.y + 1][mouse.pos.x] == 2) {
        memory.matrix[mouse.pos.y + 1][mouse.pos.x] = 3;
      } else {
        memory.matrix[mouse.pos.y + 1][mouse.pos.x] = 1;
      }
    }
  }
  //Is there a wall to the right of the mouse
  if (maze.matrix[mouse.pos.y][mouse.pos.x + 1] == 2 || maze.matrix[mouse.pos.y][mouse.pos.x + 1] == 3) {
    //store the presence of a wall into memory
    if (memory.matrix[mouse.pos.y][mouse.pos.x + 1] != 3) {
      if (memory.matrix[mouse.pos.y][mouse.pos.x + 1] == 1) {
        memory.matrix[mouse.pos.y][mouse.pos.x + 1] = 3;
      } else {
        memory.matrix[mouse.pos.y][mouse.pos.x + 1] = 2;
      }
    }
  }
  //Real code goes here (eventually)
  //This will query the sensors to see if there are any walls at the mouse's current position
}

//
// FUNCTIONS THAT RELATE TO PATHFINDING AND NAVIGATION
//

struct matrix_t calcDistances(struct xyPair_t rootCell) {
  struct matrix_t distances;  //matrix that contains the distance each cell is from the root

  //initialize distances matrix to all MAXDISTANCE
  for (uint8_t y = 0; y < MAXMAZESIZE; y++) {
    for (uint8_t x = 0; x < MAXMAZESIZE; x++) {
      distances.matrix[y][x] = MAXDISTANCE;
    }
  }

  //check if root cell is in bounds
  if (rootCell.y < 0 || rootCell.y > MAXMAZESIZE - 1 || rootCell.x < 0 || rootCell.x > MAXMAZESIZE - 1) {
    Serial.println("INVALID ROOT CELL");
    return;
  }

  //set root cell in distances matrix to 0
  distances.matrix[rootCell.y][rootCell.x] = 0;

  struct xyPair_t test;
  test.x = rootCell.x;
  test.y = rootCell.y + 1;
  distances = updateCellDistance(distances, test);
  test.x = rootCell.x - 1;
  test.y = rootCell.y + 1;
  distances = updateCellDistance(distances, test);
  test.x = rootCell.x - 1;
  test.y = rootCell.y;
  distances = updateCellDistance(distances, test);
  test.x = rootCell.x - 1;
  test.y = rootCell.y - 1;
  distances = updateCellDistance(distances, test);
  return (distances);
}

struct matrix_t updateCellDistance(struct matrix_t distances, struct xyPair_t cell) {
  //check if cell is in bounds
  if (cell.y < 0 || cell.y > MAXMAZESIZE - 1 || cell.x < 0 || cell.x > MAXMAZESIZE - 1) {
    return;
  }

  //determine the distance the current cell is from the root cell
  uint8_t lowestDistance = distances.matrix[cell.y][cell.x];

  //get neighboring cells to cell
  struct neighbors_t cellNeighbors;
  cellNeighbors = neighbors(cell);

  //check neighbors for a lower distance
  for (uint8_t i = 0; i < 4; i++) {
    //store values into variables for the current cell
    struct xyPair_t currentCell;
    uint8_t cellDirection = i;
    currentCell.x = cellNeighbors.direction[i].x;
    currentCell.y = cellNeighbors.direction[i].y;

    //check if current cell is in bounds
    if (currentCell.y < 0 || currentCell.y > MAXMAZESIZE - 1 || currentCell.x < 0 || currentCell.x > MAXMAZESIZE - 1) {
      continue;
    }

    //is there a reachable neighbor with a lower distance?
    if (lowestDistance > distances.matrix[currentCell.y][currentCell.x] - 1) {  //does the current cell has an appreciably lower distance?

      //determine if there is a wall in between cell and currentCell

      if (cellDirection == RIGHT && memory.matrix[currentCell.y][currentCell.x] != 2 && memory.matrix[currentCell.y][currentCell.x] != 3) {
        lowestDistance = distances.matrix[currentCell.y][currentCell.x];
      }

      if (cellDirection == UP && memory.matrix[cell.y][cell.x] != 1 && memory.matrix[cell.y][cell.x] != 3) {
        lowestDistance = distances.matrix[currentCell.y][currentCell.x];
      }

      if (cellDirection == LEFT && memory.matrix[cell.y][cell.x] != 2 && memory.matrix[cell.y][cell.x] != 3) {
        lowestDistance = distances.matrix[currentCell.y][currentCell.x];
      }

      if (cellDirection == DOWN && memory.matrix[cell.y][currentCell.x] != 1 && memory.matrix[currentCell.y][currentCell.x] != 3) {
        lowestDistance = distances.matrix[currentCell.y][currentCell.x];
      }
    }
  }
  if (lowestDistance < distances.matrix[cell.y][cell.x]) {  //did a lower distance get found?
    distances.matrix[cell.y][cell.x] = lowestDistance + 1;  //set cell's distance to the lowestDistance + 1
  }
  return (distances);
}

//This function determines the best next move for the mouse to take
//It takes a target position and returns the best move to make
uint8_t nextMove(struct xyPair_t targetCell) {
  //is the mouse already at the target coords?
  if (mouse.pos.y == targetCell.y && mouse.pos.x == targetCell.x) {
    return NOMOVE;
  }

  //calculate distances
  struct matrix_t distances;
  distances = calcDistances(targetCell);

  struct xyPair_t cell;
  uint8_t cellDirection;

  struct neighbors_t cellNeighbors;
  cellNeighbors = neighbors(mouse.pos);

  //check to find a reachable neighbor with a lower distance
  for (uint8_t i = 0; i < 4; i++) {
    cell.y = cellNeighbors.direction[bias[mouse.direction][i]].y;
    cell.x = cellNeighbors.direction[bias[mouse.direction][i]].x;
    cellDirection = bias[mouse.direction][i];

    //check if cell is in bounds
    if (cell.y < 0 || cell.y > MAXMAZESIZE - 1 || cell.x < 0 && cell.x > MAXMAZESIZE - 1) {
      continue;
    }

    //check if cell has a lower distance
    if (distances.matrix[cell.y][cell.x] >= distances.matrix[mouse.pos.y][mouse.pos.x]) {
      continue;
    }

    //check if cell is reachable
    if (cellDirection == RIGHT && memory.matrix[cell.y][cell.x] != 2 && memory.matrix[cell.y][cell.x] != 3) {
      return RIGHT;
    }
    if (cellDirection == UP && memory.matrix[mouse.pos.y][mouse.pos.x] != 1 && memory.matrix[mouse.pos.y][mouse.pos.x] != 3) {
      return UP;
    }
    if (cellDirection == LEFT && memory.matrix[mouse.pos.y][mouse.pos.x] != 2 && memory.matrix[mouse.pos.y][mouse.pos.x] != 3) {
      return LEFT;
    }
    if (cellDirection == DOWN && memory.matrix[cell.y][cell.x] != 1 && memory.matrix[cell.y][cell.x] != 3) {
      return DOWN;
    }
  }

  //This ideally should never be executed
  //The mouse should never be put in an unsolvable maze
  //What would the point even be?
  Serial.println("UNSOLVABLE MAZE");
  return NOMOVE;
}

void setup() {
  Serial.begin(19200);
  initVirtualMaze();  // FOR DEVELOPMENT PURPOSES ONLY

  //initialize memory to all 0
  for (uint8_t y = 0; y < MAXMAZESIZE; y++) {
    for (uint8_t x = 0; x < MAXMAZESIZE; x++) {
      memory.matrix[y][x] = 0;
    }
  }

  // FOR TESTING PURPOSES ONLY
  for (uint8_t y = 0; y < MAXMAZESIZE; y++) {
    for (uint8_t x = 0; x < MAXMAZESIZE; x++) {
      memory.matrix[y][x] = maze.matrix[y][x];
    }
  }

  // load start and goal coordinates
  startPos.y = START_Y;
  startPos.x = START_X;
  goalPos.y = GOAL_Y;
  goalPos.x = GOAL_X;
  mouse.direction = START_DIRECTION;
  mouse.pos.y = startPos.y;
  mouse.pos.x = startPos.x;
}

void loop() {
  // ALL OF THIS IS FOR TESTING ONLY
  struct matrix_t distances;
  struct xyPair_t pair;
  pair.x = 1;
  pair.y = 1;
  distances = calcDistances(pair);
  printMatrix(distances);
  uint8_t move = 0;
  while (move != NOMOVE) {
    move = nextMove(pair);
    printMove(move);
    Serial.println("");
    makeMove(move, 1);
  }
  while (1) {
    delay(1);
  }
}
