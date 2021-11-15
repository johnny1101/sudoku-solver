#ifndef SUDOKU_H
#define SUDOKU_H

#ifndef MINIGRIDSIZE
#define MINIGRIDSIZE 3
#endif
#define SIZE ((MINIGRIDSIZE)*(MINIGRIDSIZE))

typedef struct {
    int ** matrix;
    int tid;
} thread_struct;
typedef thread_struct* thread_t;

int **readInput(char *);
int isValid(int **, int **);
void **solveSudoku(void *);

#endif
