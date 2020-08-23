#ifndef HEADER
#define HEADER

#define _GNU_SOURCE
#define _XOPEN_SOURCE

#include <sys/types.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <ctype.h>

#include <errno.h>
#include <string.h> 

#include <readline/history.h>
#include <readline/readline.h>

#include "error_functions.h"
#include "capability.h"

typedef enum { FALSE, TRUE } Boolean;
#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

#endif
