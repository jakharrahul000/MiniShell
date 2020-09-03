#include "../../header.h"
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#define FP_SPECIAL 1
#define STR_SIZE sizeof("-rwxrwxrwx")

int flaga=0, flagg=0, flagG=0, flagi=0, flagl=0, flagL=0, flagn=0;

char *filePermStr(struct stat *st, int flags){
	static char str[STR_SIZE];
	mode_t perm = st->st_mode;

	snprintf(str, STR_SIZE, "%c%c%c%c%c%c%c%c%c%c",
						(S_ISDIR(st->st_mode))?'d':(S_ISCHR(st->st_mode))?'c':
						(S_ISBLK(st->st_mode))?'b':(S_ISFIFO(st->st_mode))?'p':
						(S_ISSOCK(st->st_mode))?'s':(S_ISLNK(st->st_mode))?'l':'-',
						(perm & S_IRUSR) ? 'r' : '-', (perm & S_IWUSR) ? 'w' : '-',
		 				(perm & S_IXUSR) ?
								(((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
								(((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
						(perm & S_IRGRP) ? 'r' : '-', (perm & S_IWGRP) ? 'w' : '-',
						(perm & S_IXGRP) ?
								(((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
								(((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
						(perm & S_IROTH) ? 'r' : '-', (perm & S_IWOTH) ? 'w' : '-',
						(perm & S_IXOTH) ?
								(((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 't' : 'x') :
								(((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 'T' : '-')
						);
	return str;
}

char* getUserName(uid_t uid){
	struct passwd *pw;

	if((pw = getpwuid(uid))==NULL)
		return NULL;
	return pw->pw_name;
}

char *getGroupName(gid_t gid){
	struct group *grp;

	if((grp = getgrgid(gid))==NULL)
		return NULL;
	return grp->gr_name;
}

int checkExist(char *path){
	int fd;
	fd = open(path, O_RDONLY);
	if(fd==-1){
		if(errno==ENOENT)
			return -1;
		return -2;
	}
	return 0;
}

void printLongStat(struct stat *st, char *path){
	char name[PATH_MAX], linkName[PATH_MAX];
	struct tm *mod;
	
	if((st->st_mode & S_IFMT)==S_IFLNK){
		if(readlink(path, linkName, PATH_MAX)==-1)
			return;
		snprintf(name, PATH_MAX, "%s -> %s", path, linkName);
	}else{
		snprintf(name, PATH_MAX, "%s", path);
	}
	
	mod = localtime(&st->st_mtime);
	if(mod==NULL)
		return;

	if(flagi){
		printf("%5ld ", (long)st->st_ino);
	}
	printf("%s", filePermStr(st, 1));
	printf("%2ld ", (long)st->st_nlink);
	if(flagg==0){
		if(flagn){
			printf("%5ld ", (long)st->st_uid);
		}else{
			printf("%5s ", getUserName(st->st_uid));
		}
	}
	if(flagG==0){
		if(flagn){
			printf("%5ld ", (long)st->st_gid);
		}else{
			printf("%5s ", getGroupName(st->st_gid));
		}
	}
	printf("%5ld ", (long)st->st_size);
	
	printf("%s%3d %s%d%c%s%d ", 
					(mod->tm_mon==0)?"Jan":(mod->tm_mon==1)?"Feb":(mod->tm_mon==2)?"Mar":(mod->tm_mon==3)?"Apr":
					(mod->tm_mon==4)?"May":(mod->tm_mon==5)?"June":(mod->tm_mon==6)?"July":(mod->tm_mon==7)?"Aug":
					(mod->tm_mon==8)?"Sep":(mod->tm_mon==9)?"Oct":(mod->tm_mon==10)?"Nov":"Dec",
					mod->tm_mday, (mod->tm_hour<10)?"0":"", mod->tm_hour, ':', (mod->tm_min<10)?"0":"", mod->tm_min);

	if(S_ISDIR(st->st_mode))
		printf("\033[0;34m");
	if((st->st_mode & S_IXUSR) && !S_ISDIR(st->st_mode))
		printf("\033[0;32m");
	if((st->st_mode & S_IXUSR) && S_ISLNK(st->st_mode))
		printf("\033[1;34m");
	
	printf("%s\n", name);	
	printf("\033[0m");
}

int printDir(char *path){
	DIR *dir;
	struct dirent *dp;
	struct stat st;
	int status;
	char filename[PATH_MAX];

	dir = opendir(path);
	if(dir==NULL)
		return -2;

	if(flaga){
		snprintf(filename, PATH_MAX, "%s/%s", path, ".");
		if(stat(filename, &st)==-1)
			return -2;
			
		if(flagl){
			printLongStat(&st, ".");
		}else{
			printf("%s\n", dp->d_name);
		}

		snprintf(filename, PATH_MAX, "%s/%s", path, "..");
		if(stat(filename, &st)==-1)
			return -2;
			
		if(flagl){
			printLongStat(&st, "..");
		}else{
			printf("%s\n", dp->d_name);
		}
		
		for(;;){
			errno=0;
			dp = readdir(dir);
			if(dp==NULL)
				break;
		
			if((strcmp(dp->d_name, ".")==0 && strlen(dp->d_name)==1)  || 
					(strcmp(dp->d_name, "..")==0 && strlen(dp->d_name)==2) || 
					 dp->d_name[0] != '.')
				continue;
		
			snprintf(filename, PATH_MAX, "%s/%s", path, dp->d_name);
			
			if(stat(filename, &st)==-1)
				return -2;
			
			if(flagl){
				printLongStat(&st, dp->d_name);
			}else{
				printf("%s\n", dp->d_name);
			}
		}
	}
	closedir(dir);
	if(errno!=0)
		return -2;
	
	dir = opendir(path);
	if(dir==NULL)
		return -2;
	
	for(;;){
		errno=0;
		dp = readdir(dir);
		if(dp==NULL)
			break;
		if(dp->d_name[0]=='.')
			continue;

		snprintf(filename, PATH_MAX, "%s/%s", path, dp->d_name);

		if(flagL){
			if(stat(filename, &st)==-1)
				return -2;
		}else{
			if(lstat(filename, &st)==-1)
				return -2;
		}

		if(flagl){
			printLongStat(&st, dp->d_name);
		}else{
			printf("%s\n", dp->d_name);
		}

	}
	closedir(dir);
	if(errno!=0)
		return -2;
	return -1;
}

int printFile(char *path){
	struct stat st;
	int status;

	if(flagL){
		if(stat(path, &st)==-1)
			return -2;
	}else{
		if(lstat(path, &st)==-1){
			return -2;
		}
	}

	if((st.st_mode & S_IFMT)!=S_IFDIR){
		if(flagl){
			printLongStat(&st, path);
		}else{
			printf("%s\n", path);
		}
		return -1;
	}
	status = printDir(path);
	return status;
}

int main(int argc, char *argv[]){
	int opt;
	char path[PATH_MAX];
	int status;

	while((opt = getopt(argc, argv, "agGilLn")) !=- 1){
		switch(opt){
			case 'a':
				flaga=1;
				break;
			case 'g':
				flagg=1;
				break;
			case 'G':
				flagG=1;
				break;
			case 'i':
				flagi=1;
				break;
			case 'l':
				flagl=1;
				break;
			case 'L':
				flagL=1;
				break;
			case 'n':
				flagn=1;
				break;
			default:
				fprintf(stderr, "usageErr: ls -[aAgGilLn] [name]\n");
				return -1;
		}
	}

	if(optind==argc){
		status = printFile("./");
		return status;
	}

	for(int i=optind; i<argc; i++){
		status = checkExist(argv[i]);
		if(status==-2)
			return -2;
		if(status==-1){
			fprintf(stderr, "ls: cannot access '%s': No such file or directory\n", argv[i]);
			return -1;
		}
	}
	for(; optind<argc; optind++){
		if(checkExist(argv[optind])==0){
			status = printFile(argv[optind]);
			if(status==-2)
				return -2;
		}
	}
	return -1;


}
