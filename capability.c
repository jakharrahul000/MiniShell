#include <sys/capability.h>
#include "header.h"

/* Change setting of capability in caller's effective capabilities */
int modifyCap(int capability, int setting){
  cap_t caps;
  cap_value_t capList[1];

  /* Retrieve caller's current capabilities */
  caps = cap_get_proc();
  if (caps == NULL)return -1;

  /* Change setting of 'capability' in the effective set of 'caps'. */
  capList[0] = capability;
  if (cap_set_flag(caps, CAP_EFFECTIVE, 1, capList, setting) == -1) {
    cap_free(caps);
    return -1;
  }

  /* Push modified capability sets back to kernel, to change caller's capabilities */
   if (cap_set_proc(caps) == -1) {
     cap_free(caps);
     return -1;
   }

   /* Free the structure that was allocated by libcap */
   if (cap_free(caps) == -1)
     return -1;
     
   return 0;
}

/* Raise capability in caller's effective set */
int raiseCap(int capability){
   return modifyCap(capability, CAP_SET);
}

int dropCap(int capability){
  return modifyCap(capability, CAP_CLEAR);
}

/* Drop all capabilities from all sets */
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

