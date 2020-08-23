#ifndef CAPABILITY_H
#define CAPABILITY_H

int modifyCap(int capability, int setting);
int raiseCap(int capability);
int dropCap(int capability);
int dropAllCaps(void);

#endif
