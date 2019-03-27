
//Including all libraries
#include "headers.h"
//include ls,jobs,setenv,.....
#include "functions.h"
//Include redir and other commands
#include "redir_pipe.h"

int main()
{
  char inputl[1000],prev[1007];//,outFile[1007];
  char* getr = (char*)malloc(1007 * sizeof(char));
  char **commands,**arguments,**piped;
  int sub=0,check=0,check_redirect=0;//,some = 0;
  int com=0,p=0; //int new_fds[100][2];
  int count = 0,i,j;
  size_t sizeb = 1007;
  //int pipe_pid;
  int c_p=0;
  bool check_amp = 0;
  //int some;//,some1;
  procid = -1;
  for(i=0;i<1000;i++) { process[i].pids = 0; process[i].check = 0; }
  while(1)
  {
    update_process();
    signal(SIGINT,ctrl_c_handler);
    signal(SIGTSTP,ctrl_z_handler);
    /*----------------------Display---------------------------*/
    if(count == 0){ if(getcwd(home,1007)==NULL) perror("Terminal"); count = 1; }
    if(getcwd(&pwdir[0],1007)==NULL) perror("Terminal");
    strcpy(send,home);
    createTerminal(pwdir,send);

    /*-------------------taking input and tokenzing them------------------*/
    getline(&getr,&sizeb,stdin);
    strcpy(inputl,getr);
    inputl[strlen(getr)-1]='\0';
    commands = partsof(inputl,";");

    /*-------------------for multiple commands using ";" as delimter ---------*/

    for(com=0;commands[com]!=NULL;com++)
    {
      piped = partsof(commands[com],"|"); c_p=0;
      for(p=0;piped[p]!=NULL;p++);
      c_p=p;
      if(c_p>1) redirect_pipe(piped);
      else
      {
          arguments = partsof(piped[0],"\t' '\v");
          for(j=0;arguments[j]!=NULL;j++)
          {
            if(!strcmp(arguments[j],">") || !strcmp(arguments[j],">>") || !strcmp(arguments[j],"<")) { check_redirect = 1; }
          }
            /*----------------------------cd----------------------*/
          if(!strcmp(arguments[0],"cd") && check_redirect!=1 && c_p<2)
          {
            if(arguments[1]!=NULL)
            {
              if(arguments[1][0]=='~') { arguments[1] = strcat(send,&arguments[1][1]); }
              /*---------------previous Directory------------------*/
              else if(!strcmp(arguments[1],"-")){ if(sub) arguments[1] = prev; else { check = 2; printf("Previous Directory Not set Yet\n"); } }
              if(check!=2) check = changeDir(arguments[1]);
            }
            else check = changeDir(home);
            if(check == 1) { strcpy(prev,pwdir); sub=1; } check = 0;
          }

          /*------------------------pwd----------------------*/
          else if(!strcmp(arguments[0],"pwd") && check_redirect!=1 && c_p<2){ printf("%s\n",pwdir); }

          /*------------------------echo----------------------*/
          else if(!strcmp(arguments[0],"echo") && check_redirect!=1 && c_p<2) { if(arguments[1]!=NULL) for(i=1;arguments[i]!=NULL;i++) {  printf("%s ",arguments[i]); } printf("\n"); }

          /* ------------------------ls command------------------------------*/
          else if(!strcmp(arguments[0],"ls") && check_redirect!=1 && c_p<2)  {  ls(arguments,pwdir); }


          /*--------------pinfo-------------*/
          else if(!strcmp(arguments[0],"pinfo") && check_redirect!=1 && c_p<2) { pinfo(arguments[1],1); }

          /*--------------Remaindme---------*/
          else if(!strcmp(arguments[0],"remaindme")) { remindme(arguments); }
          /*--------------Setenv------------*/
          else if(!strcmp(arguments[0],"setenv")) { Setenv(arguments); }
          /*--------------Setenv------------*/
          else if(!strcmp(arguments[0],"unsetenv")) { Unsetenv(arguments); }
          /*---------------jobs-------------*/
          else if(!strcmp(arguments[0],"jobs")) { Jobs(arguments); }
          /*------------kjob-----------------*/
          else if(!strcmp(arguments[0],"kjob")) { KJobs(arguments); }
          /*-------------fg-------------------*/
          else if(!strcmp(arguments[0],"fg"))  { Foregnd(arguments); }
          /*-------------bg-------------------*/
          else if(!strcmp(arguments[0],"bg"))  { Backgnd(arguments); }
          /*-------------overkill-------------*/
          else if(!strcmp(arguments[0],"overkill")) { killAll(arguments); }
          /*------other builtin commands-------*/
          else if(!strcmp(arguments[0],"quit")) { killAll(arguments); exit(0); }
          else
          {
            for(i=0;arguments[i]!=NULL;i++); //printf("%s\n",arguments[i]);
            if(arguments[i-1][strlen(arguments[i-1])-1]=='&')
            {
              check_amp=1;
              if(strlen(arguments[i-1])==1) arguments[i-1]=NULL;
              else arguments[i-1][strlen(arguments[i-1])-1]='\0';
            }
            othercommands(arguments,check_amp); check_amp = 0;
          }
          check_redirect=0;
      }
    }
}
  return 0;
}
