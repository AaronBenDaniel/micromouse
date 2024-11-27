// NEEDS HARDWARE INTERFACE
void measure() {
    // Check what's visible to the mouse and store it into memory
    // Is there a wall above or to the left of the mouse
    memory.matrix[mouse.pos.y][mouse.pos.x] =
        maze.matrix[mouse.pos.y][mouse.pos.x];
    // Is there a wall below the mouse
    if (maze.matrix[mouse.pos.y + 1][mouse.pos.x] == 1 ||
        maze.matrix[mouse.pos.y + 1][mouse.pos.x] == 3) {
        // store the presence of a wall into memory
        if (memory.matrix[mouse.pos.y + 1][mouse.pos.x] != 3) {
            if (memory.matrix[mouse.pos.y + 1][mouse.pos.x] == 2) {
                memory.matrix[mouse.pos.y + 1][mouse.pos.x] = 3;
            } else {
                memory.matrix[mouse.pos.y + 1][mouse.pos.x] = 1;
            }
        }
    }
    // Is there a wall to the right of the mouse
    if (maze.matrix[mouse.pos.y][mouse.pos.x + 1] == 2 ||
        maze.matrix[mouse.pos.y][mouse.pos.x + 1] == 3) {
        // store the presence of a wall into memory
        if (memory.matrix[mouse.pos.y][mouse.pos.x + 1] != 3) {
            if (memory.matrix[mouse.pos.y][mouse.pos.x + 1] == 1) {
                memory.matrix[mouse.pos.y][mouse.pos.x + 1] = 3;
            } else {
                memory.matrix[mouse.pos.y][mouse.pos.x + 1] = 2;
            }
        }
    }
    // Real code goes here (eventually)
    // This will query the sensors to see if there are any walls at the mouse's
    // current position
}