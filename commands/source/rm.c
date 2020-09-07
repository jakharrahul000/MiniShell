#include "../../header.h"

int flagf=0, flagi=0, flagd=0, flagv=0;

int main(int argc, char *argv[]){
	int opt, status, ok;
	char *filename, ask[50];

	while((opt = getopt(argc, argv, "fidv")) !=- 1){
		switch(opt){
			case 'f':
				flagf=1;
				break;
			case 'i':
				flagi=1;
				break;
			case 'd':
				flagd=1;
				break;
			case 'v':
				flagv=1;
				break;
			default:
				fprintf(stderr, "usageErr: ls -[fidv] [name]\n");
				return -1;
		}
	}

	if(optind==argc){
		fprintf(stderr, "rm: missing file operand");
		return -1;
	}

	for(; optind<argc; optind++){
		filename = argv[optind];
		ok = 1;

		if(flagi){
			printf("do you want to remove '%s'?: ", filename);
			scanf("%s", ask);
			if(strcmp(ask, "yes")==0 || strcmp(ask, "YES")==0 || strcmp(ask, "Yes")==0 || strcmp(ask, "Y")==0 || strcmp(ask, "y")==0)
				ok=1;
			else
				ok=0;
		}
		if(!ok)continue;
		status = remove(filename);
		if(status==-1){
			if(errno==ENOENT && !flagf){
				fprintf(stderr, "rm: cannot remove '%s': No such file or directory\n", filename);
				continue;
			}
			return -2;
		}
		if(flagv){
			printf("removed '%s'\n", filename);
		}

	}

	return -1;


}
