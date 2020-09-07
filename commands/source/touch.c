#include "../../header.h"
#include <time.h>
#include <utime.h>

int flaga=0, flagc=0, flagd=0, flagh=0, flagm=0, flagr=0;

int main(int argc, char *argv[]){
	int opt;
	int status;
	char *filename, *gtime, *path;
	struct stat st, fst;
	time_t currtime;
	struct utimbuf uptime;

	while((opt = getopt(argc, argv, "acd:hmr:")) !=- 1){
		switch(opt){
			case 'a':
				flaga=1;
				break;
			case 'c':
				flagc=1;
				break;
			case 'd':
				flagd=1;
				gtime = optarg;
				break;
			case 'h':
				flagh=1;
				break;
			case 'm':
				flagm=1;
				break;
			case 'r':
				flagr=1;
				filename = optarg;
				break;
			case ':':
				return -1;
			default:
				fprintf(stderr, "usageErr: touch -[aAgGilLn] [name]\n");
				return -1;
		}
	}

	if(optind==argc){
		fprintf(stderr, "touch: missing file operand");
		return -1;
	}

	for(; optind<argc; optind++){
		path = argv[optind];
		
		if(flagh){
			status = lstat(path, &st);
		}else{
			status = stat(path, &st);
		}
		if(status==-1){
			if(errno==ENOENT){
				if(!flagc){
					if(open(path, O_RDONLY|O_CREAT|O_EXCL|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)==-1)
						return -2;
				}
				continue;
			}else
				return -2;
		}

		if(!flagr && !flagd){
			if(flaga){
				uptime.modtime = st.st_mtime;
				uptime.actime = time(NULL);
			}else if(flagm){
				uptime.modtime = time(NULL);
				uptime.actime = st.st_atime;
			}else{
				uptime.modtime = time(NULL);
				uptime.actime = time(NULL);
			}

			if(utime(path, &uptime)==-1)
				return -2;
		}

		if(flagr){
			if(stat(filename, &fst)==-1)
				return -2;

			if(flaga){
				uptime.modtime = st.st_mtime;
				uptime.actime = fst.st_atime;
			}else if(flagm){
				uptime.modtime = fst.st_mtime;
				uptime.actime = st.st_atime;
			}else{
				uptime.modtime = fst.st_mtime;
				uptime.actime = fst.st_atime;
			}
			
			if(utime(path, &uptime)==-1)
				return -2;
		}

		if(flagd){
			printf("Still to be implemented\n");
		}
	}

	return -1;


}
