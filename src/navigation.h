struct matrix_t updateCellDistance(struct matrix_t distances,
                                   struct xyPair_t cell) {
    // check if cell is in bounds
    if (cell.y < 0 || cell.y > MAX_MAZE_SIZE - 2 || cell.x < 0 ||
        cell.x > MAX_MAZE_SIZE - 2) {
        return (distances);
    }

    // determine the distance the current cell is from the root cell
    uint8_t lowestDistance = distances.matrix[cell.y][cell.x];

    // get neighboring cells to cell
    struct neighbors_t cellNeighbors;
    cellNeighbors = neighbors(cell);

    // check neighbors for a lower distance
    for (uint8_t i = 0; i < 4; i++) {
        // store values into variables for the current cell
        struct xyPair_t currentCell;
        uint8_t cellDirection = i;
        currentCell.x = cellNeighbors.direction[i].x;
        currentCell.y = cellNeighbors.direction[i].y;

        // check if current cell is in bounds
        if (currentCell.y < 0 || currentCell.y > MAX_MAZE_SIZE - 1 ||
            currentCell.x < 0 || currentCell.x > MAX_MAZE_SIZE - 1) {
            continue;
        }

        // is there a reachable neighbor with a lower distance?
        if (lowestDistance > distances.matrix[currentCell.y][currentCell.x] -
                                 1) {  // does the current cell has an
                                       // appreciably lower distance?

            // determine if there is a wall in between cell and currentCell

            if (cellDirection == RIGHT &&
                memory.matrix[currentCell.y][currentCell.x] != 2 &&
                memory.matrix[currentCell.y][currentCell.x] != 3) {
                lowestDistance = distances.matrix[currentCell.y][currentCell.x];
            }

            if (cellDirection == UP && memory.matrix[cell.y][cell.x] != 1 &&
                memory.matrix[cell.y][cell.x] != 3) {
                lowestDistance = distances.matrix[currentCell.y][currentCell.x];
            }

            if (cellDirection == LEFT && memory.matrix[cell.y][cell.x] != 2 &&
                memory.matrix[cell.y][cell.x] != 3) {
                lowestDistance = distances.matrix[currentCell.y][currentCell.x];
            }

            if (cellDirection == DOWN &&
                memory.matrix[currentCell.y][currentCell.x] != 1 &&
                memory.matrix[currentCell.y][currentCell.x] != 3) {
                lowestDistance = distances.matrix[currentCell.y][currentCell.x];
            }
        }
    }
    if (lowestDistance <
        distances.matrix[cell.y][cell.x]) {  // did a lower distance get found?
        distances.matrix[cell.y][cell.x] =
            lowestDistance +
            1;  // set cell's distance to the lowestDistance + 1
    }
    return (distances);
}

struct matrix_t calcDistances(struct xyPair_t rootCell) {
    struct matrix_t distances;  // matrix that contains the distance each cell
                                // is from the root

    // initialize distances matrix to all MAX_DISTANCE
    for (uint8_t y = 0; y < MAX_MAZE_SIZE; y++) {
        for (uint8_t x = 0; x < MAX_MAZE_SIZE; x++) {
            distances.matrix[y][x] = MAX_DISTANCE;
        }
    }

    // check if root cell is in bounds
    if (rootCell.y < 0 || rootCell.y > MAX_MAZE_SIZE - 2 || rootCell.x < 0 ||
        rootCell.x > MAX_MAZE_SIZE - 2) {
        Serial.println("INVALID ROOT CELL");
        return (distances);
    }

    // set root cell in distances matrix to 0
    distances.matrix[rootCell.y][rootCell.x] = 0;

    // initialize queue
    cppQueue queue(sizeof(xyPair_t), 256, FIFO, false);

    // push neighbors of root cell to queue
    struct neighbors_t cellNeighbors;
    cellNeighbors = neighbors(rootCell);
    for (uint8_t i = 0; i < 4; i++) {
        queue.push(&cellNeighbors.direction[i]);
    }

    while (!queue.isEmpty()) {
        struct xyPair_t cell;
        queue.pop(&cell);

        uint8_t beforeDistance = distances.matrix[cell.y][cell.x];
        distances = updateCellDistance(distances, cell);

        if (distances.matrix[cell.y][cell.x] >= beforeDistance) {
            continue;
        }

        cellNeighbors = neighbors(cell);
        for (uint8_t i = 0; i < 4; i++) {
            queue.push(&cellNeighbors.direction[i]);
        }
    }

    queue.flush();
    return (distances);
}

// This function determines the best next move for the mouse to take
// It takes a target position and returns the best move to make
uint8_t nextMove(struct xyPair_t targetCell) {
    // is the mouse already at the target coords?
    if (mouse.pos.y == targetCell.y && mouse.pos.x == targetCell.x) {
        return NO_MOVE;
    }

    // calculate distances
    struct matrix_t distances;
    distances = calcDistances(targetCell);

    struct xyPair_t cell;
    uint8_t cellDirection;

    struct neighbors_t cellNeighbors;
    cellNeighbors = neighbors(mouse.pos);

    // check to find a reachable neighbor with a lower distance
    for (uint8_t i = 0; i < 4; i++) {
        cell.y = cellNeighbors.direction[bias[mouse.direction][i]].y;
        cell.x = cellNeighbors.direction[bias[mouse.direction][i]].x;
        cellDirection = bias[mouse.direction][i];

        // check if cell is in bounds
        if (cell.y < 0 || cell.y > MAX_MAZE_SIZE - 2 || cell.x < 0 ||
            cell.x > MAX_MAZE_SIZE - 2) {
            continue;
        }

        // check if cell has a lower distance
        if (distances.matrix[cell.y][cell.x] >=
            distances.matrix[mouse.pos.y][mouse.pos.x]) {
            continue;
        }

        // check if cell is reachable
        if (cellDirection == RIGHT && memory.matrix[cell.y][cell.x] != 2 &&
            memory.matrix[cell.y][cell.x] != 3) {
            return RIGHT;
        }
        if (cellDirection == UP &&
            memory.matrix[mouse.pos.y][mouse.pos.x] != 1 &&
            memory.matrix[mouse.pos.y][mouse.pos.x] != 3) {
            return UP;
        }
        if (cellDirection == LEFT &&
            memory.matrix[mouse.pos.y][mouse.pos.x] != 2 &&
            memory.matrix[mouse.pos.y][mouse.pos.x] != 3) {
            return LEFT;
        }
        if (cellDirection == DOWN && memory.matrix[cell.y][cell.x] != 1 &&
            memory.matrix[cell.y][cell.x] != 3) {
            return DOWN;
        }
    }

    // This ideally should never be executed
    // The mouse should never be put in an unsolvable maze
    // What would the point even be?
    Serial.println("UNSOLVABLE MAZE");
    failure(3);
    return NO_MOVE;
}

void navigate(struct xyPair_t destination) {
    // Measures it's surroundings in the beginning
    // It turns and then measures again because the physical mouse will not have
    // a rear sensor It still needs to measure all four sides in the beginning,
    // though After the start, it will never need to measure behind it
    setColor(YELLOW);
    measure();
    turn(TURN_RIGHT);
    measure();
    turn(TURN_LEFT);  // THIS CAN BE REMOVED ONCE THE SPRINT BUG IS FIXED
    uint8_t move = 0;
    // while loop continuously to calculate the best next move, makes it, and
    // then measure its surroundings
    while (move != NO_MOVE) {
        move = nextMove(destination);
        makeMove(move, 1);
        Serial.print("\nMade move: \"");
        printMove(move);
        Serial.print("\"\nCurrent coordinates are [");
        Serial.print(mouse.pos.y);
        Serial.print(",");
        Serial.print(mouse.pos.x);
        Serial.print("]");
        Serial.print("\n");
        measure();
    }
    Serial.println("Done!");
    setColor(GREEN);
}

void sprint(struct xyPair_t goal) {
    setColor(YELLOW);
    // store current position and direction
    struct xyPair_t realPos;
    realPos = mouse.pos;
    uint8_t realDirection = mouse.direction;
    // initialize the list of moves
    uint8_t moves[MAX_DISTANCE];
    uint8_t move = 0;
    uint8_t moveCounter = 0;
    Serial.println("\nCalculating shortest path");
    while (move != NO_MOVE) {
        // calculate the best next move
        move = nextMove(goal);
        // virtually execute the move
        mouse.direction = move;
        switch (move) {
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
        // store the move
        moves[moveCounter] = move;
        moveCounter++;
    }
    // ADD FEATURE: Make the mouse turn to make the first move pre-emptively
    Serial.println("Done!");
    uint8_t path_length = moveCounter;
    // restore actual position and direction
    mouse.pos = realPos;
    mouse.direction = realDirection;
    moveCounter = 1;
    // execute the stored moves as fast as possible
    Serial.println("\nSprint!");
    // IMPORTANT NOTE: for loop starts on the SECOND item in the list
    for (uint8_t i = 1; i <= path_length; i++) {
        // checks if the current move equals the previous move
        if (moves[i] == moves[i - 1]) {
            // increment the move counter
            moveCounter++;
            // goes to next move
            continue;
        } else {
            // makes the move with the number of stored same-direction moves
            makeMove(moves[i - 1], moveCounter);
        }
        if (moveCounter == 1) {
            // print move normally if it only happened once
            printMove(moves[i - 1]);
            Serial.print(" [");
            Serial.print(mouse.pos.y);
            Serial.print(",");
            Serial.print(mouse.pos.x);
            Serial.println("]");
        } else {
            // if multiple moves were made, print how many times
            printMove(moves[i - 1]);
            Serial.print(" ");
            Serial.print(moveCounter);
            Serial.print(" times");
            Serial.print(" [");
            Serial.print(mouse.pos.y);
            Serial.print(",");
            Serial.print(mouse.pos.x);
            Serial.println("]");
        }
        // reset move counter
        moveCounter = 1;
    }
    Serial.println("Done!");
}
