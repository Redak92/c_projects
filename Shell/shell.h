#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>


#define MAX_CMD_LENGTH 1024  
#define MAX_ARGS 100         
#define HISTORY_SIZE 10      


typedef struct {
    char alias[MAX_CMD_LENGTH];
    char command[MAX_CMD_LENGTH];
} Alias;


void readCommand(char* buffer);
void parseCommand(char* buffer, char** arguments);
void executeCommand(char** arguments);
int executeInternalCommand(char** arguments);
void handleRedirection(char** arguments);
void executePipeline(char** commands);


void saveToHistory(const char* command);
void showHistory();


void addAlias(const char* alias, const char* command);
void resolveAlias(char* buffer);


int splitPipeline(char* buffer, char** commands);

#endif 
