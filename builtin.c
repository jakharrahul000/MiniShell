#include "header.h"
#include "builtIn.c.inc"

int checkBuiltin(char *cmd){
	for(int i=0;i<len;i++){
		if(strcmp(cmd, builtIn[i])==0 && strlen(cmd)==strlen(builtIn[i])){
			return i;
		}
	}
	return -1;
}


int executeBuiltin(int ind, char **cmd){
	int argc=0;

	while(cmd[argc]!=NULL)
		argc++;

	switch(ind){
		case 0:
			break;
			//return cd_(argc, cmd);
		case 1:
			//return echo_(argc, cmd);
			break;
		case 2:
			exit(EXIT_SUCCESS);
		case 3:
			//return help_(argc, cmd);
			break;
		case 4:
			//return jobs_(argc, cmd);
			break;
		case 5:
			//return kill_(argc, cmd);
			break;
		case 6:
			//return printf_(argc, cmd);
			break;
		case 7:
			return pwd_(argc, cmd);
		case 8:
			//return umask_(argc, cmd);
			break;
		case 9:
			printf("\033[2J");
			printf("\033[H");
			return -1;
		default:
			return -2;
	}	
}
