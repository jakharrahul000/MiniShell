#include "../../header.h"
#include <time.h>
#include <utime.h>

int flagn=0;
#define SIZE 1000

int main(int argc, char *argv[]){
	int opt, ind=1;
	int status, fd;
	FILE *file;
	char line[SIZE];

	while((opt = getopt(argc, argv, "n")) !=- 1){
		switch(opt){
			case 'n':
				flagn=1;
				break;
			default:
				fprintf(stderr, "usageErr: ls -[n] [name]\n");
				return -1;
		}
	}

	if(optind==argc){
			while(fgets(line, SIZE, stdin)!=NULL){
				if(flagn){
					printf("%6d  ", ind);
					ind++;
				}
				printf("%s", line);
			}
			if(errno!=0)
				return -2;
			return -1;
	}

	for(; optind<argc; optind++){
		fd = open(argv[optind], O_RDONLY);
		if(fd==-1){
			if(errno==ENOENT){
				printf("cat: %s: No such file or directory\n", argv[optind]);
				continue;
			}else return -2;
		}
		file = fdopen(fd, "r");
		if(file==NULL)
			return -2;

	  while(fgets(line, SIZE, file)!=NULL){
			if(flagn){
				printf("%6d  ", ind);
				ind++;
			}
			printf("%s", line);
		}
		if(errno!=0)
			return -2;

	}
	return -1;

}
