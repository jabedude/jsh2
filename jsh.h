#ifndef JSH_HEADER
#define JSH_HEADER

void jsh_loop(void);
char *jsh_get_cmd(void);
char **jsh_get_args(char *cmd);
int jsh_exec(char **args);

#endif
