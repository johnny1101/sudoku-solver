#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include <string.h>
#include <time.h>

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
    if (argc < 2 && argv[1] == NULL){
      printf("Usage: ./sudoku <inputFile>\n");
      exit(0);
    }

    printf("Fajl beolvasasa: %s ...\n", argv[1]);

    int **originalGrid = readInput(argv[1]);

    printf("Fajl beolvasva. \n");

    time_t start;
    time_t finish;
    double seconds;

    int i;
    int j;
    printf("************************ORIGINAL GRID***********************\n");
    for (i = 0; i < SIZE; i++){
        for (j = 0; j < SIZE; j++){
            printf("%d ", originalGrid[i][j]);
        }
        printf("\n");
    }
    printf("*********************************************************\n");
    time(&start);
    int **outputGrid = solveSudoku(originalGrid);
    time(&finish);
    printf("************************SOLVED GRID***********************\n");
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
