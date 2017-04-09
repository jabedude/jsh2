#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>

#include "jsh.h"

fun_desc_t cmd_table[] = {
        {cmd_cd, "cd", "change current directory"},
        {cmd_exit, "exit", "exit this shell"},
        {cmd_help, "?", "print this menu"},
};

/*
 * TODO: Load config here (/etc/jsh/jsh.conf)
 * TODO: Keep history file (.jsh_history)
 */
int main(int argc, char **argv) {
        // char *hist_buf = (char *) malloc(sizeof(char) * JSH_HIST_BUFSIZE);
        /* command loop */
        jsh_loop();

        return 0;

}

void jsh_loop(void) {

        char *cmd;
        char **args;
        int exit = 0; /* This is a hack so jsh handles newlines...dont ask */
        bool is_blt;
        FILE *hist_p;

        hist_p = fopen("./.jsh_history", "a");

        do {
                is_blt = false;
                printf(JSH_PROMPT);

                cmd = jsh_get_cmd();

                if (cmd[0] == '\0')
                        continue;

                fprintf(hist_p, "\n%s", cmd);

                args = jsh_get_args(cmd);

                /* Testing if args is a built-in */
                for (int i = 0; i < (sizeof(cmd_table)/sizeof(cmd_table[0])); i++) {
                        if (strcmp(args[0], cmd_table[i].name) == 0) {
                                exit = (*cmd_table[i].func)(args[1]);
                                is_blt = true;
                        }
                }

                if (!is_blt)
                        exit = jsh_exec(args);

                free(cmd);
                free(args);
        } while(!exit);

        fclose(hist_p);
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
        /* Child */
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
        return 0;
}

/* Built-in help menu */
int cmd_help(char *args) {
        for (int i = 0; i < sizeof(cmd_table) / sizeof(cmd_table[0]); i++)
                printf("%s - %s\n", cmd_table[i].name, cmd_table[i].doc);
        return 0;
}

/* Built-in exit function 
 */
int cmd_exit(char *args) {
        return(1);
}

/* Built-in change dir
 */
int cmd_cd(char *args) {
        int ret;
        const char *hm_dir;

        if (args == NULL) {
                /* getting $HOME var */
                if ((hm_dir = getenv("HOME"))) {
                        ret = chdir(hm_dir);
                        if (ret)
                                fprintf(stderr, "cd: %s: error: %s\n", args, strerror(errno));
                } else
                        return 0;
                return 0;
        }

        ret = chdir(args);
        if (ret)
                fprintf(stderr, "cd: %s: error: %s\n", args, strerror(errno));

        return 0;
}

