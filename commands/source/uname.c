#include "../../header.h"
#include <sys/utsname.h>

int flaga=0, flags=0, flagn=0, flagr=0, flagv=0, flagm=0;

int main(int argc, char *argv[]){
	int opt, status;
	struct utsname uts;

	while((opt = getopt(argc, argv, "asnrvm")) !=- 1){
		switch(opt){
			case 'a':
				flaga=1;
				break;
			case 's':
				flags=1;
				break;
			case 'n':
				flagn=1;
				break;
			case 'r':
				flagr=1;
				break;
			case 'v':
				flagv=1;
				break;
			case 'm':
				flagm=1;
				break;
			default:
				fprintf(stderr, "usageErr: uname -[asnrvm] [name]\n");
				return -1;
		}
	}
	
	if(argc>1&&flaga==0&&flags==0&&flagn==0&&flagr==0&&flagv==0&&flagm==0){
		fprintf(stderr, "uname: extra operand '%s'\n", argv[1]);
		return -1;
	}
	if(flaga==0&&flagn==0&&flagr==0&&flagv==0&&flagm==0)
		flags=1;

	if(uname(&uts)==-1)
		return -2;

	if(flags || flaga){
		printf("%s ", uts.sysname);
	}
	if(flagn || flaga){
		printf("%s ", uts.nodename);
	}
	if(flagr || flaga){
		printf("%s ", uts.release);
	}
	if(flagv || flaga){
		printf("%s ", uts.version);
	}
	if(flagm || flaga){
		printf("%s ", uts.machine);
	}
	printf("\n");
	return -1;


}
