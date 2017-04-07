#ifndef JSH_HEADER
#define JSH_HEADER

void jsh_loop(void);
char *jsh_get_cmd(void);
char **jsh_get_args(char *cmd);
int jsh_exec(char **args);

// jsh built-in functions
int cmd_exit(char *cmd);
int cmd_cd(char *cmd);
int cmd_help(char *cmd);

// Built-in type
typedef int cmd_fun_t(char *cmd);

typedef struct {
    cmd_fun_t *func;
    char *name;
    char *doc;
} fun_desc_t;

#endif
