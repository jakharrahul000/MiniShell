#include "header.h"

char **parseEnv(char *PATH){
	char **paths, *token;
	int ind = 0;

	paths = malloc(512*sizeof(char *));
	if(paths==NULL)
		errMsg("Env: couldn't parse the input\n");

	token = strtok(PATH, ":");
	while(token!=NULL){
		paths[ind] = token;
		token = strtok(NULL, ":");
		ind++;
	}

	return paths;
}

char **parseCommand(char *command){
	char **cmd, *token;
	int ind = 0;

	cmd = malloc(512*sizeof(char *));
	if(cmd==NULL)
		errMsg("Command: couldn't parse the input\n");

	token = strtok(command, " ");
	while(token!=NULL){
		cmd[ind] = token;
		token = strtok(NULL, " ");
		ind++;
	}
	cmd[ind]=NULL;

	return cmd;
}

char *removeWhiteSpaces(char *line){
	int len;
	
	while(line!=NULL && line[0]==' ')
		line++;
	
	if(strlen(line)==0)
		return NULL;
	
	len = strlen(line);
	for(;;){
		if(line[len-1]==' '){
			line[len-1]='\0';
			len--;
		}else break;
	}

	return line;
}

char **parseCommands(char *read_line){
	char **commands, *token;
	int ind = 0;

	commands = malloc(5*256*sizeof(char *));
	if(commands==NULL)
		errMsg("Commands: couldn't parse the input\n");

	token = strtok(read_line, ";");
	while(token!=NULL){
		commands[ind] = token;
		token = strtok(NULL, ";");
		ind++;
	}

	return commands;
}

char **parsePipe(char *read_line){
	char **commands, *line, *token;
	int ind = 0;

	line = malloc(strlen(read_line)+1);
	if(line==NULL)
		errMsg("Pipe: couldn't parse the input\n");
	
	snprintf(line, strlen(read_line)+1, "%s", read_line);

	token = strtok(read_line, "|");

	if(strlen(read_line)==strlen(line))
		return NULL;

	commands = malloc(256*sizeof(char *));
	if(commands==NULL)
		errMsg("Pipe: couldn't parse the input\n");

	while(token!=NULL){
		commands[ind] = token;
		token = strtok(NULL, "|");
		ind++;
	}

	return commands;
}

char **parseOutputRedirection(char *read_line){
	char **commands, *line, *token;
	int ind = 0;

	line = malloc(strlen(read_line)+1);
	if(line==NULL)
		errMsg("Output: couldn't parse the input\n");
	
	snprintf(line, strlen(read_line)+1, "%s", read_line);

	token = strtok(read_line, ">");

	if(strlen(read_line)==strlen(line))
		return NULL;

	commands = malloc(256*sizeof(char *));
	if(commands==NULL)
		errMsg("Output: couldn't parse the input\n");

	while(token!=NULL){
		commands[ind] = token;
		token = strtok(NULL, ">");
		ind++;
	}

	return commands;
}


char **parseInputRedirection(char *read_line){
	char **commands, *line, *token;
	int ind = 0;

	line = malloc(strlen(read_line)+1);
	if(line==NULL)
		errMsg("Input: couldn't parse the input\n");
	
	snprintf(line, strlen(read_line)+1, "%s", read_line);

	token = strtok(read_line, "<");

	if(strlen(read_line)==strlen(line))
		return NULL;

	commands = malloc(256*sizeof(char *));
	if(commands==NULL)
		errMsg("Input: couldn't parse the input\n");

	while(token!=NULL){
		commands[ind] = token;
		token = strtok(NULL, "<");
		ind++;
	}

	return commands;
}
