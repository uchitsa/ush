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

char **ush_split_line(char *line) {
    return NULL;
}

char *ush_read_line(void) {
}

