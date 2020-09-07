#include "../../header.h"

int main(int argc, char *argv[]){
	int status;
	char *filename;

	if(optind==argc){
		fprintf(stderr, "mkdir: missing operand\n");
		return -1;
	}

	for(; optind<argc; optind++){
		filename = argv[optind];
	
		status = rmdir(filename);
		if(status==-1){
			if(errno==ENOENT){
				fprintf(stderr, "mkdir: cannot remove directory ‘%s’: No such file or directory\n", filename);
				continue;
			}
			return -2;
		}
	}

	return -1;


}
