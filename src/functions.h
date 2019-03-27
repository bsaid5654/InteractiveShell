#ifndef FUNC_HEADER_SD
#define FUNC_HEADER_SD

//Placing all the function headers in here.
int changeDir(char * dir);
void createTerminal(char *dir,char* home);
char **partsof(char *line,char* del);
void Ls(char *file);
char* pinfo(char* pin,int checK);
void remindme(char **args);
void ls(char **arguments,char* pwdir);
void Setenv(char** args);
void Unsetenv(char** args);
void KJobs(char** args);

#endif
