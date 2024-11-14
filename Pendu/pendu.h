#ifndef HANGMAN_H
#define HANGMAN_H

#define MAX_ATTEMPTS 6

#define EASY_WORDS_ANIMALS 3
#define EASY_WORDS_OBJECTS 3
#define EASY_WORDS_JOBS 3

#define MEDIUM_WORDS_ANIMALS 3
#define MEDIUM_WORDS_OBJECTS 3
#define MEDIUM_WORDS_JOBS 3

#define HARD_WORDS_ANIMALS 3
#define HARD_WORDS_OBJECTS 3
#define HARD_WORDS_JOBS 3

// Définition des niveaux de difficulté
typedef enum {
    EASY,
    MEDIUM,
    HARD
} DifficultyLevel;

// Définition des catégories
typedef enum {
    ANIMALS,
    OBJECTS,
    JOBS
} Category;

// Fonctions principales
void chooseDifficultyAndCategory();
void startGame(DifficultyLevel difficulty, Category category);
char* pickSecretWord(DifficultyLevel difficulty, Category category);

// Fonctions utilitaires
void displayHiddenWord(char* word, int* guessedLetters);
int checkLetter(char* word, char letter, int* guessedLetters);
void displayHangman(int attemptsLeft);
void saveScore(const char* playerName, int score);

#endif // HANGMAN_H
