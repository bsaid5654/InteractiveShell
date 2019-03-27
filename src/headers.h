#ifndef JUSTHEADERS_SD
#define JUSTHEADERS_SD

#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include<dirent.h>
#include <stdlib.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

typedef struct node{
  char command[1000];
  int pids;
  int check;
  //int pgid_req;
}node;

int procid;
node frontend;
node process[1000];
int total_pid[1];
char home[1007],pwdir[1007],send[1007];
#endif
