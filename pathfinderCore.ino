//THIS PROGRAM IS ULTRA-RECURSIVE (I KNOW THAT'S THE POINT)
//IT USES A HUGE AMOUNT OF RAM
////WAY MORE THAN ANY ARDUINO BOARD HAS
//USE AN RPI PICO
//HONESTLY THEY'RE CHEAPER THAN METRO MINIS ANYWAYS

//Use this library for neopixel control with an rpi pico
//https://github.com/MrYsLab/NeoPixelConnect

//FUTURE OPTIMIZATION
//rework the algorithm to prioritize paths with fewer turns if it means traversal time is lower

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

//This maze is only used for debugging purposes
//The mouse will never access this maze during operation
uint8_t maze[MAXMAZESIZE][MAXMAZESIZE]={
    {3, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 2},
    {2, 3, 3, 1, 1, 0, 0, 3, 1, 1, 2, 1, 2},
    {2, 0, 1, 1, 1, 1, 2, 2, 3, 2, 2, 2, 2},
    {2, 3, 1, 1, 1, 0, 1, 0, 2, 0, 0, 2, 2},
    {2, 1, 1, 1, 1, 2, 3, 1, 0, 1, 2, 2, 2},
    {3, 2, 1, 1, 1, 2, 3, 1, 1, 1, 2, 0, 2},
    {2, 1, 1, 1, 0, 2, 0, 3, 1, 1, 2, 3, 2},
    {2, 1, 1, 3, 2, 2, 1, 2, 3, 2, 2, 2, 2},
    {2, 1, 2, 2, 2, 1, 1, 0, 2, 2, 2, 0, 2},
    {3, 1, 0, 2, 2, 2, 1, 1, 0, 0, 3, 2, 2},
    {3, 2, 3, 0, 2, 2, 2, 1, 1, 3, 2, 2, 2},
    {2, 0, 1, 0, 2, 2, 1, 1, 1, 0, 2, 0, 2},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}
};

//Global variables that need to be accessed by multiple functions
//initialize distances matrix
uint8_t distances[MAXMAZESIZE][MAXMAZESIZE];

//initialize memory matrix
uint8_t memory[MAXMAZESIZE][MAXMAZESIZE];

//initialize visited matrix
uint8_t visited[MAXMAZESIZE][MAXMAZESIZE];

//initialize position variables
int8_t current_y;
int8_t current_x;
int8_t current_direction;

//Debug function for printing a matrix in a human-readable manner
void print_matrix(uint8_t matrix[MAXMAZESIZE][MAXMAZESIZE]){
    printf("Matrix:\n");
    for (uint8_t i = 0; i < MAXMAZESIZE; i++) {
        printf("{");
        for (uint8_t j = 0; j < MAXMAZESIZE ; j++) {
            if(matrix[i][j]!=MAXDISTANCE){
                printf("%d, ", matrix[i][j]);
            }
            else{
                printf(". ");
            }
        }
        printf("},");
        printf("\n");
    }
}

//Debug function for printing a move
//Moves are stored as integers 0 to 4
//In the code, #defines are used, but those don't work in the terminal
//This function converts them into readable strings
void print_move(uint8_t move){
    switch(move){
        case RIGHT:
            printf("Right");
        break;
        case UP:
            printf("Up");
        break;
        case LEFT:
            printf("Left");
        break;
        case DOWN:
            printf("Down");
        break;
        case NOMOVE:
            printf("No Move");
        break;
    }
}

//Function that returns a list[12] of the neighboring cells to an input cell
//format: y_coord,x_coord,direction,[...],y_coord,x_coord,direction
//can take a bias to change the order of the output 
//make sure to initialize a variable to receive results into first
void neighbors(int8_t y,int8_t x,uint8_t bias,uint8_t *pointer){
    switch(bias){
        case RIGHT:
            pointer[0] = y;
            pointer[1] = x+1;
            pointer[2] = RIGHT;
            pointer[3] = y+1;
            pointer[4] = x;
            pointer[5] = DOWN;
            pointer[6] = y-1;
            pointer[7] = x;
            pointer[8] = UP;
            pointer[9] = y;
            pointer[10] = x-1;
            pointer[11] = LEFT;
        break;
        case UP:
            pointer[0] = y-1;
            pointer[1] = x;
            pointer[2] = UP;
            pointer[3] = y;
            pointer[4] = x+1;
            pointer[5] = RIGHT;
            pointer[6] = y;
            pointer[7] = x-1;
            pointer[8] = LEFT;
            pointer[9] = y+1;
            pointer[10] = x;
            pointer[11] = DOWN;
        break;
        case LEFT:
            pointer[0] = y;
            pointer[1] = x-1;
            pointer[2] = LEFT;
            pointer[3] = y+1;
            pointer[4] = x;
            pointer[5] = DOWN;
            pointer[6] = y-1;
            pointer[7] = x;
            pointer[8] = UP;
            pointer[9] = y;
            pointer[10] = x+1;
            pointer[11] = RIGHT;
        break;
        case DOWN:
            pointer[0] = y+1;
            pointer[1] = x;
            pointer[2] = DOWN;
            pointer[3] = y;
            pointer[4] = x+1;
            pointer[5] = RIGHT;
            pointer[6] = y;
            pointer[7] = x-1;
            pointer[8] = LEFT;
            pointer[9] = y-1;
            pointer[10] = x;
            pointer[11] = UP;
        break;
    }
}

//The primary function that allows the mouse to work
//It takes a position and calculates the number of moves away from that position every cell in the maze is
void maze_distances(int8_t start_y,int8_t start_x){
    //initialize distances matrix to all MAXDISTANCE
    for(uint8_t i=0;i<MAXMAZESIZE;i++){
        for(uint8_t j=0;j<MAXMAZESIZE;j++){
            distances[i][j]=MAXDISTANCE;
        }
    }
    //check if start cell is in bounds
    if(!(start_y>=0 && start_y<MAXMAZESIZE-1 && start_x>=0 && start_x<MAXMAZESIZE-1)){
        printf("INVALID START CELL\n");
        return;
    }
    //initialize visited matrix to all MAXDISTANCE
    for(uint8_t i=0;i<MAXMAZESIZE;i++){
        for(uint8_t j=0;j<MAXMAZESIZE;j++){
            visited[i][j]=MAXDISTANCE;
        }
    }
    //set start cell in distances matrix to 0
    distances[start_y][start_x]=0;
    //get a list of neighboring cells to the start cell
    uint8_t neighboring_cells[12];
    neighbors(start_y,start_x,RIGHT,neighboring_cells); //bias here is completely arbitrary
    //run the recursive function on all of the neighboring cells
    for(uint8_t i=0;i<4;i++){
        recurse(neighboring_cells[0+3*i],neighboring_cells[1+3*i]);
    }
}

    //recursive function that is called for every cell
    void recurse(int8_t y,int8_t x){
        //check if cell is in bounds
        if(!(y>=0 && y<MAXMAZESIZE-1 && x>=0 && x<MAXMAZESIZE-1)){
            return;
        }
    //Determines the distance the current cell is from the target position
        uint8_t lowest_distance=distances[y][x];
        //get list of all neighboring cells
        uint8_t neighboring_cells_recurse[12];
        neighbors(y,x,RIGHT,neighboring_cells_recurse); //bias here is completely arbitrary
        //initializes some variables
    //these aren't technically necessary, but they improve code readability
        int8_t cell_y;
        int8_t cell_x;
        uint8_t cell_direction;
        //check neighbors for a lower distance
        for(uint8_t i=0;i<4;i++){
            //stores values into variables for the current cell
            cell_y=neighboring_cells_recurse[0+3*i];
            cell_x=neighboring_cells_recurse[1+3*i];
            cell_direction=neighboring_cells_recurse[2+3*i];
            //check if cell is in bounds
            if(!(cell_y>=0 && cell_y<MAXMAZESIZE-1 && cell_x>=0 && cell_x<MAXMAZESIZE-1)){
                continue;
            }
            //is there a reachable neighbor with a lower distance?
            if(lowest_distance>distances[cell_y][cell_x]){// POTENTIAL OPTIMIZATION: add "-1" to lowest_distance
                //determine if there is a wall inbetween the current cell and the neighbor
                if(cell_direction==RIGHT && memory[cell_y][cell_x]!=2 && memory[cell_y][cell_x]!=3){
                    lowest_distance=distances[cell_y][cell_x];
                    distances[y][x]=lowest_distance+1;
                }
                if(cell_direction==UP && memory[y][x]!=1 && memory[y][x]!=3){
                    lowest_distance=distances[cell_y][cell_x];
                    distances[y][x]=lowest_distance+1;
                }
                if(cell_direction==LEFT && memory[y][x]!=2 && memory[y][x]!=3){
                    lowest_distance=distances[cell_y][cell_x];
                    distances[y][x]=lowest_distance+1;
                }
                if(cell_direction==DOWN && memory[cell_y][cell_x]!=1 && memory[cell_y][cell_x]!=3){
                    lowest_distance=distances[cell_y][cell_x];
                    distances[y][x]=lowest_distance+1;
                }
            }
        }
        //Major optimization - it checks if the new distances is lower than what it previously had
        //if it wasn't, then we know that the program had already moved through this area and it already calculated all of the distances
        if(!(distances[y][x]<visited[y][x])){
            return;
        }
        //Everything below this line will execute if a lower distance was calculated
        visited[y][x]=distances[y][x];
        //Runs the recurse() function on all neighboring cells
        for(uint8_t i=0;i<4;i++){
            cell_y=neighboring_cells_recurse[0+3*i];
            cell_x=neighboring_cells_recurse[1+3*i];
            if(!(distances[cell_y][cell_x]<=lowest_distance)){
                recurse(cell_y,cell_x);
            }
        }
    }

//This function determines the best next move for the mouse to take
//It takes a target position and returns the best move to make
uint8_t next_move(int8_t target_y,int8_t target_x){
    //are we already at the target coords
    if(current_y==target_y && current_x==target_x){
        return NOMOVE;
    }
    //calculate distances
    maze_distances(target_y,target_x);
    //these aren't really necessary, but they improve code readability
    uint8_t current_distance=distances[current_y][current_x];
    int8_t cell_y;
    int8_t cell_x;
    uint8_t cell_direction;
    uint8_t neighbors_next_move[12];
    neighbors(current_y,current_x,current_direction,neighbors_next_move); //the bias here is very important, it priorotizes moves that require the least amount of rotation
    //check to find a reachable neighbor with a lower distance
    for(uint8_t i=0;i<4;i++){
        cell_y=neighbors_next_move[0+3*i];
        cell_x=neighbors_next_move[1+3*i];
        cell_direction=neighbors_next_move[2+3*i];
        //check if cell is in bounds
        if(cell_y>=0 && cell_y<MAXMAZESIZE && cell_x>=0 && cell_x<MAXMAZESIZE){
            //check if cell has a lower distance
            if(distances[cell_y][cell_x]<current_distance){
                //check if cell is reachable
                if(cell_direction==RIGHT && memory[cell_y][cell_x]!=2 && memory[cell_y][cell_x]!=3){
                    return RIGHT;
                }
                if(cell_direction==UP && memory[current_y][current_x]!=1 && memory[current_y][current_x]!=3){
                    return UP;
                }
                if(cell_direction==LEFT && memory[current_y][current_x]!=2 && memory[current_y][current_x]!=3){
                    return LEFT;
                }   
                if(cell_direction==DOWN && memory[cell_y][cell_x]!=1 && memory[cell_y][cell_x]!=3){
                    return DOWN;
                }
            }
        }
    }
    //This ideally should never be executed
    //The mouse should never be put in an unsolvable maze
    //What would the point even be?
    printf("UNSOLVABLE MAZE\n");
    return NOMOVE;
}

//NEEDS HARDWARE INTERFACE
void measure(){
    // //Debug
    // //load the maze into memory
    // for(uint8_t i=0;i<MAXMAZESIZE;i++){
    //     for(uint8_t j=0;j<MAXMAZESIZE;j++){
    //         memory[i][j]=maze[i][j];
    //     }
    // }

    //Temporary development code
    //Check what's visible to the mouse and store it into memory
    //Is there a wall above or to the left of the mouse
    memory[current_y][current_x]=maze[current_y][current_x];
    //Is there a wall below the mouse
    if(maze[current_y+1][current_x]==1 || maze[current_y+1][current_x]==3){
        //store the presence of a wall into memory
        if(memory[current_y+1][current_x]!=3){
            if(memory[current_y+1][current_x]==2){
                memory[current_y+1][current_x]=3;
            }
            else{
                memory[current_y+1][current_x]=1;
            }
        }
    }
    //Is there a wall to the right of the mouse
    if(maze[current_y][current_x+1]==2 || maze[current_y][current_x+1]==3){
        //store the presence of a wall into memory
        if(memory[current_y][current_x+1]!=3){
            if(memory[current_y][current_x+1]==1){
                memory[current_y][current_x+1]=3;
            }
            else{
                memory[current_y][current_x+1]=2;
            }
        }
    }
    //Real code goes here (eventually)
    //This will query the sensors to see if there are any walls at the mouse's current position
}

//NEEDS HARDWARE INTERFACE
//Moves the mouse forward a given number of cells
void forward(uint8_t number){
    for(uint8_t i=0;i<number;i++){
        switch(current_direction){
            case RIGHT:
            current_x++;
            break;
            case UP:
            current_y--;
            break;
            case LEFT:
            current_x--;
            break;
            case DOWN:
            current_y++;
            break;
        }
    }
    //This will actually command the motors to turn on in a certain direction for a certain amount of time
}

//NEEDS HARDWARE INTERFACE
//Turns the mouse 90 degrees right
void turn_right(){
    current_direction=current_direction-1;
    //accounts for overflow
    //I.E. if the direction was 0, turning right makes the direction -1, but we want that to stay within 0-3
    while(current_direction<0){
        current_direction=current_direction+4;
    }
    //This will actually command the motors to turn on in a certain direction for a certain amount of time 
}

//NEEDS HARDWARE INTERFACE
void turn_left(){
    current_direction=current_direction+1;
    //accounts for overflow
    //I.E. if the direction was 3, turning left makes the direction 4, but we want that to stay within 0-3
    while(current_direction>3){
        current_direction=current_direction-4;
    }
    //This will actually command the motors to turn on in a certain direction for a certain amount of time
}

//This function takes a move as input, positions the mouse to make it, and then makes the move
void make_move(uint8_t move,uint8_t number){
    //don't do anything if the move in NOMOVE
    if(move==NOMOVE){
        return;
    }
    //Turns the mouse to face the proper direction
    switch(move){
        case RIGHT:
            if(current_direction==UP){
                turn_right();
            }
            if(current_direction==LEFT){
                turn_right();
                turn_right();
            }
            if(current_direction==DOWN){
                turn_left();
            }
        break;
        case UP:
            if(current_direction==RIGHT){
                turn_left();
            }
            if(current_direction==LEFT){
                turn_right();
            }
            if(current_direction==DOWN){
                turn_right();
                turn_right();
            }
        break;
        case LEFT:
            if(current_direction==RIGHT){
                turn_right();
                turn_right();
            }
            if(current_direction==UP){
                turn_left();
            }
            if(current_direction==DOWN){
                turn_right();
            }
        break;
        case DOWN:
            if(current_direction==RIGHT){
                turn_left();
            }
            if(current_direction==UP){
                turn_right();
                turn_right();
            }
            if(current_direction==LEFT){
                turn_left();
            }
        break;
    }
    //goes forward
    forward(number);
}

//The main high-level function that gets the mouse from point A to point B
void navigate(int8_t target_y,int8_t target_x){
    //Measures it's surroundings in the beginning
    //It turns and then measures again because the physical mouse will not have a rear sensor
    //It still needs to measure all four sides in the beginning, though
    //After the start, it will never need to measure behind it
    measure();
    turn_right();
    measure();
    turn_left(); //THIS CAN BE REMOVED ONCE THE SPRINT BUG IS FIXED
    uint8_t move=0;
    //while loop continuously to calculate the best next move, makes it, and then measure its surroundings
    while(move!=NOMOVE){
        move=next_move(target_y,target_x);
        make_move(move,1);
        printf("\nMade move: \"");
        print_move(move);
        printf("\"\nCurrent coordinates are [%d,%d]\n",current_y,current_x);
        measure();
    }
    printf("Done!\n");
}

//This function gets the mouse from the start to the goal as fast as possible
//It does not stop to measure or recalculate anything
//This is used to make the actual attempt at traversing the maze in record time

//MAJOR BUG FIX REQUIRED
//right now sprint() blindly assumes unexplored areas of the maze have no walls
//this is because the flood-fill algorithm requires the aforementioned assumption to work
//however, if it attempts to spring through unexplored areas, it will hit a wall
//the code needs to be modified to not go through unexplored areas
//recreation info in "unexploredSprintBug.txt"

void sprint(int8_t start_y,int8_t start_x,int8_t objective_y,int8_t objective_x){
    //gets the mouse back to the start
    printf("Returning to start\n");
    navigate(start_y,start_x);
    //store current position and direction
    uint8_t real_y=current_y;
    uint8_t real_x=current_x;
    uint8_t real_direction=current_direction;
    //initialize the list of moves
    uint8_t moves[MAXDISTANCE];
    uint8_t move=0;
    uint8_t move_counter=0;
    printf("\nCalculating shortest path\n");
    while(move!=NOMOVE){
        //calculate the best next move
        move=next_move(objective_y,objective_x);
        //virtually execute the move
        current_direction=move;
        switch(move){
            case RIGHT:
                current_x++;
            break;
            case UP:
                current_y--;
            break;
            case LEFT:
                current_x--;
            break;
            case DOWN:
                current_y++;
            break;
        }
        //store the move
        moves[move_counter]=move;
        move_counter++;
    }
    //ADD FEATURE: Make the mouse turn to make the first move pre-emptively
    printf("Done!\n");
    uint8_t path_length=move_counter;
    //restore actual position and direction
    current_y=real_y;
    current_x=real_x;
    current_direction=real_direction;
    move_counter=1;
    //execute the stored moves as fast as possible
    printf("\nSprint!\n");
    //IMPORTANT NOTE: for loop starts on the SECOND item in the list
    for(uint8_t i=1;i<=path_length;i++){
        //checks if the current move equals the previous move
        if(moves[i]==moves[i-1]){
            //increment the move counter
            move_counter++;
            //goes to next move
            continue;
        }
        else{
            //makes the move with the number of stored same-direction moves
            make_move(moves[i-1],move_counter);
        }
        if(move_counter==1){
            //print move normally if it only happened once
            print_move(moves[i-1]);
            printf(" [%d,%d]",current_y,current_x);
            printf("\n");
        }
        else{
            //if multiple moves were made, print how many times
            print_move(moves[i-1]);
            printf(" %d times",move_counter);
            printf(" [%d,%d]\n",current_y,current_x);
        }
        //reset move counter
        move_counter=1;
    }
    printf("Done!\n");
}  

void setup(){
//initialize distances matrix to all MAXDISTANCE
for(uint8_t i=0;i<MAXMAZESIZE;i++){
    for(uint8_t j=0;j<MAXMAZESIZE;j++){
        distances[i][j]=MAXDISTANCE;
    }
}

//initialize memory to all 0
for(uint8_t i=0;i<MAXMAZESIZE;i++){
    for(uint8_t j=0;j<MAXMAZESIZE;j++){
        memory[i][j]=0;
    }
}

//set the current position and direction to the starting values
current_y=START_Y;
current_x=START_X;
current_direction=START_DIRECTION;

//high-level behavior
//currently it will behave like a Micromouse would in the competition
navigate(GOAL_Y,GOAL_X);
sprint(START_Y,START_X,GOAL_Y,GOAL_X);
}

void loop(){
    delay(1);
}