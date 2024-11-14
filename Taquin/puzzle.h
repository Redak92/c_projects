#ifndef PUZZLE_H
#define PUZZLE_H

#define GRID_SIZE 4


void initializeGrid(int grid[GRID_SIZE][GRID_SIZE]);
void displayGrid(int grid[GRID_SIZE][GRID_SIZE]);
void shuffleGrid(int grid[GRID_SIZE][GRID_SIZE]);
int moveEmptyTile(int grid[GRID_SIZE][GRID_SIZE], char direction);
int checkVictory(int grid[GRID_SIZE][GRID_SIZE]);


void saveGame(int grid[GRID_SIZE][GRID_SIZE]);
void loadGame(int grid[GRID_SIZE][GRID_SIZE]);

#endif 
