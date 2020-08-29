#include "../../header.h"

int pwd_(int argc, char *argv[]){
	int opt;
	char dir[PATH_MAX];
	
	while((opt = getopt(argc, argv, "")) !=- 1){
		switch(opt){
			case '?':
				return -1;
			default:
				break;
		}
	}
	
	if(argc>1){
		fprintf(stderr, "pwd: too many arguments\n");
		return -1;
	}

	if(getcwd(dir, PATH_MAX)==NULL)
		return -2;

	printf("%s\n", dir);
	return -1;


}
