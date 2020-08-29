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
int executePipeCommands(char *command1, char *command2);
int executeOutputCommand(char *command, char *filename);
int executeInputCommand(char *command, char *filename);

#endif

/* builtin.c header */
#ifndef BUILTIN_H
#define BUILTIN_H

int checkBuiltin(char *cmd);
int executeBuiltin(int ind, char **cmd);

#endif

/* builtin_commands */
#ifndef BUILTIN_COMMANDS_H
#define BUILTIN_COMMANDS_H

int cd_(int argc, char *argv[]);
int echo_(int argc, char *argv[]);
int exit_(int argc, char *argv[]);
int help_(int argc, char *argv[]);
int jobs_(int argc, char *argv[]);
int kill_(int argc, char *argv[]);
int printf_(int argc, char *argv[]);
int pwd_(int argc, char *argv[]);
int umask_(int argc, char *argv[]);

#endif


/* helper.c header */
/*
#ifndef HELPER_H
#define HELPER_H

#endif
*/
