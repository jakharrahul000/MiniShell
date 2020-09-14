#include "header.h"
#include <pwd.h> 
#include <shadow.h>
#include <sys/capability.h>

/* Password authentication */
int authenticate(char *username){	
	char *password, *encrypted, *p, prompt[256];
	struct passwd *pwd;
	struct spwd *spwd;
	Boolean authOk;
	size_t len;
	long lnmax;
	int chance;

	if(raiseCap(CAP_DAC_OVERRIDE)==-1)
		return -2;
	errno = 0;
	pwd = getpwnam(username);
	if(pwd==NULL){
			if(errno==0){
				fprintf(stderr, "%s not found\n", username);
				return -1;
			}else{
				return -2;
			}
	}
	spwd = getspnam(username);
	if(spwd==NULL && errno==EACCES){
		errMsg("shadow\n");
		fatal("couldn't get password record");
		return -1;
	}
	if(spwd!=NULL)
		pwd->pw_passwd = spwd->sp_pwdp;
	if(dropCap(CAP_DAC_OVERRIDE)==1)
		return -2;

	chance=3;
	authOk=0;
	snprintf(prompt, 256, "[sudo] password for %s: ", username);
	while(chance>0){
		password = getpass(prompt);
		encrypted = crypt(password, pwd->pw_passwd);
		for(p=password;*p!='\0';)
			*p++='\0';

		if(encrypted==NULL)
			return -2;

		authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
		if(!authOk){
			fprintf(stderr, "Sorry, try again\n");	
		}else break;
		chance--;
	}
	if(!authOk){
		fprintf(stderr, "sudo: 3 incorrect password attempts\n");
		return 0;
	}
	return 1;
} 
