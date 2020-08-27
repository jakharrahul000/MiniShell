#include <sys/wait.h>
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

char *pathExist(char *cmd){
	char *path, *cmdpath, *PATH, **paths, *newPATH;
	int filestatus, ind = 0;
		
	if((PATH = getenv("PATH"))==NULL)
		return NULL;
	newPATH = malloc(512*sizeof(char));
	if(newPATH==NULL)
		return NULL;

	snprintf(newPATH, 512*sizeof(char), "%s", PATH);
	
	paths = parseEnv(newPATH);
	if(paths==NULL)
		return NULL;

	cmdpath = NULL;
	for(;;){
		path = paths[ind];
		ind++;
		if(path==NULL)
			break;
		
		filestatus = commandExist(path, cmd);
		if(filestatus==-2){
			return NULL;
		}
		if(filestatus==-1)
			continue;

		cmdpath = malloc(PATH_MAX);
		if(cmdpath==NULL)
			return NULL;
		
		snprintf(cmdpath, PATH_MAX, "%s/%s", path, cmd);
		return cmdpath;
	}
	
	fprintf(stdout, "command cannot be found: %s\n", cmd);
	errno=2;
	return NULL;
}

int executeCommand(char *command){
	char **cmd, *cmdpath;
	pid_t pid, cpid;
	int status;

	command = removeWhiteSpaces(command);
	if(command==NULL)
		return -3;
	
	cmd = parseCommand(command);
	if(cmd==NULL)
		return -2;
		
	cmdpath = pathExist(cmd[0]);
	if(cmdpath==NULL){
		if(errno==2)
			return -1;
		return -2;
	}

	switch((pid=fork())){
		case -1:
			return -2;
		case 0:
			execv(cmdpath, cmd);
			errMsg("execv");
			return -2;
		default:
			cpid = waitpid(pid, &status, 0);
			if(cpid==-1)
				return -2;
			return -3;
	}

}


int executePipeCommands(char *command1, char *command2){
	char **cmd1, *cmdpath1, **cmd2, *cmdpath2;
	pid_t pid1, pid2, cpid1, cpid2;
	int pfd[2], status;

	command1 = removeWhiteSpaces(command1);
	if(command1==NULL)
		return -3;
	command2 = removeWhiteSpaces(command2);
	if(command2==NULL)
		return -3;

	cmd1 = parseCommand(command1);
	if(cmd1==NULL)
		return -2;
	cmd2 = parseCommand(command2);
	if(cmd2==NULL)
		return -2;
		
	cmdpath1 = pathExist(cmd1[0]);
	if(cmdpath1==NULL){
		if(errno==2)
			return -1;
		return -2;
	}
	cmdpath2 = pathExist(cmd2[0]);
	if(cmdpath2==NULL){
		if(errno==2)
			return -1;
		return -2;
	}

	if(pipe(pfd)==-1)
		return -2;

	switch((pid1=fork())){
		case -1:
			return -2;
		case 0:
			if(close(pfd[0])==-1)
				return -2;
			if(STDOUT_FILENO!=pfd[1]){
				if(dup2(pfd[1], STDOUT_FILENO)==-1)
					return -2;
				if(close(pfd[1])==-1)
					return -2;
			}
			execv(cmdpath1, cmd1);
			errMsg("execv");
			return -2;
		default:
			break;
	}	

	switch((pid2=fork())){
		case -1:
			return -2;
		case 0:
			if(close(pfd[1])==-1)
				return -2;
			if(STDIN_FILENO!=pfd[0]){
				if(dup2(pfd[0], STDIN_FILENO)==-1)
					return -2;
				if(close(pfd[0])==-1)
					return -2;
			}
			execv(cmdpath2, cmd2);
			errMsg("execv");
			return -2;
		default:
			break;
	}

	if(close(pfd[0])==-1)
		return -2;
	if(close(pfd[1])==-1)
		return -2;

	cpid1 = waitpid(pid1, &status, 0);
	if(cpid1==-1)
		return -2;
	cpid2 = waitpid(pid2, &status, 0);
	if(cpid2==-1)
		return -2;
	
	return -3;

}



int executeOutputCommand(char *command, char *filename){
	char **cmd, *cmdpath;
	pid_t pid, cpid;
	int fd, status;

	command = removeWhiteSpaces(command);
	if(command==NULL)
		return -3;
	
	filename = removeWhiteSpaces(filename);
	if(filename==NULL){
		fprintf(stdout, "%s", "parse error near '\\n'\n");
		return -3;
	}
	
	cmd = parseCommand(command);
	if(cmd==NULL)
		return -2;
		
	cmdpath = pathExist(cmd[0]);
	if(cmdpath==NULL){
		if(errno==2)
			return -1;
		return -2;
	}
	
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
	if(fd==-1)
		return -2;
	
	switch((pid=fork())){
		case -1:
			return -2;
		case 0:
			if(STDOUT_FILENO!=fd){
				if(dup2(fd, STDOUT_FILENO)==-1)
					return -2;
				if(close(fd)==-1)
					return -2;
			}
			execv(cmdpath, cmd);
			errMsg("execv");
			return -2;
		default:
			break;
	}	

	cpid = waitpid(pid, &status, 0);
	if(cpid==-1)
		return -2;
	
	return -3;

}


int executeInputCommand(char *command, char *filename){
	char **cmd, *cmdpath;
	pid_t pid, cpid;
	int fd, status;

	command = removeWhiteSpaces(command);
	if(command==NULL)
		return -3;
	
	filename = removeWhiteSpaces(filename);
	if(filename==NULL){
		fprintf(stdout, "%s", "parse error near '\\n'\n");
		return -3;
	}
	
	cmd = parseCommand(command);
	if(cmd==NULL)
		return -2;
		
	cmdpath = pathExist(cmd[0]);
	if(cmdpath==NULL){
		if(errno==2)
			return -1;
		return -2;
	}
	
	fd = open(filename, O_RDONLY);
	if(fd==-1)
		return -2;
	
	switch((pid=fork())){
		case -1:
			return -2;
		case 0:
			if(STDIN_FILENO!=fd){
				if(dup2(fd, STDIN_FILENO)==-1)
					return -2;
				if(close(fd)==-1)
					return -2;
			}
			execv(cmdpath, cmd);
			errMsg("execv");
			return -2;
		default:
			break;
	}	

	cpid = waitpid(pid, &status, 0);
	if(cpid==-1)
		return -2;
	
	return -3;

}
