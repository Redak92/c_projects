#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

static char history[HISTORY_SIZE][MAX_CMD_LENGTH];
static int historyCount = 0;

static Alias aliases[MAX_ARGS];
static int aliasCount = 0;


void readCommand(char* buffer) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s > ", cwd);
    } else {
        perror("getcwd failed");
    }

    if (fgets(buffer, MAX_CMD_LENGTH, stdin) == NULL) {
        perror("Error reading command");
        buffer[0] = '\0';  
        return;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    
    if (strlen(buffer) == 0 || strspn(buffer, " \t") == strlen(buffer)) {
        buffer[0] = '\0';
    }
}


void parseCommand(char* buffer, char** arguments) {
    if (buffer == NULL || arguments == NULL) {
        fprintf(stderr, "Error: Null pointer passed to parseCommand\n");
        return;
    }

    char* token = strtok(buffer, " ");
    int i = 0;
    while (token != NULL && i < MAX_ARGS - 1) {
        arguments[i++] = token;
        token = strtok(NULL, " ");
    }
    arguments[i] = NULL;  
}


void saveToHistory(const char* command) {
    strncpy(history[historyCount % HISTORY_SIZE], command, MAX_CMD_LENGTH);
    historyCount++;
}


void showHistory() {
    printf("Command History:\n");
    int start = (historyCount >= HISTORY_SIZE) ? (historyCount % HISTORY_SIZE) : 0;
    int count = (historyCount >= HISTORY_SIZE) ? HISTORY_SIZE : historyCount;

    for (int i = 0; i < count; i++) {
        printf("%d: %s\n", i + 1, history[(start + i) % HISTORY_SIZE]);
    }
}


void executeCommand(char** arguments) {
    if (arguments == NULL || arguments[0] == NULL) {
        fprintf(stderr, "Error: Invalid command or arguments\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        handleRedirection(arguments);  
        if (execvp(arguments[0], arguments) == -1) {
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }
    } else if (pid > 0) {
        waitpid(pid, NULL, 0);
    } else {
        perror("Fork failed");
    }
}


int executeInternalCommand(char** arguments) {
    if (strcmp(arguments[0], "exit") == 0) {
        printf("Exiting the shell...\n");
        exit(0);
    } else if (strcmp(arguments[0], "cd") == 0) {
        if (arguments[1] == NULL) {
            fprintf(stderr, "cd: missing argument\n");
        } else if (chdir(arguments[1]) != 0) {
            perror("cd");
        }
        return 1;
    } else if (strcmp(arguments[0], "history") == 0) {
        showHistory();
        return 1;
    } else if (strcmp(arguments[0], "help") == 0) {
        printf("Simple Shell Help:\n");
        printf("  exit            : Exit the shell\n");
        printf("  cd <dir>        : Change directory\n");
        printf("  history         : Show command history\n");
        printf("  <cmd> | <cmd>   : Use pipes to chain commands\n");
        printf("  <cmd> &         : Run command in background\n");
        printf("  >, <            : Redirect output or input\n");
        return 1;
    }
    return 0;
}


void handleRedirection(char** arguments) {
    int i = 0;
    while (arguments[i] != NULL) {
        if (strcmp(arguments[i], ">") == 0) {
            int fd = open(arguments[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("Failed to open file for redirection");
                return;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            arguments[i] = NULL;
        } else if (strcmp(arguments[i], "<") == 0) {
            int fd = open(arguments[i + 1], O_RDONLY);
            if (fd == -1) {
                perror("Failed to open file for input redirection");
                return;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            arguments[i] = NULL;
        }
        i++;
    }
}


int splitPipeline(char* buffer, char** commands) {
    if (buffer == NULL || commands == NULL) {
        fprintf(stderr, "Error: Null pointer passed to splitPipeline\n");
        return 0;
    }

    int i = 0;
    char* token = strtok(buffer, "|");
    while (token != NULL && i < MAX_ARGS - 1) {
        commands[i++] = token;
        token = strtok(NULL, "|");
    }
    commands[i] = NULL;
    return i;
}

void executePipeline(char** commands) {
    if (commands == NULL) {
        fprintf(stderr, "Error: Null pointer passed to executePipeline\n");
        return;
    }

    int pipefd[2];
    pid_t pid;
    int fd_in = 0;

    for (int i = 0; commands[i] != NULL; i++) {
        if (pipe(pipefd) == -1) {
            perror("Pipe failed");
            return;
        }

        if ((pid = fork()) == 0) {
            dup2(fd_in, STDIN_FILENO);
            if (commands[i + 1] != NULL) {
                dup2(pipefd[1], STDOUT_FILENO);
            }
            close(pipefd[0]);

            char* args[MAX_ARGS];
            parseCommand(commands[i], args);
            if (execvp(args[0], args) == -1) {
                perror("Pipeline execution failed");
                exit(EXIT_FAILURE);
            }
        } else if (pid > 0) {
            waitpid(pid, NULL, 0);
            close(pipefd[1]);
            fd_in = pipefd[0];
        } else {
            perror("Fork failed");
            return;
        }
    }
}
