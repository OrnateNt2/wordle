#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 2500

void loadWords(const char *filename, char words[MAX_WORDS][MAX_WORD_LENGTH], int *wordCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка открытия файла.\n");
        exit(1);
    }

    *wordCount = 0;
    while (fgets(words[*wordCount], MAX_WORD_LENGTH, file)) {
        // Удаляем символ новой строки
        words[*wordCount][strcspn(words[*wordCount], "\n")] = 0;
        (*wordCount)++;
        if (*wordCount >= MAX_WORDS) {
            printf("Превышен максимальный размер словаря.\n");
            break;
        }
    }
    fclose(file);
}

char *trimWhitespace(char *str) {
    char *end;

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

int isWordInDictionary(const char *word, char words[MAX_WORDS][MAX_WORD_LENGTH], int wordCount) {
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

void getHint(const char *guess, const char *word, char *hint) {
    int wordLength = strlen(word)-1;
    int correctPositions[MAX_WORD_LENGTH] = {0};
    int usedInGuess[MAX_WORD_LENGTH] = {0};

    // Первоначально отметим правильные позиции
    for (int i = 0; i < wordLength; i++) {
        if (guess[i] == word[i]) {
            hint[i] = '#';
            correctPositions[i] = 1;
            usedInGuess[i] = 1;
        } else {
            hint[i] = '_';
        }
    }

    // Отметим символы, которые существуют, но находятся не на своем месте
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

    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int wordCount;
    loadWords("dictionary.txt", words, &wordCount);

    if (wordCount == 0) {
        printf("Словарь пуст или не удалось загрузить слова.\n");
        return 1;
    }

    char word[MAX_WORD_LENGTH];
    strcpy(word, words[rand() % wordCount]);

#ifdef DEBUG
    printf("Загаданное слово: %s\n", word);
#endif

    int attempts = 6;
    char guess[MAX_WORD_LENGTH];
    char hint[MAX_WORD_LENGTH];

    printf("Добро пожаловать в игру Wordle!\n");
    printf("У вас есть %d попыток, чтобы угадать слово.\n", attempts);
    printf("Загаданное слово имеет длину %lu символов.\n", strlen(word)-1);

    while (attempts > 0) {
        printf("Введите ваше предположение: ");
        fgets(guess, MAX_WORD_LENGTH, stdin);
        guess[strcspn(guess, "\n")] = 0;

        if (strlen(guess) != strlen(word)-1) {
            printf("Слово должно быть длиной %lu символов.\n", strlen(word)-1);
            continue;
        }

        if (!isWordInDictionary(guess, words, wordCount)) {
            printf("Такого слова нет в словаре. Пожалуйста, введите существующее слово.\n");
            continue;
        }
        //СТРАННО ЧТО -13. В ином случае -8
        if (strcmp(guess, word) == -13) {
            printf("Поздравляем! Вы угадали слово: %s\n", word);
            break;
        }

        getHint(guess, word, hint);
        printf("Подсказка: %s\n", hint);

        attempts--;
        if (attempts == 0) {
            printf("К сожалению, вы не смогли угадать слово. Это было: %s\n", word);
        } else {
            printf("У вас осталось %d попыток.\n", attempts);
        }
    }

    return 0;
}
