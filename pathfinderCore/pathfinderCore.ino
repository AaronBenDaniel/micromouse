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

//This maze is only used for development purposes
//The mouse will never access this maze during operation
uint8_t maze[MAXMAZESIZE][MAXMAZESIZE] = {
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

//Global variables that need to be accessed by multiple functions
//initialize distances matrix
uint8_t distances[MAXMAZESIZE][MAXMAZESIZE];

//initialize memory matrix
uint8_t memory[MAXMAZESIZE][MAXMAZESIZE];

//initialize position struct
struct mouse_t{
  int8_t y;
  int8_t x;
  int8_t current_direction;
};

struct xyPair_t{
  uint8_t x;
  uint8_t y;
};

struct neighbors_t{
struct xy_pair_t right;
struct xy_pair_t down;
struct xy_pair_t up;
struct xy_pair_t left;
};

struct mouse_t mouse;

//Development function for printing a matrix in a human-readable manner
void print_matrix(uint8_t matrix[MAXMAZESIZE][MAXMAZESIZE]) {
  Serial.println("Matrix:");
  for (uint8_t i = 0; i < MAXMAZESIZE; i++) {
    Serial.print("{");
    for (uint8_t j = 0; j < MAXMAZESIZE; j++) {
      if (matrix[i][j] != MAXDISTANCE) {
        Serial.print(matrix[i][j]);
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
void print_move(uint8_t move) {
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

struct neighbors_t neighbors(struct mouse_t mouse){

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  struct neighbors_t test;
  test=neighbor(mouse)
  test.right.x
}

void loop() {
  // put your main code here, to run repeatedly:
  print_matrix(maze);
  Serial.println("");
  print_move(LEFT);
  while (1) {
    delay(1);
  }
}
