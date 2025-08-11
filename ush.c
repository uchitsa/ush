//
// Created by uchitsa on 02.07.2025.
//

#include "ush.h"
#include <stdlib.h>
#include <printf.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

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

    token = strtok(line, USH_TOK_DELIM);
    while (token != NULL) {
        tokens[pos] = token;
        pos++;
        if (pos >= bufsize) {
            bufsize += USH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "ush: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, USH_TOK_DELIM);
    }
    tokens[pos] = NULL;
    return tokens;
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

int ush_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("ush");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("ush");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int ush_cd(char **args);

int ush_help(char **args);

int ush_exit(char **args);

char *builtin_str[] = {
        "cd",
        "help",
        "exit"
};

int (*builtin_func[])(char **) = {
        &ush_cd,
        &ush_help,
        &ush_exit
};

int ush_num_builtins() {
    return sizeof(builtin_str);
}

int ush_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "ush: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("ush");
        }
    }
    return 1;
}

int ush_help(char **args) {
    printf("Uchitsa's shell");
    printf("Input program name with arguments and press enter");
    printf("There are some built-in:");
    for (int i = 0; i < ush_num_builtins(); i++) {
        printf(" %s\n", builtin_str[i]);
    }
    printf("Man command can show info about other programs");
    return 1;
}


int ush_exit(char **args) {
    return 0;
};


int ush_execute(char **args) {
    if (args[0] == NULL) {
        return 1;
    }
    for (int i = 0; i < ush_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (int) args[0];
        }
    }
    return ush_launch(args);
}