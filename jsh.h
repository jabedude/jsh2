#ifndef JSH_HEADER
#define JSH_HEADER

#include <errno.h>

#define JSH_RL_BUFSIZE 1024
#define JSH_HIST_BUFSIZE 1024
#define JSH_TOK_BUFSIZE 64
#define JSH_TOK_DELIM " \r\n\t\a"

extern int errno;

void jsh_loop(void);
char *jsh_get_cmd(void);
char **jsh_get_args(char *cmd);
int jsh_exec(char **args);
void jsh_int_handler(int sig_num);

// jsh built-in functions
int cmd_exit(char *args);
int cmd_cd(char *args);
int cmd_help(char *args);

// Built-in type
typedef int cmd_fun_t(char *cmd);

typedef struct {
        cmd_fun_t *func;
        char *name;
        char *doc;
} fun_desc_t;

#endif
