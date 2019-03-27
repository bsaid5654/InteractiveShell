#include "headers.h"

void createTerminal(char *dir,char *home)
{
  char hn[1007],un[1007];
  struct passwd *id; int i;
  gethostname(hn,1007);
  id = getpwuid(getuid());
  //Username
  strcpy(un,id->pw_name);
  //printing the prompt
  //printf("x\n");
  if(strstr(dir,home)==NULL) printf("<%s@%s:%s>",un,hn,dir);
  //if home directory
  else if(strcmp(dir,home)==0)printf("<%s@%s:~>",un,hn);
  //if not subdirectory
  else{ for(i=0;home[i]==dir[i];i++); printf("<%s@%s:~%s>",un,hn,&dir[i]);}
  return;
}

//getting the arguments of the commands.......

char **partsof(char* line,char* del)
{
  int i = 0; char **part = (char **) malloc(1000 * sizeof(char*)); char * pointer;
  pointer = strtok(line,del);
  for(i=0;pointer!=NULL;i++) { part[i] = pointer; pointer  = strtok(NULL,del);}
  part[i]=NULL; return part;
}

//changing the directory.......

int changeDir(char* dir)
{
  //printf("one\n");
  if(chdir(dir)){ perror("terminal"); return 0;}
  else return 1;
}


//listing the directories and files in long list form

void Ls(char *file)
{
  struct stat file_data;
  stat(file,&file_data);
  char time_m[201];
  strftime(time_m, 201, "%h %d %H:%M", localtime(&(file_data.st_ctime)));
  printf( (S_ISDIR(file_data.st_mode)) ? "d" : "-");
  printf( (file_data.st_mode & S_IRUSR) ? "r" : "-");
  printf( (file_data.st_mode & S_IWUSR) ? "w" : "-");
  printf( (file_data.st_mode & S_IXUSR) ? "x" : "-");
  printf( (file_data.st_mode & S_IRGRP) ? "r" : "-");
  printf( (file_data.st_mode & S_IWGRP) ? "w" : "-");
  printf( (file_data.st_mode & S_IXGRP) ? "x" : "-");
  printf( (file_data.st_mode & S_IROTH) ? "r" : "-");
  printf( (file_data.st_mode & S_IWOTH) ? "w" : "-");
  printf( (file_data.st_mode & S_IXOTH) ? "x " : "- ");
  printf("%ld ",file_data.st_nlink);
  printf("%s\t",getpwuid(file_data.st_uid)->pw_name);
  printf("%s\t",getpwuid(file_data.st_gid)->pw_name);
  printf("%ld\t",file_data.st_size);
  //printf("%ld ",file_data.st_mtime);
  printf("%s\t",time_m);
  printf("%s\n",file);
}


//ls command
void ls(char **arguments,char* pwdir)
{
  DIR *dir; //int pid;
  struct stat file_stat;
  int check_ls=0;
  struct dirent *read_dir; int i;

  /*-------------------------ls-----------------*/
  if(arguments[1]==NULL)
  {
    dir = opendir(".");
    if(dir!=NULL)while((read_dir=readdir(dir))!=NULL) { if(read_dir->d_name[0]!='.')  printf("%s\n",read_dir->d_name); }
    else perror("Terminal");
  }

  /*------------------------ls -a [arguments]-------------------*/
  else if(!strcmp(arguments[1],"-a"))
  {
    if(arguments[2]==NULL) { dir = opendir(pwdir); while((read_dir=readdir(dir))!=NULL) { printf("%s\n",read_dir->d_name); } }
    else if(strcmp(arguments[2],"-l"))
    {
      for(i=0;arguments[2+i]!=NULL;i++)
      {
        dir = opendir(arguments[2+i]);
        if(dir!=NULL) { read_dir=readdir(dir); while(read_dir!=NULL) { printf("%s\n",read_dir->d_name); read_dir=readdir(dir);} }
        else perror("Terminal");
      }
    }
  }

  /*------------------------ls -l [arguments]-------------------*/
  else if(!strcmp(arguments[1],"-l"))
  {
    if(arguments[2]==NULL)
    {
      dir  = opendir(pwdir) ; check_ls=1;
      if(dir!=NULL) { read_dir=readdir(dir); while((read_dir)!=NULL) { if(read_dir->d_name[0]!='.') Ls(read_dir->d_name); read_dir=readdir(dir);} }
      else perror("Terminal");
    }
    if(!check_ls && strcmp(arguments[2],"-a"))
    {
    /*---------------------for multiple arguments---------------------*/
      for(i=2;arguments[i]!=NULL;i++)
      {
        check_ls =  stat(arguments[i],&file_stat) + 1;
        if (check_ls)
        {
          if(S_ISDIR(file_stat.st_mode)) { dir = opendir(arguments[i]); check_ls = 1; }
          else { Ls(arguments[i]); check_ls=0; }
        }
        else perror("Terminal");
        if(check_ls) while((read_dir=readdir(dir))!=NULL) { if(read_dir->d_name[0]!='.') Ls(read_dir->d_name); }
      }
    }
  }
  else
  {
    if(strcmp(arguments[1],"-la") && strcmp(arguments[1],"-al"))
    for(i=0;arguments[1+i]!=NULL;i++)
    {
      dir = opendir(arguments[1+i]);
      if(dir!=NULL) { while((read_dir=readdir(dir))!=NULL) { if(read_dir->d_name[0]!='.')  printf("%s\n",read_dir->d_name); } }
      else { perror("Terminal1"); }
    }
  }

    /*------------------------ls [-la/-al/-l -a] [arguments]------------------------*/
if(arguments[1]!=NULL)
{
    if(!strcmp(arguments[1],"-la") || !strcmp(arguments[1],"-al") || (arguments[2]!=NULL && (!strcmp(arguments[1],"-l") && !strcmp(arguments[2],"-a"))) || (arguments[2]!=NULL &&  (!strcmp(arguments[1],"-a") && !strcmp(arguments[2],"-l"))))
    {
      if((!strcmp(arguments[1],"-l") && !strcmp(arguments[2],"-a")) || (!strcmp(arguments[1],"-a") && !strcmp(arguments[2],"-l")))
      {
        for(i=0;arguments[3+i]!=NULL;i++) arguments[2+i] = arguments[3+i]; arguments[2+i] = NULL;
      }
      if(arguments[2]==NULL) { dir  = opendir(pwdir) ; check_ls=1; while((read_dir=readdir(dir))!=NULL) { Ls(read_dir->d_name); } }
      if(!check_ls)
      {
      /*---------------------for multiple arguments---------------------*/
        for(i=0;arguments[2+i]!=NULL;i++)
        {
          check_ls =  stat(arguments[2+i],&file_stat) + 1;
          if (check_ls)
          {
            if(S_ISDIR(file_stat.st_mode)) { dir = opendir(arguments[2+i]); if(dir!=NULL) check_ls = 1; else { check_ls = 0; perror("Terminal"); } }
            else { Ls(arguments[2+i]); check_ls=0; }
          }
          else perror("terminal"); //printf("Cannot access %s\n",arguments[2+i]); }
          if(check_ls) while((read_dir=readdir(dir))!=NULL) { Ls(read_dir->d_name); }
        }
      }
    }
  }

}

//info of the process ........

char* pinfo(char* pin,int checK)
{
  char* store; char a[8]=":\n' '\t\v";
  char* pid = (char*)malloc(10*sizeof(char));
  char* read_fd = (char*)malloc(3000*sizeof(char));
  char **arguments;
  store = read_fd;
  if(pin==NULL) { sprintf(pid,"%d",getpid()); }
  else{ strcpy(pid,pin); }
  char start[10] = "/proc/"; char end1[10] = "/status"; char end2[10] = "/exe";
  char* final = (char*)malloc(101*sizeof(char)); final[0]='\0';
  char* _exec = (char*)malloc(101*sizeof(char)); _exec[0]='\0';
  char* path = (char*)malloc(101*sizeof(char)); path[0]='\0';
  strcat(final,start); strcat(final,pid);
  strcat(_exec,final);
  strcat(final,end1); strcat(_exec,end2);
  int fd = open(final,O_RDONLY)+1;
  if(!fd)
  {
    if(checK == 1) printf("The process with the pid %s mentioned doesnot exist\n",pid);
    return "N";
  }
  int check = readlink(_exec,path,100)+1;
  if(!check) path[check-1] = '\0'; fd -= 1;
  while(read(fd,read_fd,1)) read_fd+=1;
  arguments = partsof(store,a);
  if(checK == 1)
  {  
    printf("pid---%s\n",pid);
    printf("Process Status ---- %s\n",arguments[5]);
    printf("- %s{Virtual Memory}\n",arguments[50]);
    printf("Executable Path-"); printf("%s\n",check?path:"No path");
  }
  return arguments[5];
}

//KJobs
void KJobs(char **args){
  int i;
  for(i=0;args[i]!=NULL;i++);
  if(i!=3) { printf("Terminal: correct command -- kjob [jobNo] [signalNo]"); return; }
  int arg1=atoi(args[1]); int arg2=atoi(args[2]);
  if(arg1 > total_pid[0]) { printf("Such Process doesnot exist\n"); return; }
  arg1 = process[arg1-1].pids;
  if(!(kill(arg1,arg2)+1)) { perror("Terminal"); }
}


//for all other commands..........
void Setenv(char **args){
  int i; for(i=0;args[i]!=NULL;i++);
  if(i!=3){
    printf("Terminal: correct method - setenv [key] [value]\n");
  }
  else{
    if(!(setenv(args[1],args[2],1)+1))perror("Terminal");
  }
}

void Unsetenv(char **args){
  int i; for(i=0;args[i]!=NULL;i++);
  if(i!=2){
    printf("Terminal: correct method - unsetenv [key]\n");
  }
  else{
    if(!(unsetenv(args[1])+1)) perror("Terminal");
  }
}

//void Jobs(char **args){

//}
//remainder for every given interval.........

void remindme(char **args)
{
  int timer = 0,check=0,som=0;
  for(int i=0;i<(int)strlen(args[1]);i++) if(args[1][i]-'0'>9) check = 1;
  if(check!=1)
  {
  for(int i=0;i<(int)strlen(args[1]);i++) timer = 10*timer + (args[1][i]-'0'); //printf("%d\n",timer);
  int pid = fork();
  if(pid < 0) { perror("Terminal"); }
  else if(pid == 0) { while(1) { if(sleep(timer)) { perror("Terminal");} printf("\n----\n"); printf("Remainder: %s\n",args[2]); }}
  else { waitpid(pid,&som,WNOHANG); }//printf("---%d--\n",pid); }
  }
  else printf("Usage:remaindme [time(int)] [remainder]\n");

}
