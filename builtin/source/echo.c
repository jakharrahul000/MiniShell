#include "../../header.h"

#define SIZE 200

int main(int argc, char *argv[]){
	int opt, ind, flagn, flage, flagE;
	char buff[SIZE], *arg, b[1];

	flagn = 0;
	flage = 1;
	flagE = 0;
	while((opt = getopt(argc, argv, "neE")) !=- 1){
		switch(opt){
			case 'n':
				flagn = 1;
				break;
			case 'e':
				flage = 1;
				break;
			case 'E':
				flagE = 1;
				break;
			case '?':
				printf("echo: invalid option -- '%c'\n", optopt);
				fprintf(stderr, "%s", "Usage: -[neE]\n");
			default:
				break;
		}
	}

	ind = 0;
	for(; optind<argc; optind++){
		arg = argv[optind];
		int len = strlen(arg);

		for(int i=0;i<len;i++){
			if(arg[i]=='\\'){
				printf("1=%d\n", i);
				if(flagE){
					if(write(STDOUT_FILENO, "\\", 1)==-1)
						return -2;
				}else{
					if(i==len-1){
						fprintf(stderr, "error parsing input near '\\n'\n");
						return -1;
					}else{
						if(arg[i+1]!='\\'&&arg[i+1]!='a'&&arg[i+1]!='b'&&
								arg[i+1]!='f'&&arg[i+1]!='n'&&arg[i+1]!='r'&&arg[i+1]!='t'&&arg[i+1]!='v')
							continue;
						i++;
						if(arg[i]=='\\')b[0]='\\';
						if(arg[i]=='a')b[0]='\a';
						if(arg[i]=='b')b[0]='\b';
						if(arg[i]=='f')b[0]='\f';
						if(arg[i]=='n')b[0]='\n';
						if(arg[i]=='r')b[0]='\r';
						if(arg[i]=='t')b[0]='\t';
						if(arg[i]=='v')b[0]='\v';
						if(write(STDOUT_FILENO, b, 2)==-1)
							return -2;
					}
				}
			}else if(arg[i]=='"'){
				printf("2=%d\n", i);
				continue;
			}else{
				printf("3=%d\n", i);
				b[0]=arg[i];
				if(write(STDOUT_FILENO, b, 1)==-1)
					return -2;
			}
		}
	}

	if(!flagn){
		printf("\n");
	}

}






