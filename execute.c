#include "header.h"

int commandExist(char *path, char *cmd){
	char *filepath;
	int fd, savederrno;

	filepath = malloc(PATH_MAX);
	if(filepath==NULL)
		return -2;
	snprintf(filepath, PATH_MAX, "%s/%s", path, cmd);
	
	savederrno = errno;
	fd = open(filepath, O_RDONLY);
	if(fd==-1 && errno==ENOENT)
		return -1;

	free(filepath);
	return 0;
}

int executeCommand(char *command){
	char **cmd, *path, *cmdpath, *PATH, **paths, *newPATH;
	int filestatus, ind = 0;

	command = removeWhiteSpaces(command);
	if(command==NULL)
		return -3;
	
	cmd = parseCommand(command);
	if(cmd==NULL)
		return -2;
		
	if((PATH = getenv("PATH"))==NULL)
		return -2;
	newPATH = malloc(512*sizeof(char));
	if(newPATH==NULL)
		return -2;

	snprintf(newPATH, 512*sizeof(char), "%s", PATH);
	
	paths = parseEnv(newPATH);
	if(paths==NULL)
		return -2;

	cmdpath = NULL;
	for(;;){
		path = paths[ind];
		ind++;
		if(path==NULL)
			break;
		
		filestatus = commandExist(path, cmd[0]);
		if(filestatus==-2){
			return -2;
		}
		if(filestatus==-1)
			continue;

		cmdpath = malloc(PATH_MAX);
		if(cmdpath==NULL)
			return -2;
		
		snprintf(cmdpath, PATH_MAX, "%s/%s", path, cmd[0]);
		break;
	}

	if(cmdpath==NULL){
		fprintf(stdout, "command cannot be found: %s\n", cmd[0]);
		return -1;
	}

	execv(cmdpath, cmd);
	errExit("execv");
}
