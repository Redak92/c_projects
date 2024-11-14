#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "puzzle.h"


void initializeGrid(int grid[GRID_SIZE][GRID_SIZE]) {
    int counter = 1;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (counter <= 15) {
                grid[i][j] = counter;
                counter++;
            } else {
                grid[i][j] = 0;  
            }
        }
    }
}


void displayGrid(int grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == 0) {
                printf("   ");  
            } else {
                printf("%2d ", grid[i][j]);
            }
        }
        printf("\n");
    }
}


void findEmptyTile(int grid[GRID_SIZE][GRID_SIZE], int *row, int *col) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == 0) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}


int moveEmptyTile(int grid[GRID_SIZE][GRID_SIZE], char direction) {
    int row, col;
    findEmptyTile(grid, &row, &col);

    int newRow = row;
    int newCol = col;

    
    switch (direction) {
        case 'u': newRow -= 1; break; 
        case 'd': newRow += 1; break; 
        case 'l': newCol -= 1; break; 
        case 'r': newCol += 1; break; 
        default: printf("Invalid move.\n"); return 0;
    }

    
    if (newRow >= 0 && newRow < GRID_SIZE && newCol >= 0 && newCol < GRID_SIZE) {
        
        grid[row][col] = grid[newRow][newCol];
        grid[newRow][newCol] = 0;
        return 1; 
    }
    return 0; 
}


void shuffleGrid(int grid[GRID_SIZE][GRID_SIZE]) {
    srand(time(NULL));  

    
    for (int i = 0; i < 100; i++) {
        int validMove = 0;
        while (!validMove) {
            int direction = rand() % 4;
            switch (direction) {
                case 0: validMove = moveEmptyTile(grid, 'u'); break; 
                case 1: validMove = moveEmptyTile(grid, 'd'); break; 
                case 2: validMove = moveEmptyTile(grid, 'l'); break; 
                case 3: validMove = moveEmptyTile(grid, 'r'); break; 
            }
        }
    }
}


int checkVictory(int grid[GRID_SIZE][GRID_SIZE]) {
    int counter = 1;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (i == GRID_SIZE - 1 && j == GRID_SIZE - 1) {
                if (grid[i][j] != 0) {
                    return 0;
                }
            } else if (grid[i][j] != counter) {
                return 0;
            }
            counter++;
        }
    }
    return 1; 
}


void saveGame(int grid[GRID_SIZE][GRID_SIZE]) {
    FILE *file = fopen("save.txt", "w");
    if (file == NULL) {
        perror("Error opening save file");
        return;
    }

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            fprintf(file, "%d ", grid[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Game saved successfully.\n");
}


void loadGame(int grid[GRID_SIZE][GRID_SIZE]) {
    FILE *file = fopen("save.txt", "r");
    if (file == NULL) {
        printf("No save file found.\n");
        return;
    }

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            fscanf(file, "%d", &grid[i][j]);
        }
    }

    fclose(file);
    printf("Game loaded successfully.\n");
}

