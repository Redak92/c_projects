#include "analyseur.h"

FILE* openFileRead(const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", path);
        exit(1);
    }
    return file;
}

int countLines(FILE* file) {
    int count = 0;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file)) {
        count++;
    }

    rewind(file);
    return count;
}

int countChars(FILE* file) {
    int count = 0;
    char c;

    while ((c = fgetc(file)) != EOF) {
        count++;
    }

    rewind(file);
    return count;
}

int countWords(FILE* file) {
    int count = 0;
    char buffer[1024];

    while (fscanf(file, "%s", buffer) == 1) {
        count++;
    }

    rewind(file);
    return count;
}

void addOrUpdateWord(const char* word, Word* words, int* size) {
    for (int i = 0; i < *size; i++) {
        if (strcmp(words[i].word, word) == 0) {
            words[i].freq++;
            return;
        }
    }

    strcpy(words[*size].word, word);
    words[*size].freq = 1;
    (*size)++;
}

int analyzeWordFreq(FILE* file, Word* words, int maxSize) {
    char buffer[50];
    int size = 0;

    while (fscanf(file, "%49s", buffer) == 1) {
        addOrUpdateWord(buffer, words, &size);

        if (size >= maxSize) {
            printf("Error: Maximum array size reached.\n");
            break;
        }
    }

    rewind(file);
    return size;
}

void sortWordsByFreq(Word* words, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (words[i].freq < words[j].freq) {
                Word temp = words[i];
                words[i] = words[j];
                words[j] = temp;
            }
        }
    }
}

void displayTopWords(Word* words, int size, int limit) {
    printf("Top %d words:\n", limit);
    for (int i = 0; i < limit && i < size; i++) {
        printf("%s : %d\n", words[i].word, words[i].freq);
    }
}

void saveResults(
    const char* outputPath,
    int lineCount,
    int wordCount,
    int charCount,
    Word* words,
    int uniqueWords
) {
    FILE* outputFile = fopen(outputPath, "w");
    if (outputFile == NULL) {
        printf("Error: Unable to create output file %s\n", outputPath);
        exit(1);
    }

    fprintf(outputFile, "Text file statistics:\n");
    fprintf(outputFile, "Lines: %d\n", lineCount);
    fprintf(outputFile, "Words: %d\n", wordCount);
    fprintf(outputFile, "Characters: %d\n\n", charCount);

    fprintf(outputFile, "Word frequencies:\n");
    for (int i = 0; i < uniqueWords; i++) {
        fprintf(outputFile, "%s : %d\n", words[i].word, words[i].freq);
    }

    fclose(outputFile);
    printf("Results saved to %s\n", outputPath);
}
