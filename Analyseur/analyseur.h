#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    char word[50];  
    int freq;       
} Word;


FILE* openFileRead(const char* path);
int countLines(FILE* file);
int countChars(FILE* file);
int countWords(FILE* file);
int analyzeWordFreq(FILE* file, Word* words, int maxSize);
void sortWordsByFreq(Word* words, int size);
void displayTopWords(Word* words, int size, int limit);
void saveResults(const char* outputPath, int lineCount, int wordCount, int charCount, Word* words, int uniqueWords);

#endif 
