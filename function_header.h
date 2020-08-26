/* capability.c header */
#ifndef CAPABILITY_H
#define CAPABILITY_H

int modifyCap(int capability, int setting);
int raiseCap(int capability);
int dropCap(int capability);
int dropAllCaps(void);

#endif

/* parse.c header  */
#ifndef PARSE_H
#define PARSE_H

char **parseEnv(char *PATH);
char **parseCommand(char *command);
char **parseCommands(char *read_line);
char **parsePipe(char *read_line);
char **parseOutputRedirection(char *read_line);
char **parseInputRedirection(char *read_line);
char *removeWhiteSpaces(char *line);

#endif

/* execute.c header */
#ifndef EXECUTE_H
#define EXECUTE_H

int executeCommand(char *command);

#endif
