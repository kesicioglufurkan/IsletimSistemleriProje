#include <stdio.h>
#include <string.h>
#include "parse_command.h"

// Argümanları ayrıştıran fonksiyon
int parse_command(char *input, char **args) {
	
    int argc = 0;
    char *current = input;
    char *start = NULL;
    int in_quotes = 0;
    char quote_char = '\0';

    while (*current != '\0') {
        if (in_quotes) {
            if (*current == quote_char) {
                in_quotes = 0;
                *current = '\0'; // Tırnağı kapat
            }
        } else {
            if (*current == '\'' || *current == '"') {
                in_quotes = 1;
                quote_char = *current;
                start = current + 1; // Tırnaktan sonraki karakter
                current++;
                continue;
            } else if (*current == ' ') {
                if (start != NULL) {
                    *current = '\0'; // Argümanı sonlandır
                    args[argc++] = start;
                    start = NULL;
                }
            } else {
                if (start == NULL) {
                    start = current; // Argüman başlangıcı
                }
            }
        }
        current++;
    }

    if (start != NULL) {
        args[argc++] = start; // Son argüman
    }
    args[argc] = NULL; // Argümanları sonlandır
    return argc;
}
/*
int parse_command(char *input, char **args) {

    int argc = 0;
    char *current = input;
    char *start = NULL;
    int in_quotes = 0;
    char quote_char = '\0';

    // Komut satırını tek tek işleyerek ayırma
    while (*current != '\0') {
        if (in_quotes) {
            // Tırnak içindeyken, tırnağı kapat
            if (*current == quote_char) {
                in_quotes = 0;
                *current = '\0'; // Tırnağı kapat
            }
        } else {
            // Tırnak karakteri bulunduğunda
            if (*current == '\'' || *current == '"') {
                in_quotes = 1;
                quote_char = *current;
                start = current + 1; // Tırnaktan sonraki karakter
                current++;
                continue;
            } else if (*current == ' ') {
                // Boşluk bulunursa ve argüman varsa, argümanı sonlandır
                if (start != NULL) {
                    *current = '\0'; // Argümanı sonlandır
                    args[argc++] = start;
                    start = NULL;
                }
            } else {
                // Argüman başlangıcı
                if (start == NULL) {
                    start = current;
                }
            }
        }
        current++;
    }

    // Son argümanı ekle
    if (start != NULL) {
        args[argc++] = start;
    }
    args[argc] = NULL; // Argümanları sonlandır

    return argc;
}*/