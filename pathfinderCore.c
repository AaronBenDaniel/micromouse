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
            printf("%d ", matrix[i][j]);
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
        pointer[5] = UP;
        pointer[6] = y-1;
        pointer[7] = x;
        pointer[8] = DOWN;
        pointer[9] = y;
        pointer[10] = x-1;
        pointer[11] = LEFT;
    }
    if (bias==UP){
        pointer[0] = y+1;
        pointer[1] = x;
        pointer[2] = UP;
        pointer[3] = y;
        pointer[4] = x+1;
        pointer[5] = RIGHT;
        pointer[6] = y;
        pointer[7] = x-1;
        pointer[8] = LEFT;
        pointer[9] = y-1;
        pointer[10] = x;
        pointer[11] = DOWN;
    }
    if (bias==LEFT){
        pointer[0] = y;
        pointer[1] = x-1;
        pointer[2] = LEFT;
        pointer[3] = y+1;
        pointer[4] = x;
        pointer[5] = UP;
        pointer[6] = y-1;
        pointer[7] = x;
        pointer[8] = DOWN;
        pointer[9] = y;
        pointer[10] = x-1;
        pointer[11] = RIGHT;
    }
    if (bias==DOWN){
        pointer[0] = y-1;
        pointer[1] = x;
        pointer[2] = DOWN;
        pointer[3] = y;
        pointer[4] = x+1;
        pointer[5] = RIGHT;
        pointer[6] = y;
        pointer[7] = x-1;
        pointer[8] = LEFT;
        pointer[9] = y+1;
        pointer[10] = x;
        pointer[11] = UP;
    }
}

void maze_distances(int start_x,int start_y){
    void recurse(int *distances,int y,int x){

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

int *neighborsb=(int *)calloc(12, sizeof(int));
neighbors(1,1,UP,neighborsb);
printf("%d,%d,%d\n%d,%d,%d\n%d,%d,%d\n%d,%d,%d\n",neighborsb[0],neighborsb[1],neighborsb[2],neighborsb[3],neighborsb[4],neighborsb[5],neighborsb[6],neighborsb[7],neighborsb[8],neighborsb[9],neighborsb[10],neighborsb[11]);
free(neighborsb);
}