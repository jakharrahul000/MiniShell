#include "../../header.h"

void printMask(mode_t perm){
	int usr, grp, oth;

	usr=0;
	grp=0;
	oth=0;
	
	if(perm & S_IRUSR)usr+=4;
	if(perm & S_IWUSR)usr+=2;
	if(perm & S_IXUSR)usr+=1;
	if(perm & S_IRGRP)grp+=4;
	if(perm & S_IWGRP)grp+=2;
	if(perm & S_IXGRP)grp+=1;
	if(perm & S_IROTH)oth+=4;
	if(perm & S_IWOTH)oth+=2;
	if(perm & S_IXOTH)oth+=1;
	
	if(usr)printf("0");
	printf("%d%d%d\n", usr, grp, oth);

}

int main(int argc, char *argv[]){
	int opt, len;
	mode_t mode;
	char *arg;

	while((opt = getopt(argc, argv, "")) !=- 1){
		switch(opt){
			case '?':
				return -1;
			default:
				break;
		}
	}
	
	if(argc>2){
		fprintf(stderr, "pwd: too many arguments\n");
		return -1;
	}
	
	if(argc==1){
		mode = umask(0);
		printMask(mode);
		umask(mode);
		return -1;
	}

	arg = argv[1];
	len = strlen(arg);

	for(int i=0;i<len;i++){
		if(!(arg[i]>='0' && arg[i]<='9')){
			fprintf(stderr, "umask: bad symbolic mode operator: %c\n", arg[i]);
			return -1;
		}
	}

	for(int i=0;i<len;i++){
		if(!(arg[i]>='0' && arg[i]<='7')){
			fprintf(stderr, "umask: bad mask\n");
			return -1;
		}
	}



}
