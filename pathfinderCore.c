#include <stdio.h>
#include <stdlib.h>

#define MAXMAZESIZE 11
#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3

void print_matrix(int matrix[MAXMAZESIZE][MAXMAZESIZE]){
    // Printing the matrix
    printf("Matrix:\n");
    for (int i = 0; i < MAXMAZESIZE; i++) {
        for (int j = 0; j < MAXMAZESIZE ; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

//run "*neighbors=(int *)calloc(12, sizeof(int));"
void neighbors(int x,int y,int bias,int *pointer){
    if (bias==RIGHT){
        pointer[0] = x+1;
        pointer[1] = y;
        pointer[2] = RIGHT;
        pointer[3] = x;
        pointer[4] = y+1;
        pointer[5] = UP;
        pointer[6] = x;
        pointer[7] = y-1;
        pointer[8] = DOWN;
        pointer[9] = x-1;
        pointer[10] = y;
        pointer[11] = LEFT;
    }
    if (bias==UP){
        pointer[0] = x;
        pointer[1] = y+1;
        pointer[2] = UP;
        pointer[3] = x+1;
        pointer[4] = y;
        pointer[5] = RIGHT;
        pointer[6] = x-1;
        pointer[7] = y;
        pointer[8] = LEFT;
        pointer[9] = x;
        pointer[10] = y-1;
        pointer[11] = DOWN;
    }
    if (bias==LEFT){
        pointer[0] = x-1;
        pointer[1] = y;
        pointer[2] = LEFT;
        pointer[3] = x;
        pointer[4] = y+1;
        pointer[5] = UP;
        pointer[6] = x;
        pointer[7] = y-1;
        pointer[8] = DOWN;
        pointer[9] = x-1;
        pointer[10] = y;
        pointer[11] = RIGHT;
    }
    if (bias==DOWN){
        pointer[0] = x;
        pointer[1] = y-1;
        pointer[2] = DOWN;
        pointer[3] = x+1;
        pointer[4] = y;
        pointer[5] = RIGHT;
        pointer[6] = x-1;
        pointer[7] = y;
        pointer[8] = LEFT;
        pointer[9] = x;
        pointer[10] = y+1;
        pointer[11] = UP;
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

int *neighborsb=(int *)calloc(12, sizeof(int));
neighborsb[0]=1;
neighborsb[1]=2;
neighborsb[2]=3;
neighborsb[3]=4;
neighborsb[4]=5;
neighborsb[5]=6;
neighborsb[6]=7;
neighborsb[7]=8;
neighborsb[8]=9;
neighborsb[9]=10;
neighborsb[10]=11;
neighborsb[11]=12;
neighbors(1,1,UP,neighborsb);
printf("%d,%d,%d\n%d,%d,%d\n%d,%d,%d\n%d,%d,%d\n",neighborsb[0],neighborsb[1],neighborsb[2],neighborsb[3],neighborsb[4],neighborsb[5],neighborsb[6],neighborsb[7],neighborsb[8],neighborsb[9],neighborsb[10],neighborsb[11]);
}