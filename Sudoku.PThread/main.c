#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

int thread_count = 4;

/**
*   Fájl beolvasás,
*
*   INPUT: fálnév
*   OUTPUT: SIZE x SIZE tömb
*/
int ** readInput(char *filename){
   FILE *infile;
   infile = fopen(filename, "r");

   int i, j;
   int _errno;
   char dummy;
   int value;
   int **sudokuGrid = (int**)malloc(sizeof(int*)*SIZE);
   // inicializálás
   for (i=0;i<SIZE;i++){
      sudokuGrid[i] = (int*)malloc(sizeof(int)*SIZE);
      for (j=0;j<SIZE;j++)
         sudokuGrid[i][j] = 0;
   }

   for (i = 0; i < SIZE; i++){
      for (j = 0; j < SIZE; j++){
         // Ha fájl vége indikátor találunk és a sorok száma != SIZE-zal, kilépünk
        if (feof(infile)){
            if (i != SIZE){
               printf("A GRID sorainak szama kevesebb, mint %d. \n", SIZE);
               exit(-1);
            }
        }

        _errno = fscanf_s(infile, "%d", &value);
        if (_errno > 0) {
            printf("ERR: %s\n", strerror(_errno));
        }

        if(value >= 0 && value <= SIZE) {
            sudokuGrid[i][j] = value;
        } else {
            printf("Hibas erteket tartalmazo GRID: (0 <= n <= %d), %dx%d.\n", SIZE, SIZE, SIZE);
            exit(-1);
         }
      }
      fscanf(infile, "%c", &dummy);

      if (j > SIZE){
         printf("Az %d. sor tobb elemet tartalmaz, mint %d. \n", i + 1, SIZE);
         exit(-1);
      }
   }

   return sudokuGrid;
}

int checkRowOrColumn(int **grid, int isRow) {
    int valuesSeen[SIZE];
    int i,j,k;

    printf(isRow ? "Sorok ellenorzese.\n" : "Oszlopok ellenorzese.\n");

    for (i = 0; i < SIZE; i++){
        for (k = 0; k < SIZE; k++)
            valuesSeen[k] = 0;

        for (j = 0; j < SIZE; j++){
            int item = isRow ? grid[i][j] : grid[j][i];

            if (valuesSeen[item - 1] == 1){
                printf("Ismetlodo elem: %d, (%d, %d) \n", item, isRow ? i : j, isRow ? j : i);
                return 0;
            }

            valuesSeen[item - 1] = 1;
        }
    }

    return 1;
}

int checkMiniGrid(int **grid) {
    int valuesSeen[SIZE];
    int i,j,k;

    for (i = 0; i < SIZE; i = i + MINIGRIDSIZE){
        for (j = 0; j < SIZE; j = j + MINIGRIDSIZE){
            // init
            for (k = 0; k < SIZE; k++)
                valuesSeen[k] = 0;

            int r,s;
            for (r = i; r < i + MINIGRIDSIZE; r++) {
                for (s = j; s < j + MINIGRIDSIZE; s++){
                    int item = grid[r][s];

                    if (valuesSeen[item - 1] == 1) {
                        printf("Ismetlodo elem a minigridbenban: %d, (%d, %d) \n\n", item, i, j);
                        return 0;
                    }

                    valuesSeen[item - 1] = 1;
                }
            }
        }
    }

    return 1;
}

int isValid(int **original, int **solution){
    int i,j;

    // Csalás ellenõrzése
    for (i = 0; i < SIZE; i++){
        for (j = 0; j < SIZE; j++){
            if (solution[i][j] == 0)
                return 0;

            if (original[i][j] > 0 && (solution[i][j] != original[i][j])) {
                printf("Nem egyezo ertek: %d != %d, (%d, %d) \n", original[i][j], solution[i][j], i, j);
                return 0;
            }
        }
    }

    if (checkRowOrColumn(solution, 1) == 0) {
        printf("Hibas GRID megoldas. Sor ellenorzes. /n");
        return 0;
    }

    if (checkRowOrColumn(solution, 0) == 0) {
        printf("Hibas GRID megoldas. Oszlop ellenorzes. /n");
        return 0;
    }

    if (checkMiniGrid(solution) == 0) {
        printf("Hibas GRID megoldas. MiniGrid ellenorzes. /n");
        return 0;
    }

    return 1;
}

int main(int argc, char *argv[]){
    if (argc < 3 && argv[2] == NULL){
      printf("Usage: ./sudoku <threadcnt> <inputFile>\n");
      exit(0);
    }

    thread_count = atoi(argv[1]);
    if (thread_count<=0){
      printf("A szalak szama nagyobb kell, hogy legyen nullanal.\n");
    }

    printf("Szalak szama: %s ...\n", argv[1]);
    printf("Fajl beolvasasa: %s ...\n", argv[2]);

    int **originalGrid = readInput(argv[2]);

    printf("Fajl beolvasva. \n");

    time_t start;
    time_t finish;
    double seconds;

    int i;
    int j;
    printf("************************EREDTI GRID***********************\n");
    for (i = 0; i < SIZE; i++){
        for (j = 0; j < SIZE; j++){
            printf("%d ", originalGrid[i][j]);
        }
        printf("\n");
    }
    printf("*********************************************************\n");
    time(&start);

    pthread_t threads[thread_count];
    int rc;
    long t;

    for (t = 0; t < thread_count; t++) {
        printf("%d szal letrehozasa.\n", t);

        thread_t pthread_s;
        pthread_s = (thread_t)malloc(sizeof(thread_t));
        pthread_s->matrix = originalGrid;
        pthread_s->tid = t;

        rc = pthread_create(&threads[t], NULL, solveSudoku, (void *)pthread_s);
        if (rc) {
            printf("ERORR; return code from pthread_create() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    int **outputGrid;
    for (i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], (void **)outputGrid);
    }
    //int **outputGrid = solveSudoku(originalGrid);
    time(&finish);
    printf("************************MEGOLDOTT GRID***********************\n");
    for (i=0;i<SIZE;i++){
        for (j=0;j<SIZE;j++) {
            printf("%d ",outputGrid[i][j]);
        }
        printf("\n");
    }
    printf("*********************************************************\n");
    seconds = difftime(finish, start);
    if (isValid(originalGrid,outputGrid)){
      printf("SOLVED \n TIME = %lf\n",(seconds));
    } else{
      printf("UNSOLVED \n TIME =%lf\n",(seconds));
    }

    return 0;
}
