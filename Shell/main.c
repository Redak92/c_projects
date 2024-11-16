#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void handleChildExit(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    char buffer[MAX_CMD_LENGTH];  
    char* arguments[MAX_ARGS];    
    char* commands[MAX_ARGS];     

    signal(SIGCHLD, handleChildExit);  

    printf("Welcome to the Simple Shell!\n");
    printf("Type 'help' for a list of available commands.\n");

    while (1) {
        readCommand(buffer);  
        if (buffer[0] == '\0') {
            continue;  
        }

        saveToHistory(buffer);  

        int numCommands = splitPipeline(buffer, commands);  
        if (numCommands > 1) {
            executePipeline(commands);
        } else {
            parseCommand(buffer, arguments);  
            if (!executeInternalCommand(arguments)) {
                executeCommand(arguments);  
            }
        }
    }

    return 0;
}
