#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "pendu.h"

// Word definitions
const char* easyAnimals[] = {"cat", "dog", "rat"};
const char* easyObjects[] = {"key", "book", "lamp"};
const char* easyJobs[] = {"firefighter", "cook", "mason"};

const char* mediumAnimals[] = {"dolphin", "horse", "pigeon"};
const char* mediumObjects[] = {"tablet", "computer", "sofa"};
const char* mediumJobs[] = {"dentist", "plumber", "electrician"};

const char* hardAnimals[] = {"hippopotamus", "chimpanzee", "platypus"};
const char* hardObjects[] = {"radiator", "refrigerator", "vacuum"};
const char* hardJobs[] = {"astrophysicist", "bioengineer", "ethnologist"};


void chooseDifficultyAndCategory() {
    int difficultyChoice, categoryChoice;
    
    printf("Select a difficulty level:\n");
    printf("1. Easy\n2. Medium\n3. Hard\nYour choice: ");
    scanf("%d", &difficultyChoice);

    DifficultyLevel difficulty;
    switch (difficultyChoice) {
        case 1: difficulty = EASY; break;
        case 2: difficulty = MEDIUM; break;
        case 3: difficulty = HARD; break;
        default: difficulty = EASY;
    }

    printf("Select a category:\n");
    printf("1. Animals\n2. Objects\n3. Jobs\nYour choice: ");
    scanf("%d", &categoryChoice);

    Category category;
    switch (categoryChoice) {
        case 1: category = ANIMALS; break;
        case 2: category = OBJECTS; break;
        case 3: category = JOBS; break;
        default: category = ANIMALS;
    }

    startGame(difficulty, category);
}

void startGame(DifficultyLevel difficulty, Category category) {
    printf("Welcome to the Hangman game!\n");

    char* secretWord = pickSecretWord(difficulty, category);
    int wordLength = strlen(secretWord);
    int* guessedLetters = (int*)malloc(wordLength * sizeof(int));
    if (guessedLetters == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < wordLength; i++) {
        guessedLetters[i] = 0;
    }

    int attemptsLeft = MAX_ATTEMPTS;
    int gameEnd = 0;
    char guessedLetter;

    char playerName[50];
    printf("Enter your name: ");
    scanf("%s", playerName);

    while (attemptsLeft > 0 && !gameEnd) {
        displayHiddenWord(secretWord, guessedLetters);
        printf("\nAttempts remaining: %d\n", attemptsLeft);
        displayHangman(attemptsLeft);

        printf("Enter a letter: ");
        scanf(" %c", &guessedLetter);

        int occurrences = checkLetter(secretWord, guessedLetter, guessedLetters);
        if (occurrences == 0) {
            attemptsLeft--;
        }

        gameEnd = 1;
        for (int i = 0; i < wordLength; i++) {
            if (guessedLetters[i] == 0) {
                gameEnd = 0;
                break;
            }
        }
    }

    if (gameEnd) {
        printf("Congratulations! You've guessed the word: %s\n", secretWord);
        saveScore(playerName, attemptsLeft);
    } else {
        printf("Sorry! You lost. The word was: %s\n", secretWord);
    }

    free(guessedLetters);
}

char* pickSecretWord(DifficultyLevel difficulty, Category category) {
    srand(time(NULL));
    switch (difficulty) {
        case EASY:
            switch (category) {
                case ANIMALS: return (char*)easyAnimals[rand() % EASY_WORDS_ANIMALS];
                case OBJECTS: return (char*)easyObjects[rand() % EASY_WORDS_OBJECTS];
                case JOBS: return (char*)easyJobs[rand() % EASY_WORDS_JOBS];
                default: return (char*)easyAnimals[0];
            }
        case MEDIUM:
            switch (category) {
                case ANIMALS: return (char*)mediumAnimals[rand() % MEDIUM_WORDS_ANIMALS];
                case OBJECTS: return (char*)mediumObjects[rand() % MEDIUM_WORDS_OBJECTS];
                case JOBS: return (char*)mediumJobs[rand() % MEDIUM_WORDS_JOBS];
                default: return (char*)mediumAnimals[0];
            }
        case HARD:
            switch (category) {
                case ANIMALS: return (char*)hardAnimals[rand() % HARD_WORDS_ANIMALS];
                case OBJECTS: return (char*)hardObjects[rand() % HARD_WORDS_OBJECTS];
                case JOBS: return (char*)hardJobs[rand() % HARD_WORDS_JOBS];
                default: return (char*)hardAnimals[0];
            }
        default:
            return (char*)easyAnimals[0];
    }
}

void displayHiddenWord(char* word, int* guessedLetters) {
    for (int i = 0; i < strlen(word); i++) {
        if (guessedLetters[i]) {
            printf("%c ", word[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

int checkLetter(char* word, char letter, int* guessedLetters) {
    int occurrences = 0;
    for (int i = 0; i < strlen(word); i++) {
        if (word[i] == letter && !guessedLetters[i]) {
            guessedLetters[i] = 1;
            occurrences++;
        }
    }
    return occurrences;
}

void displayHangman(int attemptsLeft) {
    switch (attemptsLeft) {
        case 6: printf("\n\n\n\n\n"); break;
        case 5: printf("  O\n\n\n\n"); break;
        case 4: printf("  O\n  |\n\n\n"); break;
        case 3: printf("  O\n /|\n\n\n"); break;
        case 2: printf("  O\n /|\\\n\n\n"); break;
        case 1: printf("  O\n /|\\\n /\n"); break;
        case 0: printf("  O\n /|\\\n / \\\n"); break;
    }
}

void saveScore(const char* playerName, int score) {
    FILE* file = fopen("scores.txt", "a");
    if (file != NULL) {
        fprintf(file, "Player: %s, Score: %d\n", playerName, score);
        fclose(file);
    }
}
