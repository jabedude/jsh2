#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JSH_PROMPT "jsh # "
#define JSH_RL_BUFSIZE 1024
#define JSH_TOK_BUFSIZE 64
#define JSH_TOK_DELIM " \r\n\t\a"

void jsh_loop(void);
char *jsh_get_cmd(void);
char **jsh_get_args(char *cmd);
int jsh_exec(char **args);

int main(int argc, char **argv){
    
    //Load config?

    //command loop
    jsh_loop();

    return 0;

}

void jsh_loop(void){

    char *cmd;
    char **args;
    int exit;

    do {

        printf(JSH_PROMPT);
        cmd = jsh_get_cmd();
        args = jsh_get_args(cmd);
        exit = jsh_exec(args);

        free(cmd);
        free(args);
    } while(exit);

}

char *jsh_get_cmd(void){
    //TODO: replace with getline() in stdio.h

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

char **jsh_get_args(char *cmd){
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

int jsh_exec(char **args){
    return 0;
}

