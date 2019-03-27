#ifndef REDIR_FUNC_HEADER_SD
#define REDIR_FUNC_HEADER_SD

int redir_output(char* args,int mode);
void redir_input(char* args);
void othercommands(char** c_args,int check);
void redirect_pipe(char** piped);
void childSignalHandler(int signum);
void Jobs(char** args);
void Foregnd(char **args);
void Backgnd(char **args);
void killAll(char **args);
void update_processes();
void update_process();
void ctrl_c_handler(int signum);
void ctrl_z_handler(int signum);
#endif
