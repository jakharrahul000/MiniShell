#include "../../header.h"

int flagm=0, flagv=0;
mode_t umode = S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH;

mode_t findMode(char *smode){
	int len, usr, grp, oth, setbit;
	mode_t mode;

	mode=0;
	len=strlen(smode);
	if(len>4){	
		fprintf(stderr, "mkdir: invalid mode '%s'\n", smode);
		return -1;
	}
	for(int i=0;i<len;i++){
		if(!(smode[i]>='0' && smode[i]<='7')){
			fprintf(stderr, "mkdir: invalid mode '%s'\n", smode);
			return -1;
		}
	}

	oth = smode[len-1]-'0';
	grp = smode[len-2]-'0';
	usr = smode[len-3]-'0';
	if(len>3)setbit = smode[len-4]-'0';
	else setbit = 0;
	

	if(usr==1)mode |= S_IXUSR;
	if(usr==2)mode |= S_IWUSR;
	if(usr==3)mode |= S_IWUSR | S_IXUSR;
	if(usr==4)mode |= S_IRUSR;
	if(usr==5)mode |= S_IXUSR | S_IRUSR;
	if(usr==6)mode |= S_IWUSR | S_IRUSR;
	if(usr==7)mode |= S_IXUSR | S_IWUSR | S_IRUSR;


	if(oth==1)mode |= S_IXOTH;
	if(oth==2)mode |= S_IWOTH;
	if(oth==3)mode |= S_IWOTH | S_IXOTH;
	if(oth==4)mode |= S_IROTH;
	if(oth==5)mode |= S_IXOTH | S_IROTH;
	if(oth==6)mode |= S_IWOTH | S_IROTH;
	if(oth==7)mode |= S_IXOTH | S_IWOTH | S_IROTH;

	
	if(grp==1)mode |= S_IXGRP;
	if(grp==2)mode |= S_IWGRP;
	if(grp==3)mode |= S_IWGRP | S_IXGRP;
	if(grp==4)mode |= S_IRGRP;
	if(grp==5)mode |= S_IXGRP | S_IRGRP;
	if(grp==6)mode |= S_IWGRP | S_IRGRP;
	if(grp==7)mode |= S_IXGRP | S_IWGRP | S_IRGRP;
	
	if(setbit==1)mode |= S_ISVTX;
	if(setbit==2)mode |= S_ISGID;
	if(setbit==3)mode |= S_ISGID | S_ISVTX;
	if(setbit==4)mode |= S_ISUID;
	if(setbit==5)mode |= S_ISVTX | S_ISUID;
	if(setbit==6)mode |= S_ISUID | S_ISGID;
	if(setbit==7)mode |= S_ISVTX | S_ISUID | S_ISGID;
	
	return mode;
}

int main(int argc, char *argv[]){
	int opt, status;
	char *filename, *smode;
	mode_t mode;

	while((opt = getopt(argc, argv, "m:v")) !=- 1){
		switch(opt){
			case 'm':
				flagm=1;
				smode = optarg;
				break;
			case 'v':
				flagv=1;
				break;
			case ':':
				return -1;
			default:
				fprintf(stderr, "usageErr: ls -[mv] [name]\n");
				return -1;
		}
	}

	if(optind==argc){
		fprintf(stderr, "mkdir: missing operand\n");
		return -1;
	}

	if(flagm){
		mode = findMode(smode);
		if(mode==-1)
			return -1;
	}else{
		mode = umode;
	}

	for(; optind<argc; optind++){
		filename = argv[optind];
		mode_t lmode = umask(0);
		status = mkdir(filename, mode);
		umask(lmode);
		if(status==-1){
			if(errno==ENOENT){
				fprintf(stderr, "mkdir: cannot create directory ‘%s’: No such file or directory\n", filename);
				continue;
			}else if(errno==EEXIST){
				fprintf(stderr, "mkdir: cannot create directory ‘%s’: File exists\n", filename);
				continue;
			}
			return -2;
		}
		if(flagv){
			printf("mkdir: created directory '%s'\n", filename);			
		}
	}

	return -1;


}
