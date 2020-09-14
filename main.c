#include <sys/capability.h>
#include "header.h"

static char *read_line = (char *)NULL;

static void signal_handler(int sig){
	printf("\e[0;32m\n$ \e[0m");
}

void initialize(char *name){
  struct sigaction sa;
	char *path;
	pid_t pid;

	/* Changing the environment variables */
	path = malloc(512*sizeof(char));
	if(path==NULL)
		errExit("malloc");
	snprintf(path, 512*sizeof(char), "%s", "usr/bin:bin");
	if(setenv("PATH", path, 1)!=0)
		errExit("setenv");

  /* Setting commands history environment */
	using_history();

	/* Handling signals */
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if(sigaction(SIGINT, &sa, NULL)==-1)
		errExit("sigaction");
	sa.sa_handler = SIG_IGN;
	if(sigaction(SIGQUIT, &sa, NULL)==-1)
		errExit("sigaction");
	if(sigaction(SIGTSTP, &sa, NULL)==-1)
		errExit("sigaction");

	/* Droping privilages */
	if(seteuid(getuid())==-1)
		errExit("seteuid");

	/* Set timer */
	timer = 0;
	
	printf("\033[2J");
	printf("\033[H");
	pid = getpid();
	printf("==%ld== This is a MiniShell currently in development.\n", (long)pid);
	printf("==%ld== Copyright (C) 2020-2033, and GNU GPL'd, by Rahul Jakhar et al.\n", (long)pid);
	printf("==%ld== Memcheck: No memory leak detected(actually it checked nothing).\n", (long)pid);
	printf("==%ld== Command: %s\n", (long)pid, name);
}

int main(int argc, char *argv[]){

  initialize(argv[0]);

  for(;;){
    if(read_line){
      free(read_line);
      read_line = (char *)NULL;
    }
    read_line = readline("\e[0;32m$ \e[0m");
    
    if(read_line == (char *)NULL)
      continue;
    if(read_line && *read_line){
      add_history(read_line);

			char **commands = parseCommands(read_line);
			int ind = 0;			
			for(;;){
				char *line = commands[ind];
				int status;

				if(line==NULL)
					break;
				ind++;

				char **pipe = parsePipe(line);
				char **ou = parseOutputRedirection(line);
				char **in = parseInputRedirection(line);


				if(pipe!=NULL){
					if(pipe[1]==NULL){
						fprintf(stdout, "%s", "parse error near '\\n'\n");
						continue;
					}
					status = executePipeCommands(pipe[0], pipe[1]);
				}else if(ou!=NULL){
					if(ou[1]==NULL){
						fprintf(stdout, "%s", "parse error near '\\n'\n");
						continue;
					}
					status = executeOutputCommand(ou[0], ou[1]);
				}else if(in!=NULL){
					if(in[1]==NULL){
						fprintf(stdout, "%s", "parse error near '\\n'\n");
						continue;
					}
					status = executeInputCommand(in[0], in[1]);
				}else{
					status = executeCommand(line);
				}
				
				if(status==-3 || status==-1)
					continue;
				if(status==-2){
					fprintf(stderr, "%s", "command cannot be executed, some error occured\n");
					continue;
				}
			}

    }
  
  }
  
  

}
