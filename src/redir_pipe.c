#include "headers.h"
#include "functions.h"

void ctrl_c_handler(int signum)
{
  if(procid!=-1)
  {
    kill(procid,9);
    //procid = -1;
  }
  return;
}


void update_processes(){
  int i,req,some,j;
  for(i=0;i<total_pid[0];i++)
  {
    req = waitpid(process[i].pids,&some, WNOHANG | WUNTRACED);
    if(req > 0){
      if(WIFSTOPPED(some)) process[i].check = 1; //printf("%d -----\n",process[i].pids); }
      if(WIFSIGNALED(some)) process[i].check = 1;
    }
    if(req == -1){
      for(j=i;j<total_pid[0];j++){
        strcpy(process[i].command,process[i+1].command);
        process[i].pids = process[i+1].pids;
        process[i].check = process[i+1].check;
      }
      total_pid[0]--;
      i--;
    }
  }
  return;
}

void update_process(){
  int i,j,some;
  char* req; char send_req[20];
  for(i=0;i<total_pid[0];i++)
  {
    waitpid(process[i].pids,&some, WNOHANG | WUNTRACED);
    sprintf(send_req,"%d",process[i].pids);
    req = pinfo(send_req,0);
    if(!strcmp(req,"N"))
    {
      for(j=i;j<total_pid[0];j++){
        strcpy(process[i].command,process[i+1].command);
        process[i].pids = process[i+1].pids;
        process[i].check = process[i+1].check;
      }
      total_pid[0]--; i--;
    }
    else if(!strcmp(req,"S") || !strcmp(req,"R")) process[i].check = 0;
    else { process[i].check = 1; }
  }
}

void ctrl_z_handler(int signum)
{
  if(procid!=-1)
  {
    kill(procid,SIGTSTP);
    strcpy(process[total_pid[0]].command,frontend.command);
    process[total_pid[0]].pids = frontend.pids;
    total_pid[0]++;
    update_process();
    //procid = -1;
  }
  return;
}


int redir_output(char* args,int mode)
{
  int fd=0,dupl=0;
  if(args==NULL) { printf("<--------Enter the output file---------->\n");  return -1;}
  if(mode==1) { fd = open(args,O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU); }
  else if(mode==2) { fd = open(args,O_CREAT|O_APPEND|O_WRONLY,S_IRWXU); }
  if(fd==-1) {  printf("Couldn't create or access the given file\n"); }
  else { dupl = dup2(fd,1); if(dupl <= -1) perror("Terminal"); close(fd); }
  return dupl;
}

void redir_input(char* args)
{
  int fd=0,dupl=0;
  if(args==NULL) { printf("<--------Enter the output file---------->\n");  return;}
  fd = open(args,O_RDONLY,S_IRWXU);
  //printf("---(%d)---\n",fd);
  if(fd == -1){ perror("Terminal"); exit(1); }
  else{ dupl = dup2(fd,0); if(dupl <= -1) perror("Terminal"); close(fd); }
  return;
}

void childSignalHandler(int signum) {
    int status; int i;
    pid_t pid;
    pid = waitpid(-1,&status,WNOHANG);
    if(pid==0) return;
    else if(pid > -1)
    {
      if(pid > 0 && WIFEXITED(status)){ printf("\npid-- %d exited normally\n",pid); }
      if(pid > 0 && WIFSIGNALED(status)){ printf("\npid ---%d By signal\n",pid); }
    }
  return;
}


void redirect_pipe(char** piped)
{
  signal(SIGCHLD,childSignalHandler);
  int p=0; char** args; int j,i;  int new_fd[2][2]; int checK=0,some;
  for(p=0;piped[p]!=NULL;p++)
  {
    pipe(new_fd[p%2]);
    args = partsof(piped[p],"\t' '\v");
    for(i=0;args[i]!=NULL;i++);
    if(args[i-1][strlen(args[i-1])-1]=='&')
    {
      checK=1;
      if(strlen(args[i-1])==1) args[i-1]=NULL;
      else args[i-1][strlen(args[i-1])-1]='\0';

    }
    int pid = fork();
    if(pid==0)
    {
      if(p!=0) dup2(new_fd[(p-1)%2][0],0);
      if(piped[p+1]!=NULL) dup2(new_fd[(p%2)][1],1);
      for(j=0;args[j]!=NULL;j++)
      {
        if(!strcmp(args[j],">")){ redir_output(args[j+1],1); args[j]=NULL; }
        else if(!strcmp(args[j],">>")){  redir_output(args[j+1],2); args[j]=NULL; }
        else if(!strcmp(args[j],"<")){  redir_input(args[j+1]); args[j]=NULL; }
      }
      execvp(args[0],args);
    }
    else
    {
      if(p!=0) close(new_fd[(p-1)%2][0]);
      if(piped[p+1]!=NULL) close(new_fd[p%2][1]);
      if(checK==1) waitpid(pid,&some,WNOHANG);
      else wait(NULL);
    }
  }
  return;
}

void Jobs(char** args){
  int i,j;
  for(i=0;args[i]!=NULL;i++);
  if(i!=1) { printf("Terminal: Correct----- jobs"); return; }
  update_process();
  //int some,req;
  for(i=0;i<total_pid[0];i++)
  {
    if(process[i].check == 0) printf("[%d] Running %s [%d]\n",i+1,process[i].command,process[i].pids);
    if(process[i].check == 1) printf("[%d] Stopped %s [%d]\n",i+1,process[i].command,process[i].pids);
  }
}

void Foregnd(char** args)
{
  //update_process();
  int i=0;
  for(i=0;args[i]!=NULL;i++);
  if(i!=2) {
    printf("Terminal: correct method - fg [no]\n");
    return;
  }
  int req = atoi(args[1]);
  if(req<1 || req > total_pid[0]){
    printf("Such background no doesnot exists\n");
    return;
  }
  int some;
  procid = process[req-1].pids;
  strcat(frontend.command,process[total_pid[0]].command);
  frontend.pids =  process[total_pid[0]].pids;
  //signal(SIGTSTP,ctrl_z_handler); //int pid = fork();
  //if(pid==0) waitpid(process[req-1].pids,&some,WNOHANG); 
  //if(pid>0) 
  waitpid(process[req-1].pids,&some,WUNTRACED);
  return;
}

void Backgnd(char** args)
{
  int i=0;
  for(i=0;args[i]!=NULL;i++);
  if(i!=2) {
    printf("Terminal: correct method - bg [no]\n");
    return;
  }
  int req = atoi(args[1]);
  if(req<1 || req > total_pid[0]){
    printf("Such background no doesnot exists\n");
    return;
  }
  process[req-1].check = 0;
  //signal(SIGINT,childSignalHandler);
  kill(process[req-1].pids,SIGCONT);
  return;
}

void killAll(char** args)
{
  int i=0;
  for(i=0;args[i]!=NULL;i++);
  if(i!=1) {
    printf("Terminal: correct method - overkill \n");
    return;
  }
  for(i=0;i<total_pid[0];i++){
      kill(process[i].pids,9);
  }
  return;
}



void othercommands(char** c_args,int check)
{
  if(check) signal(SIGCHLD,childSignalHandler);
  int pid = fork(); int some = 0,j;
  if(pid < 0) { perror("Terminal"); }
  else if(pid > 0)
  {
    char *temp; int i;
    temp = (char *) malloc(1000 * sizeof(char));
    if(check)
    {
      for(i=0;c_args[i]!=NULL;i++) { strcat(temp,c_args[i]); strcat(temp," "); }
      strcat(process[total_pid[0]].command,temp);
      process[total_pid[0]].pids = pid;
      total_pid[0]++;
      waitpid(pid,&some,WNOHANG);
    }
    else
    {
      procid = pid;
      for(i=0;c_args[i]!=NULL;i++) { strcat(temp,c_args[i]); strcat(temp," "); }
      strcpy(frontend.command,"");
      strcat(frontend.command,temp);
      frontend.pids = pid;
      waitpid(pid,&some,WUNTRACED);
    }
  }
  else
  {
    //printf("child-----%d",getpgid(0));
    setpgid(0,0);
    for(j=0;c_args[j]!=NULL;j++)
    {
      if(!strcmp(c_args[j],">")){ redir_output(c_args[j+1],1); c_args[j]=NULL; }
      else if(!strcmp(c_args[j],">>")){  redir_output(c_args[j+1],2); c_args[j]=NULL; }
      else if(!strcmp(c_args[j],"<")){  redir_input(c_args[j+1]); c_args[j]=NULL; }
    }
    if(execvp(c_args[0],c_args)==-1) {  printf("\n"); perror("Terminal"); exit(1);}
  }
  return;
}
