#include <stdio.h>

#define MAXMAZESIZE 11

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
print_matrix(maze);
}