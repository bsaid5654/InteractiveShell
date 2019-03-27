# InteractiveShell
a user-interactive shell in C,implementing the major features of GNU/Linux shelllike redirection, piping, handling background processes.

# RUN
```shell
make  
./terminal
```  

# functions.c
-->Contains the functions like ls,etc

# main.c
-->Contains the main code

# redir_pipe.c  
--->Contains some of the functions required

# redir_pipe.h
--->Contains function headers 

# headers.h
-->Contains the header of libraries to be included

# functions.h
-->Contains the headers of functions to be included in main.c

# inbuild functions used
-->_gethostname_
-->_getpwuid_
-->_getuid_
-->_printf_
-->_getline_
-->_strstr_
-->_strcpy_
-->_strcat_
-->_strlen_
-->_strtok_
-->_getcwd_
-->_strcmp_
-->_chdir_
-->_stat_
-->**strftime**
-->**localtime**
-->_opendir_
-->_readdir_
-->_readlink_
-->_perror_
-->**sprintf**
-->_malloc_
-->**signal**
-->_fork_
-->_waitpid_
-->_execvp_
-->**sleep**
-->_read_
-->_open_
