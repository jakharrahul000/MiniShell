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

int umask_(int argc, char *argv[]){
	int opt, len, usr, grp, oth;
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
	
	mode = 0;
	oth = arg[len-1] - '0';
	if(len>=2)grp = arg[len-2] - '0';
	if(len>=3)usr = arg[len-3] - '0';

	// For user
	if(len>=3){
		if(usr==1)mode|=(S_IXUSR);
	  if(usr==2)mode|=(S_IWUSR);
	  if(usr==3)mode|=(S_IXUSR | S_IWUSR);
	  if(usr==4)mode|=(S_IRUSR);
	  if(usr==5)mode|=(S_IXUSR | S_IRUSR);
	  if(usr==6)mode|=(S_IWUSR | S_IRUSR);
	  if(usr==7)mode|=(S_IXUSR | S_IWUSR | S_IRUSR);
	}

	// For grp
	if(len>=2){
		if(grp==1)mode|=(S_IXGRP);
	  if(grp==2)mode|=(S_IWGRP);
	  if(grp==3)mode|=(S_IXGRP | S_IWGRP);
	  if(grp==4)mode|=(S_IRGRP);
	  if(grp==5)mode|=(S_IXGRP | S_IRGRP);
	  if(grp==6)mode|=(S_IWGRP | S_IRGRP);
	  if(grp==7)mode|=(S_IXGRP | S_IWGRP | S_IRGRP);
	}

	// For others
	if(oth==1)mode|=(S_IXOTH);
	if(oth==2)mode|=(S_IWOTH);
	if(oth==3)mode|=(S_IXOTH | S_IWOTH);
	if(oth==4)mode|=(S_IROTH);
	if(oth==5)mode|=(S_IXOTH | S_IROTH);
	if(oth==6)mode|=(S_IWOTH | S_IROTH);
	if(oth==7)mode|=(S_IXOTH | S_IWOTH | S_IROTH);

	umask(mode);
	return -1;
}
