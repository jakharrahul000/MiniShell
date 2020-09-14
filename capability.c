#include <sys/capability.h>
#include "header.h"

int modifyCap(int capability, int setting){
  cap_t caps;
  cap_value_t capList[1];

  caps = cap_get_proc();
  if (caps == NULL)return -1;

  capList[0] = capability;
  if (cap_set_flag(caps, CAP_EFFECTIVE, 1, capList, setting) == -1) {
    cap_free(caps);
		printf("error = %d\n", 1);
    return -1;
  }

  if (cap_set_proc(caps) == -1) {
    cap_free(caps);
    errMsg("cap_set_proc");
		return -1;
  }

  if (cap_free(caps) == -1){
		printf("error = %d\n", 3);
    return -1;
  }
	return 0;
}

int raiseCap(int capability){
   return modifyCap(capability, CAP_SET);
}

int dropCap(int capability){
  return modifyCap(capability, CAP_CLEAR);
}

int dropAllCaps(void){
  cap_t empty;
  int s;
  
  empty = cap_init();
  if (empty == NULL)
    return -1;
    
  s = cap_set_proc(empty);
  if (cap_free(empty) == -1)return -1;
      
  return s;
}

