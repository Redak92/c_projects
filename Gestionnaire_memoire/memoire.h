#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>


typedef struct MemoryBlock {
    size_t size;                     
    int isFree;                      
    struct MemoryBlock* next;        
} MemoryBlock;


typedef enum {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
} AllocationAlgorithm;


void initMemory();
void* simulatedMalloc(size_t size);
void simulatedFree(void* ptr);
void displayMemory();
void displayMemoryGraph();
void setAlgorithm(AllocationAlgorithm algo);


void simulateInternalFragmentation();
void simulateExternalFragmentation();

#endif 
