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
  delay(200); //TESTING ONLY PLEASE REMOVE THIS LINE
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
