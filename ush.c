//
// Created by uchitsa on 02.07.2025.
//

#include "ush.h"
#include <stdlib.h>
#include <printf.h>

void ush_loop();

char *ush_read_line();

char **ush_split_line(char *line);

int ush_execute(char **pString);

int main(int argc, char **argv) {
    ush_loop();
    return EXIT_SUCCESS;
}

void ush_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = ush_read_line();
        args = ush_split_line(line);
        status = ush_execute(args);

        free(line);
        free(args);
    } while (status);
}

int ush_execute(char **args) {
    return 0;
}

#define USH_TOK_BUFSIZE 64
#define USH_TOK_DELIM " \t\r\n\a"

char **ush_split_line(char *line) {
    int bufsize = USH_TOK_BUFSIZE, pos = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "ush: allocation error\n");
        exit(EXIT_FAILURE);
    }
    return NULL;
}

#define USH_RL_BUFSIZE 1024

char *ush_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

