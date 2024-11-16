#include "memoire.h"
#include <stdio.h>
#include <locale.h>

int main() {

    printf("Initializing memory...\n");
    initMemory();

    printf("\nChoose a simulation:\n");
        printf("1. Internal fragmentation simulation\n");
    printf("2. External fragmentation simulation\n");

    int userChoice;
    scanf("%d", &userChoice);

    if (userChoice == 1) {
        simulateInternalFrag();
    } else if (userChoice == 2) {
            simulateExternalFrag();
    } else {
        printf("Invalid option. Exiting program.\n");
        return 1;
    }
    return 0;
}
