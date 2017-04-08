#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>

#define JSH_PROMPT "jsh # "
#define JSH_RL_BUFSIZE 1024
#define JSH_TOK_BUFSIZE 64
#define JSH_TOK_DELIM " \r\n\t\a"

#include "jsh.h"

fun_desc_t cmd_table[] = {
    {cmd_exit, "exit", "exit this shell"},
    {cmd_cd, "cd", "change current directory"},
    {cmd_help, "?", "print this menu"},
};

int main(int argc, char **argv) {
    
    // TODO: Load config here (/etc/jsh/jsh.conf)?

    // command loop
    jsh_loop();

    return 0;

}

void jsh_loop(void) {

    char *cmd;
    char **args;
    int exit = 1; // This is a hack so jsh handles newlines...dont ask
    bool is_blt;

    do {
        is_blt = false;
        printf(JSH_PROMPT);

        cmd = jsh_get_cmd();

        if (cmd[0] == '\0')
            continue;

        args = jsh_get_args(cmd);
        // Testing if args is a built-in
        for (int i = 0; i < (sizeof(cmd_table)/sizeof(cmd_table[0])); i++) {
            if (strcmp(args[0], cmd_table[i].name) == 0) {
                (*cmd_table[i].func)(args[1]);
                is_blt = true;
            }
        }

        if (!is_blt)
            exit = jsh_exec(args);

        free(cmd);
        free(args);
    } while(exit);
}

char *jsh_get_cmd(void) {

    int buffsz = JSH_RL_BUFSIZE;
    char *buff = (char *) malloc(sizeof(char) * buffsz);
    int p;
    int pos = 0;

    if (!buff) {
        fprintf(stderr, "jsh read-line allocation error!\n");
        exit(-1);
    }

    while(1) {
        p = getchar();
        if (p == EOF || p == '\n') {
            buff[pos] = '\0';
            return buff;
        } else {
            buff[pos] = p;
        }
        pos++;

        if (pos >= buffsz) {
            buffsz += JSH_RL_BUFSIZE;
            buff = (char *) realloc(buff, buffsz);

            if (!buff) {
                fprintf(stderr, "jsh read-line allocation error!\n");
                exit(-1);
            }
        }
    }
}

char **jsh_get_args(char *cmd) {
    int buffsz = JSH_TOK_BUFSIZE;
    char **toks = (char **) malloc(sizeof(char**) * buffsz);
    char *tok;
    int pos = 0;
    
    if (!toks) {
        fprintf(stderr, "jsh tokens allocation error!\n");
        exit(-1);
    }

    tok = strtok(cmd, JSH_TOK_DELIM);
    while (tok != NULL) {
        toks[pos] = tok;
        pos++;

        if (pos >= buffsz) {
            buffsz += JSH_TOK_BUFSIZE;
            toks = (char **) realloc(toks, buffsz);

            if (!toks){
                fprintf(stderr, "jsh tokens allocation error!\n");
                exit(-1);
            }
        }
        tok = strtok(NULL, JSH_TOK_DELIM);
    }
    toks[pos] = NULL;
    return toks;
}

int jsh_exec(char **args) {
    pid_t pid;
    //int stat;

    pid = fork();
    // Child
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            fprintf(stderr, "jsh error!\n");
            exit(-1);
        }
    } else if (pid < 0) {
        fprintf(stderr, "jsh error!\n");
        exit(-1);
    } else {
        wait(&pid);
    }
    return 1;
}

// Built-in help menu
int cmd_help(char *args) {
    for (int i = 0; i < sizeof(cmd_table) / sizeof(cmd_table[0]); i++)
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].doc);
    return 1;
}

// Built-in exit function
int cmd_exit(char *args) {
    exit(0);
}

// Built-in change dir
int cmd_cd(char *args) {
    chdir(args);
    // TODO: print error if cd to non-existing directory
    return 1;
}

