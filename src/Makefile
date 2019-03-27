#Program Executable file

terminal: main.o functions.o redir_pipe.o
	gcc functions.o main.o redir_pipe.o -o terminal

function.o: headers.h  functions.c
	gcc functions.c -c -o functions.o

redir_pipe.o: headers.h functions.h redir_pipe.c
	gcc redir_pipe.c -c -o redir_pipe.o

main.o:headers.h redir_pipe.h functions.h main.c
	gcc main.c -c -o main.o
