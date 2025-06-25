#include "Library.h"
#include <string.h>
#include <ctype.h>

static char current_word[256];
static int current_index = 0;
static char most_frequent = '\0';

void init_letter_sender(const char* word) {
    strncpy(current_word, word, sizeof(current_word) - 1);
    current_word[sizeof(current_word) - 1] = '\0';
    current_index = 0;

    // Contar letras
    int counts[256] = {0};
    for (int i = 0; word[i]; i++) {
        unsigned char c = tolower(word[i]);
        if (isalpha(c)) {
            counts[c]++;
        }
    }

    // Encontrar la letra más frecuente
    int max_count = 0;
    for (int i = 0; i < 256; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            most_frequent = i;
        }
    }
}

char next_letter() {
    if (current_word[current_index] != '\0') {
        return current_word[current_index++];
    } else {
        return '\0';  // Fin de palabra
    }
}

char get_most_frequent_letter() {
    return most_frequent;
}
