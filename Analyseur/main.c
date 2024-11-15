#include <stdio.h>
#include <stdlib.h>
#include "analyseur.h"

#define MAX_SIZE 1000 
#define LIMIT 10      

int main() {
    char inputPath[100];
    char outputPath[100];

    printf("Enter the path of the file to analyze: ");
    scanf("%s", inputPath);

    printf("Enter the path to save the results: ");
    scanf("%s", outputPath);

    FILE* file = openFileRead(inputPath);

    int lineCount = countLines(file);
    int charCount = countChars(file);
    int wordCount = countWords(file);

    Word words[MAX_SIZE];
    int uniqueWords = analyzeWordFreq(file, words, MAX_SIZE);

    sortWordsByFreq(words, uniqueWords);
    displayTopWords(words, uniqueWords, LIMIT);

    saveResults(outputPath, lineCount, wordCount, charCount, words, uniqueWords);

    fclose(file);
    return 0;
}
