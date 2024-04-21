#include <stdio.h>
#include <stdlib.h>

//dynamic initialization of matrix \/
//
// //initialize visited matrix to all MAXDISTANCE
// // Allocate memory for the rows
// int **visited;
// visited = (int **)calloc(MAXMAZESIZE, sizeof(int *));
// // Allocate memory for each x position in the row
// for (int i = 0; i < MAXMAZESIZE; i++) {
//     visited[i] = (int *)calloc(MAXMAZESIZE, sizeof(int));
// }
//
//CODE HERE
//
// // Free the allocated memory for each row
// for (int i = 0; i < MAXMAZESIZE; i++) {
//     free(visited[i]);
// }
// // Free the allocated memory for the rows
// free(visited);

#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3
#define NOMOVE -1
#define MAXMAZESIZE 11
#define MAXDISTANCE 1000

//initialize distances matrix
int distances[MAXMAZESIZE][MAXMAZESIZE];

//initialize visited matrix
int visited[MAXMAZESIZE][MAXMAZESIZE];

//initialize memory matrix
int memory[MAXMAZESIZE][MAXMAZESIZE];

int current_y;
int current_x;
int current_direction;

void print_matrix(int matrix[MAXMAZESIZE][MAXMAZESIZE]){
    printf("Matrix:\n");
    for (int i = 0; i < MAXMAZESIZE; i++) {
        for (int j = 0; j < MAXMAZESIZE ; j++) {
            if(matrix[i][j]!=MAXDISTANCE){
                printf("%d ", matrix[i][j]);
            }
            else{
                printf(". ");
            }
        }
        printf("\n");
    }
}

void print_move(int move){
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

//make sure to initialize a variable to recieve results into first
void neighbors(int y,int x,int bias,int *pointer){
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
            pointer[10] = x-1;
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

void maze_distances(int start_y,int start_x){
    void recurse(int y,int x){
        //check if cell is in bounds
        if(!(y>=0 && y<MAXMAZESIZE && x>=0 && x<MAXMAZESIZE)){
            return;
        }
        //determine distance
        int lowest_distance=distances[y][x];
        int neighboring_cells_recurse[12];
        neighbors(y,x,RIGHT,neighboring_cells_recurse); //bias here is completely arbitrary
        int cell_y;
        int cell_x;
        int cell_direction;
        //check neighbors for a lower distance
        for(int i=0;i<4;i++){
            cell_y=neighboring_cells_recurse[0+3*i];
            cell_x=neighboring_cells_recurse[1+3*i];
            cell_direction=neighboring_cells_recurse[2+3*i];
            //check if cell is in bounds
            if(!(cell_y>=0 && cell_y<MAXMAZESIZE && cell_x>=0 && cell_x<MAXMAZESIZE)){
                continue;
            }
            if(lowest_distance>distances[cell_y][cell_x]){ //is there a neighbor with a lower distance? POTENTIAL OPTIMIZATON: add "-1" to lowest_distance
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
        if(!(distances[y][x]<visited[y][x])){
            return;
        }
        visited[y][x]=distances[y][x];
        //recurse
        for(int i=0;i<4;i++){
            cell_y=neighboring_cells_recurse[0+3*i];
            cell_x=neighboring_cells_recurse[1+3*i];
            if(!(distances[cell_y][cell_x]<=lowest_distance)){
                recurse(cell_y,cell_x);
            }
        }
    }
    //check if start cell is in bounds
    if(!(start_y>=0 && start_y<MAXMAZESIZE && start_x>=0 && start_x<MAXMAZESIZE)){
        printf("INVALID START CELL\n");
    }
    //initialize distances matrix to all MAXDISTANCE
    for(int i=0;i<MAXMAZESIZE;i++){
        for(int j=0;j<MAXMAZESIZE;j++){
            distances[i][j]=MAXDISTANCE;
        }
    }
    //initialize visted matrix to all MAXDISTANCE
    for(int i=0;i<MAXMAZESIZE;i++){
        for(int j=0;j<MAXMAZESIZE;j++){
            visited[i][j]=MAXDISTANCE;
        }
    }
    distances[start_y][start_x]=0;
    int neighboring_cells[12];
    neighbors(start_y,start_x,RIGHT,neighboring_cells); //bias here is completely arbitrary
    for(int i=0;i<4;i++){
        recurse(neighboring_cells[0+3*i],neighboring_cells[1+3*i]);
    }
}

int next_move(int target_y,int target_x){
    //are we already at the target coords
    if(current_y==target_y && current_x==target_x){
        return NOMOVE;
    }
    //calculate distances
    maze_distances(target_y,target_x);
    int current_distance=distances[current_y][current_x];
    int neighbors_next_move[12];
    int cell_y;
    int cell_x;
    int cell_direction;
    neighbors(current_y,current_x,current_direction,neighbors_next_move);
    //check to find lower reachable distance
    for(int i=0;i<4;i++){
        cell_y=neighbors_next_move[0+3*i];
        cell_x=neighbors_next_move[1+3*i];
        cell_direction=neighbors_next_move[2+3*i];
        //check if cell is in bounds
        if(!(cell_y>=0 && cell_y<MAXMAZESIZE && cell_x>=0 && cell_x<MAXMAZESIZE)){
            continue;
        }
        //check if cell has a lower distance
        if(!(distances[cell_y][cell_x]<current_distance)){
            continue;
        }
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
    return NOMOVE;
}

//WRITE ME!
void measure(){
    ;
}

//NEEDS HARDWARE INTERFACE
void forward(){
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

//NEEDS HARDWARE INTERFACE
void turn_right(){
    current_direction=current_direction-1;
    while(current_direction<0){
        current_direction=current_direction+4;
    }
}

//NEEDS HARDWARE INTERFACE
void turn_left(){
    current_direction=current_direction+1;
    while(current_direction>3){
        current_direction=current_direction-4;
    }
}

void make_move(int move,int number){
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
    for(int i=0;i<number;i++){
        forward();
    }
}

void navigate(int target_y,int target_x){
    measure();
    int move=0;
    while(move!=NOMOVE){
        move=next_move(target_y,target_x);
        make_move(move,1);
        print_move(move);
        printf("\n");
        measure();
    }
    printf("Done!\n");
}

void sprint(int start_y,int start_x,int objective_y,int objective_x){
    printf("Returning to start!\n");
    navigate(start_y,start_x);
    int real_y=current_y;
    int real_x=current_x;
    int real_direction=current_direction;
    int moves[MAXDISTANCE];
    int move=0;
    int move_counter=0;
    while(move!=NOMOVE){
        move=next_move(objective_y,objective_x);
        switch(move){
            case RIGHT:
                current_direction=0;
                current_x++;
            break;
            case UP:
                current_direction=1;
                current_y--;
            break;
            case LEFT:
                current_direction=2;
                current_x--;
            break;
            case DOWN:
                current_direction=3;
                current_y++;
            break;
        }
        moves[move_counter]=move;
        move_counter++;
    }
    int path_length=move_counter;
    move_counter=1;
    for(int i=1;i<=path_length;i++){
        if(moves[i]==moves[i-1]){
            move_counter++;
            continue;
        }
        else{
            make_move(moves[i-1],move_counter);
        }
        if(move_counter==1){
            print_move(moves[i-1]);
            printf("\n");
        }
        else{
            print_move(moves[i-1]);
            printf(" %d times\n",move_counter);
        }
        move_counter=1;
    }
}  

int main(){
    int maze[MAXMAZESIZE][MAXMAZESIZE]={
    {3, 1, 3, 1, 3, 3, 1, 1, 1, 1, 2},
    {2, 1, 0, 2, 2, 2, 2, 3, 2, 1, 2},
    {3, 1, 1, 0, 2, 0, 2, 2, 1, 0, 2},
    {2, 2, 2, 1, 1, 1, 2, 1, 3, 1, 2},
    {2, 1, 3, 3, 0, 2, 1, 0, 0, 2, 2},
    {2, 2, 2, 0, 2, 2, 0, 1, 1, 2, 2},
    {2, 2, 2, 3, 1, 2, 3, 1, 2, 0, 2},
    {2, 2, 1, 2, 2, 1, 0, 2, 1, 2, 2},
    {2, 3, 0, 2, 1, 1, 3, 1, 0, 2, 2},
    {2, 1, 1, 1, 0, 3, 0, 3, 1, 0, 2},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
};

    // int maze[MAXMAZESIZE][MAXMAZESIZE]={
    // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // };

//initialize distances matrix to all MAXDISTANCE
for(int i=0;i<MAXMAZESIZE;i++){
    for(int j=0;j<MAXMAZESIZE;j++){
        distances[i][j]=MAXDISTANCE;
    }
}

//initialize visited matrix to all MAXDISTANCE
for(int i=0;i<MAXMAZESIZE;i++){
    for(int j=0;j<MAXMAZESIZE;j++){
        visited[i][j]=MAXDISTANCE;
    }
}

//initialize memory to all 0
for(int i=0;i<MAXMAZESIZE;i++){
    for(int j=0;j<MAXMAZESIZE;j++){
        memory[i][j]=0;
    }
}

//load the maze into memory
for(int i=0;i<MAXMAZESIZE;i++){
    for(int j=0;j<MAXMAZESIZE;j++){
        memory[i][j]=maze[i][j];
    }
}

int start_y=0;
int start_x=0;
int start_direction=RIGHT;
int goal_y=3;
int goal_x=3;
current_y=start_y;
current_x=start_x;
current_direction=start_direction;
sprint(start_y,start_x,goal_y,goal_x);
}