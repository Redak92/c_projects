#include <stdio.h>
#include "puzzle.h"

void showMenu() {
    printf("\n--- Command Menu ---\n");
    printf("U : Move the empty tile up\n");
    printf("D : Move the empty tile down\n");
    printf("L : Move the empty tile left\n");
    printf("R : Move the empty tile right\n");
    printf("S : Save the game\n");
    printf("C : Load a saved game\n");
    printf("N : Start a new game\n");
    printf("Q : Quit the game\n");
    printf("--------------------\n");
}

int main() {
    int grid[GRID_SIZE][GRID_SIZE];
    char command;

    
    printf("Would you like to load the last saved game? (Y/N): ");
    scanf(" %c", &command);
    if (command == 'Y' || command == 'y') {
        loadGame(grid);
    } else {
        initializeGrid(grid);
        shuffleGrid(grid);
    }
    showMenu();
    printf("Current Grid:\n");
    displayGrid(grid);

    
    while (1) {
        printf("Enter a command: ");
        scanf(" %c", &command);

        switch (command) {
            case 'Q': case 'q':
                printf("Game ended.\n");
                return 0;

            case 'S': case 's':
                saveGame(grid);
                break;

            case 'C': case 'c':
                loadGame(grid);
                displayGrid(grid);
                break;

            case 'n': case 'N':
                initializeGrid(grid);
                shuffleGrid(grid);
                printf("New game started.\n");
                displayGrid(grid);
                break;

            case 'u':
            case 'd':
            case 'l':
            case 'r':
                if (moveEmptyTile(grid, command)) {
                    displayGrid(grid);
                    if (checkVictory(grid)) {
                        printf("Congratulations! You've solved the puzzle.\n");
                        displayGrid(grid);
                        return 0;
                    }
                } else {
                    printf("Move not possible.\n");
                }
                break;

            default:
                printf("Invalid command.\n");
                break;
        }
    }
    return 0;
}
