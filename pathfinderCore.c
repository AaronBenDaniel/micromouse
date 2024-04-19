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
#define MAXMAZESIZE 11
#define MAXDISTANCE 1000

//initialize distances matrix
int distances[MAXMAZESIZE][MAXMAZESIZE];

//initialize visited matrix
int visited[MAXMAZESIZE][MAXMAZESIZE];

//initialize memory matrix
int memory[MAXMAZESIZE][MAXMAZESIZE];

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

//run \/ \/ \/ 
//*neighbors=(int *)calloc(12, sizeof(int));
void neighbors(int y,int x,int bias,int *pointer){
    if (bias==RIGHT){
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
    }
    if (bias==UP){
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
    }
    if (bias==LEFT){
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
    }
    if (bias==DOWN){
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

for(int i=0;i<MAXMAZESIZE;i++){
    for(int j=0;j<MAXMAZESIZE;j++){
        memory[i][j]=maze[i][j];
    }
}
int start_y=0;
int start_x=0;
int goal_y=3;
int goal_x=3;
maze_distances(goal_y,goal_x);
printf("Distances=\n");
print_matrix(distances);
}