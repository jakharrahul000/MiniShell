#include <sys/capability.h>
#include "header.h"

static char *read_line = (char *)NULL;

void initialize(){
  char *dir;
  dir = malloc(PATH_MAX);

  if(getcwd(dir, PATH_MAX)==NULL)
    errExit("getcwd");
  
  if(raiseCap(CAP_SYS_CHROOT)==-1)
    fatal("raiseCap() failed");
  if(chroot(dir)==-1)
    errExit("chroot");
  if(dropCap(CAP_SYS_CHROOT)==-1)
    fatal("dropCap() failed");
  
  if(chdir("/")==-1)
    errExit("chdir");

  using_history();
}

int main(int argc, char *argv[]){
  
  initialize();

  for(;;){
    if(read_line){
      free(read_line);
      read_line = (char *)NULL;
    }
    read_line = readline("\e[0;32m$ \e[0m");
    
    if(read_line == (char *)NULL)
      continue;
    if(read_line && *read_line){
      add_history(read_line);
      

    }
  
  }
  
  

}
