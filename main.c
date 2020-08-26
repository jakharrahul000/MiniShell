#include <sys/capability.h>
#include "header.h"

static char *read_line = (char *)NULL;

void initialize(){
  char *path;

	/* Changing the environment variables */
	path = malloc(512*sizeof(char));
	if(path==NULL)
		errExit("malloc");
	snprintf(path, 512*sizeof(char), "%s", "usr/bin:bin");
	if(setenv("PATH", path, 1)!=0)
		errExit("setenv");

  /* Setting commands history environment */
	using_history();
}

int main(int argc, char *argv[]){

  initialize();

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
					printf("%s %s\n", pipe[0], pipe[1]);
				}else if(ou!=NULL){
					printf("%s %s\n", ou[0], ou[1]);
				}else if(in!=NULL){
					printf("%s %s\n", in[0], in[1]);
				}else{
					status = executeCommand(line);
					
					if(status==-3 || status==-1)
						continue;
					if(status==-2){
						errMsg("command cannot be executed, some error occured\n");
						continue;
					}
				}
			}

    }
  
  }
  
  

}
