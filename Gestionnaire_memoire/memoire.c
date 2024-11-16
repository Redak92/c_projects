#include "memoire.h"
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_MEMORY 1024 

static char memory[TOTAL_MEMORY]; 
static MemoryBlock* firstBlock = NULL; 
static AllocationAlgorithm currentAlgorithm = FIRST_FIT; 


void initMemory() {
    firstBlock = (MemoryBlock*)memory;
    firstBlock->size = TOTAL_MEMORY - sizeof(MemoryBlock);
        firstBlock->isFree = 1;
    firstBlock->next = NULL;
}


void setAlgorithm(AllocationAlgorithm algo) {
        currentAlgorithm = algo;
}


MemoryBlock* findFirstFit(size_t size) {
    MemoryBlock* current = firstBlock;
    while (current != NULL) {
        if (current->isFree && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


MemoryBlock* findBestFit(size_t size) {
    MemoryBlock* current = firstBlock;
        MemoryBlock* bestBlock = NULL;
    while (current != NULL) {
        if (current->isFree && current->size >= size) {
            if (bestBlock == NULL || current->size < bestBlock->size) {
                bestBlock = current;
            }
        }
        current = current->next;
    }
    return bestBlock;
}


MemoryBlock* findWorstFit(size_t size) {
    MemoryBlock* current = firstBlock;
    MemoryBlock* largestBlock = NULL;
    while (current != NULL) {
        if (current->isFree && current->size >= size) {
            if (largestBlock == NULL || current->size > largestBlock->size) {
                largestBlock = current;
            }
        }
        current = current->next;
    }
    return largestBlock;
}


void* simulatedMalloc(size_t size) {
    MemoryBlock* chosenBlock = NULL;

    if (currentAlgorithm == FIRST_FIT) {
        chosenBlock = findFirstFit(size);
    } else if (currentAlgorithm == BEST_FIT) {
        chosenBlock = findBestFit(size);
    } else if (currentAlgorithm == WORST_FIT) {
        chosenBlock = findWorstFit(size);
    }

    if (chosenBlock == NULL) {
        printf("Error: Not enough memory to allocate %zu bytes\n", size);
        return NULL;
    }

    if (chosenBlock->size > size + sizeof(MemoryBlock)) {
        MemoryBlock* newBlock = (MemoryBlock*)((char*)chosenBlock + sizeof(MemoryBlock) + size);
        newBlock->size = chosenBlock->size - size - sizeof(MemoryBlock);
            newBlock->isFree = 1;
        newBlock->next = chosenBlock->next;

        chosenBlock->size = size;
        chosenBlock->next = newBlock;
    }

    chosenBlock->isFree = 0;
    return (void*)((char*)chosenBlock + sizeof(MemoryBlock));
}


void simulatedFree(void* ptr) {
    if (ptr == NULL) {
        printf("Error: NULL pointer passed to simulatedFree\n");
        return;
    }

    MemoryBlock* current = (MemoryBlock*)((char*)ptr - sizeof(MemoryBlock));
    current->isFree = 1;

    if (current->next != NULL && current->next->isFree) {
        current->size += sizeof(MemoryBlock) + current->next->size;
        current->next = current->next->next;
    }

    MemoryBlock* prev = firstBlock;
    while (prev != NULL && prev->next != current) {
        prev = prev->next;
    }

    if (prev != NULL && prev->isFree) {
        prev->size += sizeof(MemoryBlock) + current->size;
        prev->next = current->next;
    }
}


void displayMemory() {
    MemoryBlock* current = firstBlock;
    printf("\nCurrent memory state:\n");
    printf("-----------------------------------\n");
    printf("| Address       | Size   | Status |\n");
    printf("-----------------------------------\n");
    while (current != NULL) {
        printf("| %p | %-7zu | %-6s |\n",
               (void*)current,
               current->size,
               current->isFree ? "Free" : "Used");
        current = current->next;
    }
    printf("-----------------------------------\n");
}


void displayMemoryGraph() {
    MemoryBlock* current = firstBlock;
    printf("\nMemory visualization:\n");
    while (current != NULL) {
        if (current->isFree) {
            for (size_t i = 0; i < current->size / 10; i++) {
                printf("_");
            }
        } else {
            for (size_t i = 0; i < current->size / 10; i++) {
                printf("█");
            }
        }
        printf(" ");
        current = current->next;
    }
    printf("\n");
}


void simulateInternalFragmentation() {
    printf("\nSimulating internal fragmentation:\n");
    void* ptr1 = simulatedMalloc(150);
    void* ptr2 = simulatedMalloc(200);
    void* ptr3 = simulatedMalloc(300);

    displayMemory();
    displayMemoryGraph();

    simulatedFree(ptr1);
    simulatedFree(ptr2);
    simulatedFree(ptr3);

    displayMemory();
    displayMemoryGraph();
}


void simulateExternalFragmentation() {
    printf("\nSimulating external fragmentation:\n");
    void* ptr1 = simulatedMalloc(200);
    void* ptr2 = simulatedMalloc(300);
    void* ptr3 = simulatedMalloc(100);

    simulatedFree(ptr2);
    simulatedMalloc(50);

    displayMemory();
    displayMemoryGraph();

    simulatedFree(ptr1);
    simulatedFree(ptr3);

    displayMemory();
    displayMemoryGraph();
}
