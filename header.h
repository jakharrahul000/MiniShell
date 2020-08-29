#ifndef HEADER
#define HEADER

#define _GNU_SOURCE
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <ctype.h>

#include <errno.h>
#include <string.h> 
#include <signal.h>

#include <sys/stat.h>
#include <sys/types.h> 

#include <readline/history.h>
#include <readline/readline.h>

#include "error_functions.h"
#include "function_header.h"

typedef enum { FALSE, TRUE } Boolean;
#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

#endif
