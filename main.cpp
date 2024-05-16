#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <iostream>

#pragma warning(disable: 4996)
#define MAX_WORD_LENGTH 100
#define MAX_WORDS 2500

void loadWords(const char* filename, char words[MAX_WORDS][MAX_WORD_LENGTH], int* wordCount) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("������ �������� �����.\n");
        exit(1);
    }

    *wordCount = 0;
    while (fgets(words[*wordCount], MAX_WORD_LENGTH, file)) {
        // ������� ������ ����� ������
        words[*wordCount][strcspn(words[*wordCount], "\n")] = 0;
        (*wordCount)++;
        if (*wordCount >= MAX_WORDS) {
            printf("�������� ������������ ������ �������.\n");
            break;
        }
    }
    fclose(file);
}

char* trimWhitespace(char* str) {
    char* end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

int isWordInDictionary(const char* word, char words[MAX_WORDS][MAX_WORD_LENGTH], int wordCount) {
    char trimmedWord[MAX_WORD_LENGTH];
    strcpy(trimmedWord, word);
    trimWhitespace(trimmedWord);

    for (int i = 0; i < wordCount; i++) {
        char trimmedDictWord[MAX_WORD_LENGTH];
        strcpy(trimmedDictWord, words[i]);
        trimWhitespace(trimmedDictWord);

        // Debug output
        //printf("Comparing '%s' with '%s'\n", trimmedWord, trimmedDictWord);

        if (strcmp(trimmedWord, trimmedDictWord) == 0) {
            return 1;
        }
    }
    return 0;
}

void getHint(const char* guess, const char* word, char* hint) {
    int wordLength = strlen(word);
    int correctPositions[MAX_WORD_LENGTH] = { 0 };
    int usedInGuess[MAX_WORD_LENGTH] = { 0 };

    // ������������� ������� ���������� �������
    for (int i = 0; i < wordLength; i++) {
        if (guess[i] == word[i]) {
            hint[i] = '#';
            correctPositions[i] = 1;
            usedInGuess[i] = 1;
        }
        else {
            hint[i] = '_';
        }
    }

    // ������� �������, ������� ����������, �� ��������� �� �� ����� �����
    for (int i = 0; i < wordLength; i++) {
        if (hint[i] == '#') continue;
        for (int j = 0; j < wordLength; j++) {
            if (guess[i] == word[j] && !correctPositions[j] && !usedInGuess[j]) {
                hint[i] = 'o';
                usedInGuess[j] = 1;
                break;
            }
        }
    }

    hint[wordLength] = '\0';
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int wordCount;
    loadWords("dictionary.txt", words, &wordCount);

    if (wordCount == 0) {
        printf("������� ���� ��� �� ������� ��������� �����.\n");
        return 1;
    }

    char word[MAX_WORD_LENGTH];
    strcpy(word, words[rand() % wordCount]);

//#ifdef DEBUG
    printf("���������� �����: %s\n", word);
//#endif

    int attempts = 6;
    char guess[MAX_WORD_LENGTH];
    char hint[MAX_WORD_LENGTH];

    printf("����� ���������� � ���� Wordle!\n");
    printf("� ��� ���� %d �������, ����� ������� �����.\n", attempts);
    printf("���������� ����� ����� ����� %lu ��������.\n", strlen(word));

    while (attempts > 0) {
        printf("������� ���� �������������: ");
        fgets(guess, MAX_WORD_LENGTH, stdin);
        guess[strcspn(guess, "\n")] = 0;

        if (strlen(guess) != strlen(word)) {
            printf("����� ������ ���� ������ %lu ��������.\n", strlen(word));
            continue;
        }

        if (!isWordInDictionary(guess, words, wordCount)) {
            printf("������ ����� ��� � �������. ����������, ������� ������������ �����.\n");
            continue;
        }
        //������� ��� -13. � ���� ������ -8
        if (strcmp(guess, word) == 0) {
            printf("�����������! �� ������� �����: %s\n", word);
            break;
        }

        getHint(guess, word, hint);
        printf("���������: %s\n", hint);

        attempts--;
        if (attempts == 0) {
            printf("� ���������, �� �� ������ ������� �����. ��� ����: %s\n", word);
        }
        else {
            printf("� ��� �������� %d �������.\n", attempts);
        }
    }
    scanf("");
    return 0;
}
